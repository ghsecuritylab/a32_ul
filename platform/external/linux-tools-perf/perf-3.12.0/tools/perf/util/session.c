#include <linux/kernel.h>
#include <traceevent/event-parse.h>

#include <byteswap.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/mman.h>

#include "evlist.h"
#include "evsel.h"
#include "session.h"
#include "tool.h"
#include "sort.h"
#include "util.h"
#include "cpumap.h"
#include "perf_regs.h"
#include "vdso.h"

static int perf_session__open(struct perf_session *self, bool force)
{
	struct stat input_stat;

	if (!strcmp(self->filename, "-")) {
		self->fd_pipe = true;
		self->fd = STDIN_FILENO;

		if (perf_session__read_header(self) < 0)
			pr_err("incompatible file format (rerun with -v to learn more)");

		return 0;
	}

	self->fd = open(self->filename, O_RDONLY);
	if (self->fd < 0) {
		int err = errno;

		pr_err("failed to open %s: %s", self->filename, strerror(err));
		if (err == ENOENT && !strcmp(self->filename, "perf.data"))
			pr_err("  (try 'perf record' first)");
		pr_err("\n");
		return -errno;
	}

	if (fstat(self->fd, &input_stat) < 0)
		goto out_close;

	if (!force && input_stat.st_uid && (input_stat.st_uid != geteuid())) {
		pr_err("file %s not owned by current user or root\n",
		       self->filename);
		goto out_close;
	}

	if (!input_stat.st_size) {
		pr_info("zero-sized file (%s), nothing to do!\n",
			self->filename);
		goto out_close;
	}

	if (perf_session__read_header(self) < 0) {
		pr_err("incompatible file format (rerun with -v to learn more)");
		goto out_close;
	}

	if (!perf_evlist__valid_sample_type(self->evlist)) {
		pr_err("non matching sample_type");
		goto out_close;
	}

	if (!perf_evlist__valid_sample_id_all(self->evlist)) {
		pr_err("non matching sample_id_all");
		goto out_close;
	}

	if (!perf_evlist__valid_read_format(self->evlist)) {
		pr_err("non matching read_format");
		goto out_close;
	}

	self->size = input_stat.st_size;
	return 0;

out_close:
	close(self->fd);
	self->fd = -1;
	return -1;
}

void perf_session__set_id_hdr_size(struct perf_session *session)
{
	u16 id_hdr_size = perf_evlist__id_hdr_size(session->evlist);

	machines__set_id_hdr_size(&session->machines, id_hdr_size);
}

int perf_session__create_kernel_maps(struct perf_session *self)
{
	int ret = machine__create_kernel_maps(&self->machines.host);

	if (ret >= 0)
		ret = machines__create_guest_kernel_maps(&self->machines);
	return ret;
}

static void perf_session__destroy_kernel_maps(struct perf_session *self)
{
	machines__destroy_kernel_maps(&self->machines);
}

struct perf_session *perf_session__new(const char *filename, int mode,
				       bool force, bool repipe,
				       struct perf_tool *tool)
{
	struct perf_session *self;
	struct stat st;
	size_t len;

	if (!filename || !strlen(filename)) {
		if (!fstat(STDIN_FILENO, &st) && S_ISFIFO(st.st_mode))
			filename = "-";
		else
			filename = "perf.data";
	}

	len = strlen(filename);
	self = zalloc(sizeof(*self) + len);

	if (self == NULL)
		goto out;

	memcpy(self->filename, filename, len);
	self->repipe = repipe;
	INIT_LIST_HEAD(&self->ordered_samples.samples);
	INIT_LIST_HEAD(&self->ordered_samples.sample_cache);
	INIT_LIST_HEAD(&self->ordered_samples.to_free);
	machines__init(&self->machines);

	if (mode == O_RDONLY) {
		if (perf_session__open(self, force) < 0)
			goto out_delete;
		perf_session__set_id_hdr_size(self);
	} else if (mode == O_WRONLY) {
		if (perf_session__create_kernel_maps(self) < 0)
			goto out_delete;
	}

	if (tool && tool->ordering_requires_timestamps &&
	    tool->ordered_samples && !perf_evlist__sample_id_all(self->evlist)) {
		dump_printf("WARNING: No sample_id_all support, falling back to unordered processing\n");
		tool->ordered_samples = false;
	}

out:
	return self;
out_delete:
	perf_session__delete(self);
	return NULL;
}

static void perf_session__delete_dead_threads(struct perf_session *session)
{
	machine__delete_dead_threads(&session->machines.host);
}

static void perf_session__delete_threads(struct perf_session *session)
{
	machine__delete_threads(&session->machines.host);
}

static void perf_session_env__delete(struct perf_session_env *env)
{
	free(env->hostname);
	free(env->os_release);
	free(env->version);
	free(env->arch);
	free(env->cpu_desc);
	free(env->cpuid);

	free(env->cmdline);
	free(env->sibling_cores);
	free(env->sibling_threads);
	free(env->numa_nodes);
	free(env->pmu_mappings);
}

void perf_session__delete(struct perf_session *self)
{
	perf_session__destroy_kernel_maps(self);
	perf_session__delete_dead_threads(self);
	perf_session__delete_threads(self);
	perf_session_env__delete(&self->header.env);
	machines__exit(&self->machines);
	close(self->fd);
	free(self);
	vdso__exit();
}

static int process_event_synth_tracing_data_stub(struct perf_tool *tool
						 __maybe_unused,
						 union perf_event *event
						 __maybe_unused,
						 struct perf_session *session
						__maybe_unused)
{
	dump_printf(": unhandled!\n");
	return 0;
}

static int process_event_synth_attr_stub(struct perf_tool *tool __maybe_unused,
					 union perf_event *event __maybe_unused,
					 struct perf_evlist **pevlist
					 __maybe_unused)
{
	dump_printf(": unhandled!\n");
	return 0;
}

static int process_event_sample_stub(struct perf_tool *tool __maybe_unused,
				     union perf_event *event __maybe_unused,
				     struct perf_sample *sample __maybe_unused,
				     struct perf_evsel *evsel __maybe_unused,
				     struct machine *machine __maybe_unused)
{
	dump_printf(": unhandled!\n");
	return 0;
}

static int process_event_stub(struct perf_tool *tool __maybe_unused,
			      union perf_event *event __maybe_unused,
			      struct perf_sample *sample __maybe_unused,
			      struct machine *machine __maybe_unused)
{
	dump_printf(": unhandled!\n");
	return 0;
}

static int process_finished_round_stub(struct perf_tool *tool __maybe_unused,
				       union perf_event *event __maybe_unused,
				       struct perf_session *perf_session
				       __maybe_unused)
{
	dump_printf(": unhandled!\n");
	return 0;
}

static int process_finished_round(struct perf_tool *tool,
				  union perf_event *event,
				  struct perf_session *session);

void perf_tool__fill_defaults(struct perf_tool *tool)
{
	if (tool->sample == NULL)
		tool->sample = process_event_sample_stub;
	if (tool->mmap == NULL)
		tool->mmap = process_event_stub;
	if (tool->mmap2 == NULL)
		tool->mmap2 = process_event_stub;
	if (tool->comm == NULL)
		tool->comm = process_event_stub;
	if (tool->fork == NULL)
		tool->fork = process_event_stub;
	if (tool->exit == NULL)
		tool->exit = process_event_stub;
	if (tool->lost == NULL)
		tool->lost = perf_event__process_lost;
	if (tool->read == NULL)
		tool->read = process_event_sample_stub;
	if (tool->throttle == NULL)
		tool->throttle = process_event_stub;
	if (tool->unthrottle == NULL)
		tool->unthrottle = process_event_stub;
	if (tool->attr == NULL)
		tool->attr = process_event_synth_attr_stub;
	if (tool->tracing_data == NULL)
		tool->tracing_data = process_event_synth_tracing_data_stub;
	if (tool->build_id == NULL)
		tool->build_id = process_finished_round_stub;
	if (tool->finished_round == NULL) {
		if (tool->ordered_samples)
			tool->finished_round = process_finished_round;
		else
			tool->finished_round = process_finished_round_stub;
	}
}
 
void mem_bswap_32(void *src, int byte_size)
{
	u32 *m = src;
	while (byte_size > 0) {
		*m = bswap_32(*m);
		byte_size -= sizeof(u32);
		++m;
	}
}

void mem_bswap_64(void *src, int byte_size)
{
	u64 *m = src;

	while (byte_size > 0) {
		*m = bswap_64(*m);
		byte_size -= sizeof(u64);
		++m;
	}
}

static void swap_sample_id_all(union perf_event *event, void *data)
{
	void *end = (void *) event + event->header.size;
	int size = end - data;

	BUG_ON(size % sizeof(u64));
	mem_bswap_64(data, size);
}

static void perf_event__all64_swap(union perf_event *event,
				   bool sample_id_all __maybe_unused)
{
	struct perf_event_header *hdr = &event->header;
	mem_bswap_64(hdr + 1, event->header.size - sizeof(*hdr));
}

static void perf_event__comm_swap(union perf_event *event, bool sample_id_all)
{
	event->comm.pid = bswap_32(event->comm.pid);
	event->comm.tid = bswap_32(event->comm.tid);

	if (sample_id_all) {
		void *data = &event->comm.comm;

		data += PERF_ALIGN(strlen(data) + 1, sizeof(u64));
		swap_sample_id_all(event, data);
	}
}

static void perf_event__mmap_swap(union perf_event *event,
				  bool sample_id_all)
{
	event->mmap.pid	  = bswap_32(event->mmap.pid);
	event->mmap.tid	  = bswap_32(event->mmap.tid);
	event->mmap.start = bswap_64(event->mmap.start);
	event->mmap.len	  = bswap_64(event->mmap.len);
	event->mmap.pgoff = bswap_64(event->mmap.pgoff);

	if (sample_id_all) {
		void *data = &event->mmap.filename;

		data += PERF_ALIGN(strlen(data) + 1, sizeof(u64));
		swap_sample_id_all(event, data);
	}
}

static void perf_event__mmap2_swap(union perf_event *event,
				  bool sample_id_all)
{
	event->mmap2.pid   = bswap_32(event->mmap2.pid);
	event->mmap2.tid   = bswap_32(event->mmap2.tid);
	event->mmap2.start = bswap_64(event->mmap2.start);
	event->mmap2.len   = bswap_64(event->mmap2.len);
	event->mmap2.pgoff = bswap_64(event->mmap2.pgoff);
	event->mmap2.maj   = bswap_32(event->mmap2.maj);
	event->mmap2.min   = bswap_32(event->mmap2.min);
	event->mmap2.ino   = bswap_64(event->mmap2.ino);

	if (sample_id_all) {
		void *data = &event->mmap2.filename;

		data += PERF_ALIGN(strlen(data) + 1, sizeof(u64));
		swap_sample_id_all(event, data);
	}
}
static void perf_event__task_swap(union perf_event *event, bool sample_id_all)
{
	event->fork.pid	 = bswap_32(event->fork.pid);
	event->fork.tid	 = bswap_32(event->fork.tid);
	event->fork.ppid = bswap_32(event->fork.ppid);
	event->fork.ptid = bswap_32(event->fork.ptid);
	event->fork.time = bswap_64(event->fork.time);

	if (sample_id_all)
		swap_sample_id_all(event, &event->fork + 1);
}

static void perf_event__read_swap(union perf_event *event, bool sample_id_all)
{
	event->read.pid		 = bswap_32(event->read.pid);
	event->read.tid		 = bswap_32(event->read.tid);
	event->read.value	 = bswap_64(event->read.value);
	event->read.time_enabled = bswap_64(event->read.time_enabled);
	event->read.time_running = bswap_64(event->read.time_running);
	event->read.id		 = bswap_64(event->read.id);

	if (sample_id_all)
		swap_sample_id_all(event, &event->read + 1);
}

static u8 revbyte(u8 b)
{
	int rev = (b >> 4) | ((b & 0xf) << 4);
	rev = ((rev & 0xcc) >> 2) | ((rev & 0x33) << 2);
	rev = ((rev & 0xaa) >> 1) | ((rev & 0x55) << 1);
	return (u8) rev;
}

static void swap_bitfield(u8 *p, unsigned len)
{
	unsigned i;

	for (i = 0; i < len; i++) {
		*p = revbyte(*p);
		p++;
	}
}

void perf_event__attr_swap(struct perf_event_attr *attr)
{
	attr->type		= bswap_32(attr->type);
	attr->size		= bswap_32(attr->size);
	attr->config		= bswap_64(attr->config);
	attr->sample_period	= bswap_64(attr->sample_period);
	attr->sample_type	= bswap_64(attr->sample_type);
	attr->read_format	= bswap_64(attr->read_format);
	attr->wakeup_events	= bswap_32(attr->wakeup_events);
	attr->bp_type		= bswap_32(attr->bp_type);
	attr->bp_addr		= bswap_64(attr->bp_addr);
	attr->bp_len		= bswap_64(attr->bp_len);

	swap_bitfield((u8 *) (&attr->read_format + 1), sizeof(u64));
}

static void perf_event__hdr_attr_swap(union perf_event *event,
				      bool sample_id_all __maybe_unused)
{
	size_t size;

	perf_event__attr_swap(&event->attr.attr);

	size = event->header.size;
	size -= (void *)&event->attr.id - (void *)event;
	mem_bswap_64(event->attr.id, size);
}

static void perf_event__event_type_swap(union perf_event *event,
					bool sample_id_all __maybe_unused)
{
	event->event_type.event_type.event_id =
		bswap_64(event->event_type.event_type.event_id);
}

static void perf_event__tracing_data_swap(union perf_event *event,
					  bool sample_id_all __maybe_unused)
{
	event->tracing_data.size = bswap_32(event->tracing_data.size);
}

typedef void (*perf_event__swap_op)(union perf_event *event,
				    bool sample_id_all);

static perf_event__swap_op perf_event__swap_ops[] = {
	[PERF_RECORD_MMAP]		  = perf_event__mmap_swap,
	[PERF_RECORD_MMAP2]		  = perf_event__mmap2_swap,
	[PERF_RECORD_COMM]		  = perf_event__comm_swap,
	[PERF_RECORD_FORK]		  = perf_event__task_swap,
	[PERF_RECORD_EXIT]		  = perf_event__task_swap,
	[PERF_RECORD_LOST]		  = perf_event__all64_swap,
	[PERF_RECORD_READ]		  = perf_event__read_swap,
	[PERF_RECORD_SAMPLE]		  = perf_event__all64_swap,
	[PERF_RECORD_HEADER_ATTR]	  = perf_event__hdr_attr_swap,
	[PERF_RECORD_HEADER_EVENT_TYPE]	  = perf_event__event_type_swap,
	[PERF_RECORD_HEADER_TRACING_DATA] = perf_event__tracing_data_swap,
	[PERF_RECORD_HEADER_BUILD_ID]	  = NULL,
	[PERF_RECORD_HEADER_MAX]	  = NULL,
};

struct sample_queue {
	u64			timestamp;
	u64			file_offset;
	union perf_event	*event;
	struct list_head	list;
};

static void perf_session_free_sample_buffers(struct perf_session *session)
{
	struct ordered_samples *os = &session->ordered_samples;

	while (!list_empty(&os->to_free)) {
		struct sample_queue *sq;

		sq = list_entry(os->to_free.next, struct sample_queue, list);
		list_del(&sq->list);
		free(sq);
	}
}

static int perf_session_deliver_event(struct perf_session *session,
				      union perf_event *event,
				      struct perf_sample *sample,
				      struct perf_tool *tool,
				      u64 file_offset);

static int flush_sample_queue(struct perf_session *s,
		       struct perf_tool *tool)
{
	struct ordered_samples *os = &s->ordered_samples;
	struct list_head *head = &os->samples;
	struct sample_queue *tmp, *iter;
	struct perf_sample sample;
	u64 limit = os->next_flush;
	u64 last_ts = os->last_sample ? os->last_sample->timestamp : 0ULL;
	unsigned idx = 0, progress_next = os->nr_samples / 16;
	bool show_progress = limit == ULLONG_MAX;
	int ret;

	if (!tool->ordered_samples || !limit)
		return 0;

	list_for_each_entry_safe(iter, tmp, head, list) {
		if (session_done())
			return 0;

		if (iter->timestamp > limit)
			break;

		ret = perf_evlist__parse_sample(s->evlist, iter->event, &sample);
		if (ret)
			pr_err("Can't parse sample, err = %d\n", ret);
		else {
			ret = perf_session_deliver_event(s, iter->event, &sample, tool,
							 iter->file_offset);
			if (ret)
				return ret;
		}

		os->last_flush = iter->timestamp;
		list_del(&iter->list);
		list_add(&iter->list, &os->sample_cache);
		if (show_progress && (++idx >= progress_next)) {
			progress_next += os->nr_samples / 16;
			ui_progress__update(idx, os->nr_samples,
					    "Processing time ordered events...");
		}
	}

	if (list_empty(head)) {
		os->last_sample = NULL;
	} else if (last_ts <= limit) {
		os->last_sample =
			list_entry(head->prev, struct sample_queue, list);
	}

	os->nr_samples = 0;

	return 0;
}

static int process_finished_round(struct perf_tool *tool,
				  union perf_event *event __maybe_unused,
				  struct perf_session *session)
{
	int ret = flush_sample_queue(session, tool);
	if (!ret)
		session->ordered_samples.next_flush = session->ordered_samples.max_timestamp;

	return ret;
}

static void __queue_event(struct sample_queue *new, struct perf_session *s)
{
	struct ordered_samples *os = &s->ordered_samples;
	struct sample_queue *sample = os->last_sample;
	u64 timestamp = new->timestamp;
	struct list_head *p;

	++os->nr_samples;
	os->last_sample = new;

	if (!sample) {
		list_add(&new->list, &os->samples);
		os->max_timestamp = timestamp;
		return;
	}

	if (sample->timestamp <= timestamp) {
		while (sample->timestamp <= timestamp) {
			p = sample->list.next;
			if (p == &os->samples) {
				list_add_tail(&new->list, &os->samples);
				os->max_timestamp = timestamp;
				return;
			}
			sample = list_entry(p, struct sample_queue, list);
		}
		list_add_tail(&new->list, &sample->list);
	} else {
		while (sample->timestamp > timestamp) {
			p = sample->list.prev;
			if (p == &os->samples) {
				list_add(&new->list, &os->samples);
				return;
			}
			sample = list_entry(p, struct sample_queue, list);
		}
		list_add(&new->list, &sample->list);
	}
}

#define MAX_SAMPLE_BUFFER	(64 * 1024 / sizeof(struct sample_queue))

int perf_session_queue_event(struct perf_session *s, union perf_event *event,
				    struct perf_sample *sample, u64 file_offset)
{
	struct ordered_samples *os = &s->ordered_samples;
	struct list_head *sc = &os->sample_cache;
	u64 timestamp = sample->time;
	struct sample_queue *new;

	if (!timestamp || timestamp == ~0ULL)
		return -ETIME;

	if (timestamp < s->ordered_samples.last_flush) {
		printf("Warning: Timestamp below last timeslice flush\n");
		return -EINVAL;
	}

	if (!list_empty(sc)) {
		new = list_entry(sc->next, struct sample_queue, list);
		list_del(&new->list);
	} else if (os->sample_buffer) {
		new = os->sample_buffer + os->sample_buffer_idx;
		if (++os->sample_buffer_idx == MAX_SAMPLE_BUFFER)
			os->sample_buffer = NULL;
	} else {
		os->sample_buffer = malloc(MAX_SAMPLE_BUFFER * sizeof(*new));
		if (!os->sample_buffer)
			return -ENOMEM;
		list_add(&os->sample_buffer->list, &os->to_free);
		os->sample_buffer_idx = 2;
		new = os->sample_buffer + 1;
	}

	new->timestamp = timestamp;
	new->file_offset = file_offset;
	new->event = event;

	__queue_event(new, s);

	return 0;
}

static void callchain__printf(struct perf_sample *sample)
{
	unsigned int i;

	printf("... chain: nr:%" PRIu64 "\n", sample->callchain->nr);

	for (i = 0; i < sample->callchain->nr; i++)
		printf("..... %2d: %016" PRIx64 "\n",
		       i, sample->callchain->ips[i]);
}

static void branch_stack__printf(struct perf_sample *sample)
{
	uint64_t i;

	printf("... branch stack: nr:%" PRIu64 "\n", sample->branch_stack->nr);

	for (i = 0; i < sample->branch_stack->nr; i++)
		printf("..... %2"PRIu64": %016" PRIx64 " -> %016" PRIx64 "\n",
			i, sample->branch_stack->entries[i].from,
			sample->branch_stack->entries[i].to);
}

static void regs_dump__printf(u64 mask, u64 *regs)
{
	unsigned rid, i = 0;

	for_each_set_bit(rid, (unsigned long *) &mask, sizeof(mask) * 8) {
		u64 val = regs[i++];

		printf(".... %-5s 0x%" PRIx64 "\n",
		       perf_reg_name(rid), val);
	}
}

static void regs_user__printf(struct perf_sample *sample, u64 mask)
{
	struct regs_dump *user_regs = &sample->user_regs;

	if (user_regs->regs) {
		printf("... user regs: mask 0x%" PRIx64 "\n", mask);
		regs_dump__printf(mask, user_regs->regs);
	}
}

static void stack_user__printf(struct stack_dump *dump)
{
	printf("... ustack: size %" PRIu64 ", offset 0x%x\n",
	       dump->size, dump->offset);
}

static void perf_session__print_tstamp(struct perf_session *session,
				       union perf_event *event,
				       struct perf_sample *sample)
{
	u64 sample_type = __perf_evlist__combined_sample_type(session->evlist);

	if (event->header.type != PERF_RECORD_SAMPLE &&
	    !perf_evlist__sample_id_all(session->evlist)) {
		fputs("-1 -1 ", stdout);
		return;
	}

	if ((sample_type & PERF_SAMPLE_CPU))
		printf("%u ", sample->cpu);

	if (sample_type & PERF_SAMPLE_TIME)
		printf("%" PRIu64 " ", sample->time);
}

static void sample_read__printf(struct perf_sample *sample, u64 read_format)
{
	printf("... sample_read:\n");

	if (read_format & PERF_FORMAT_TOTAL_TIME_ENABLED)
		printf("...... time enabled %016" PRIx64 "\n",
		       sample->read.time_enabled);

	if (read_format & PERF_FORMAT_TOTAL_TIME_RUNNING)
		printf("...... time running %016" PRIx64 "\n",
		       sample->read.time_running);

	if (read_format & PERF_FORMAT_GROUP) {
		u64 i;

		printf(".... group nr %" PRIu64 "\n", sample->read.group.nr);

		for (i = 0; i < sample->read.group.nr; i++) {
			struct sample_read_value *value;

			value = &sample->read.group.values[i];
			printf("..... id %016" PRIx64
			       ", value %016" PRIx64 "\n",
			       value->id, value->value);
		}
	} else
		printf("..... id %016" PRIx64 ", value %016" PRIx64 "\n",
			sample->read.one.id, sample->read.one.value);
}

static void dump_event(struct perf_session *session, union perf_event *event,
		       u64 file_offset, struct perf_sample *sample)
{
	if (!dump_trace)
		return;

	printf("\n%#" PRIx64 " [%#x]: event: %d\n",
	       file_offset, event->header.size, event->header.type);

	trace_event(event);

	if (sample)
		perf_session__print_tstamp(session, event, sample);

	printf("%#" PRIx64 " [%#x]: PERF_RECORD_%s", file_offset,
	       event->header.size, perf_event__name(event->header.type));
}

static void dump_sample(struct perf_evsel *evsel, union perf_event *event,
			struct perf_sample *sample)
{
	u64 sample_type;

	if (!dump_trace)
		return;

	printf("(IP, %d): %d/%d: %#" PRIx64 " period: %" PRIu64 " addr: %#" PRIx64 "\n",
	       event->header.misc, sample->pid, sample->tid, sample->ip,
	       sample->period, sample->addr);

	sample_type = evsel->attr.sample_type;

	if (sample_type & PERF_SAMPLE_CALLCHAIN)
		callchain__printf(sample);

	if (sample_type & PERF_SAMPLE_BRANCH_STACK)
		branch_stack__printf(sample);

	if (sample_type & PERF_SAMPLE_REGS_USER)
		regs_user__printf(sample, evsel->attr.sample_regs_user);

	if (sample_type & PERF_SAMPLE_STACK_USER)
		stack_user__printf(&sample->user_stack);

	if (sample_type & PERF_SAMPLE_WEIGHT)
		printf("... weight: %" PRIu64 "\n", sample->weight);

	if (sample_type & PERF_SAMPLE_DATA_SRC)
		printf(" . data_src: 0x%"PRIx64"\n", sample->data_src);

	if (sample_type & PERF_SAMPLE_READ)
		sample_read__printf(sample, evsel->attr.read_format);
}

static struct machine *
	perf_session__find_machine_for_cpumode(struct perf_session *session,
					       union perf_event *event,
					       struct perf_sample *sample)
{
	const u8 cpumode = event->header.misc & PERF_RECORD_MISC_CPUMODE_MASK;

	if (perf_guest &&
	    ((cpumode == PERF_RECORD_MISC_GUEST_KERNEL) ||
	     (cpumode == PERF_RECORD_MISC_GUEST_USER))) {
		u32 pid;

		if (event->header.type == PERF_RECORD_MMAP
		    || event->header.type == PERF_RECORD_MMAP2)
			pid = event->mmap.pid;
		else
			pid = sample->pid;

		return perf_session__findnew_machine(session, pid);
	}

	return &session->machines.host;
}

static int deliver_sample_value(struct perf_session *session,
				struct perf_tool *tool,
				union perf_event *event,
				struct perf_sample *sample,
				struct sample_read_value *v,
				struct machine *machine)
{
	struct perf_sample_id *sid;

	sid = perf_evlist__id2sid(session->evlist, v->id);
	if (sid) {
		sample->id     = v->id;
		sample->period = v->value - sid->period;
		sid->period    = v->value;
	}

	if (!sid || sid->evsel == NULL) {
		++session->stats.nr_unknown_id;
		return 0;
	}

	return tool->sample(tool, event, sample, sid->evsel, machine);
}

static int deliver_sample_group(struct perf_session *session,
				struct perf_tool *tool,
				union  perf_event *event,
				struct perf_sample *sample,
				struct machine *machine)
{
	int ret = -EINVAL;
	u64 i;

	for (i = 0; i < sample->read.group.nr; i++) {
		ret = deliver_sample_value(session, tool, event, sample,
					   &sample->read.group.values[i],
					   machine);
		if (ret)
			break;
	}

	return ret;
}

static int
perf_session__deliver_sample(struct perf_session *session,
			     struct perf_tool *tool,
			     union  perf_event *event,
			     struct perf_sample *sample,
			     struct perf_evsel *evsel,
			     struct machine *machine)
{
	
	u64 sample_type = evsel->attr.sample_type;
	u64 read_format = evsel->attr.read_format;

	
	if (!(sample_type & PERF_SAMPLE_READ))
		return tool->sample(tool, event, sample, evsel, machine);

	
	if (read_format & PERF_FORMAT_GROUP)
		return deliver_sample_group(session, tool, event, sample,
					    machine);
	else
		return deliver_sample_value(session, tool, event, sample,
					    &sample->read.one, machine);
}

static int perf_session_deliver_event(struct perf_session *session,
				      union perf_event *event,
				      struct perf_sample *sample,
				      struct perf_tool *tool,
				      u64 file_offset)
{
	struct perf_evsel *evsel;
	struct machine *machine;

	dump_event(session, event, file_offset, sample);

	evsel = perf_evlist__id2evsel(session->evlist, sample->id);
	if (evsel != NULL && event->header.type != PERF_RECORD_SAMPLE) {
		hists__inc_nr_events(&evsel->hists, event->header.type);
	}

	machine = perf_session__find_machine_for_cpumode(session, event,
							 sample);

	switch (event->header.type) {
	case PERF_RECORD_SAMPLE:
		dump_sample(evsel, event, sample);
		if (evsel == NULL) {
			++session->stats.nr_unknown_id;
			return 0;
		}
		if (machine == NULL) {
			++session->stats.nr_unprocessable_samples;
			return 0;
		}
		return perf_session__deliver_sample(session, tool, event,
						    sample, evsel, machine);
	case PERF_RECORD_MMAP:
		return tool->mmap(tool, event, sample, machine);
	case PERF_RECORD_MMAP2:
		return tool->mmap2(tool, event, sample, machine);
	case PERF_RECORD_COMM:
		return tool->comm(tool, event, sample, machine);
	case PERF_RECORD_FORK:
		return tool->fork(tool, event, sample, machine);
	case PERF_RECORD_EXIT:
		return tool->exit(tool, event, sample, machine);
	case PERF_RECORD_LOST:
		if (tool->lost == perf_event__process_lost)
			session->stats.total_lost += event->lost.lost;
		return tool->lost(tool, event, sample, machine);
	case PERF_RECORD_READ:
		return tool->read(tool, event, sample, evsel, machine);
	case PERF_RECORD_THROTTLE:
		return tool->throttle(tool, event, sample, machine);
	case PERF_RECORD_UNTHROTTLE:
		return tool->unthrottle(tool, event, sample, machine);
	default:
		++session->stats.nr_unknown_events;
		return -1;
	}
}

static int perf_session__process_user_event(struct perf_session *session, union perf_event *event,
					    struct perf_tool *tool, u64 file_offset)
{
	int err;

	dump_event(session, event, file_offset, NULL);

	
	switch (event->header.type) {
	case PERF_RECORD_HEADER_ATTR:
		err = tool->attr(tool, event, &session->evlist);
		if (err == 0)
			perf_session__set_id_hdr_size(session);
		return err;
	case PERF_RECORD_HEADER_TRACING_DATA:
		
		lseek(session->fd, file_offset, SEEK_SET);
		return tool->tracing_data(tool, event, session);
	case PERF_RECORD_HEADER_BUILD_ID:
		return tool->build_id(tool, event, session);
	case PERF_RECORD_FINISHED_ROUND:
		return tool->finished_round(tool, event, session);
	default:
		return -EINVAL;
	}
}

static void event_swap(union perf_event *event, bool sample_id_all)
{
	perf_event__swap_op swap;

	swap = perf_event__swap_ops[event->header.type];
	if (swap)
		swap(event, sample_id_all);
}

static int perf_session__process_event(struct perf_session *session,
				       union perf_event *event,
				       struct perf_tool *tool,
				       u64 file_offset)
{
	struct perf_sample sample;
	int ret;

	if (session->header.needs_swap)
		event_swap(event, perf_evlist__sample_id_all(session->evlist));

	if (event->header.type >= PERF_RECORD_HEADER_MAX)
		return -EINVAL;

	events_stats__inc(&session->stats, event->header.type);

	if (event->header.type >= PERF_RECORD_USER_TYPE_START)
		return perf_session__process_user_event(session, event, tool, file_offset);

	ret = perf_evlist__parse_sample(session->evlist, event, &sample);
	if (ret)
		return ret;

	if (tool->ordered_samples) {
		ret = perf_session_queue_event(session, event, &sample,
					       file_offset);
		if (ret != -ETIME)
			return ret;
	}

	return perf_session_deliver_event(session, event, &sample, tool,
					  file_offset);
}

void perf_event_header__bswap(struct perf_event_header *self)
{
	self->type = bswap_32(self->type);
	self->misc = bswap_16(self->misc);
	self->size = bswap_16(self->size);
}

struct thread *perf_session__findnew(struct perf_session *session, pid_t pid)
{
	return machine__findnew_thread(&session->machines.host, 0, pid);
}

static struct thread *perf_session__register_idle_thread(struct perf_session *self)
{
	struct thread *thread = perf_session__findnew(self, 0);

	if (thread == NULL || thread__set_comm(thread, "swapper")) {
		pr_err("problem inserting idle task.\n");
		thread = NULL;
	}

	return thread;
}

static void perf_session__warn_about_errors(const struct perf_session *session,
					    const struct perf_tool *tool)
{
	if (tool->lost == perf_event__process_lost &&
	    session->stats.nr_events[PERF_RECORD_LOST] != 0) {
		ui__warning("Processed %d events and lost %d chunks!\n\n"
			    "Check IO/CPU overload!\n\n",
			    session->stats.nr_events[0],
			    session->stats.nr_events[PERF_RECORD_LOST]);
	}

	if (session->stats.nr_unknown_events != 0) {
		ui__warning("Found %u unknown events!\n\n"
			    "Is this an older tool processing a perf.data "
			    "file generated by a more recent tool?\n\n"
			    "If that is not the case, consider "
			    "reporting to linux-kernel@vger.kernel.org.\n\n",
			    session->stats.nr_unknown_events);
	}

	if (session->stats.nr_unknown_id != 0) {
		ui__warning("%u samples with id not present in the header\n",
			    session->stats.nr_unknown_id);
	}

 	if (session->stats.nr_invalid_chains != 0) {
 		ui__warning("Found invalid callchains!\n\n"
 			    "%u out of %u events were discarded for this reason.\n\n"
 			    "Consider reporting to linux-kernel@vger.kernel.org.\n\n",
 			    session->stats.nr_invalid_chains,
 			    session->stats.nr_events[PERF_RECORD_SAMPLE]);
 	}

	if (session->stats.nr_unprocessable_samples != 0) {
		ui__warning("%u unprocessable samples recorded.\n"
			    "Do you have a KVM guest running and not using 'perf kvm'?\n",
			    session->stats.nr_unprocessable_samples);
	}
}

volatile int session_done;

static int __perf_session__process_pipe_events(struct perf_session *self,
					       struct perf_tool *tool)
{
	union perf_event *event;
	uint32_t size, cur_size = 0;
	void *buf = NULL;
	int skip = 0;
	u64 head;
	int err;
	void *p;

	perf_tool__fill_defaults(tool);

	head = 0;
	cur_size = sizeof(union perf_event);

	buf = malloc(cur_size);
	if (!buf)
		return -errno;
more:
	event = buf;
	err = readn(self->fd, event, sizeof(struct perf_event_header));
	if (err <= 0) {
		if (err == 0)
			goto done;

		pr_err("failed to read event header\n");
		goto out_err;
	}

	if (self->header.needs_swap)
		perf_event_header__bswap(&event->header);

	size = event->header.size;
	if (size < sizeof(struct perf_event_header)) {
		pr_err("bad event header size\n");
		goto out_err;
	}

	if (size > cur_size) {
		void *new = realloc(buf, size);
		if (!new) {
			pr_err("failed to allocate memory to read event\n");
			goto out_err;
		}
		buf = new;
		cur_size = size;
		event = buf;
	}
	p = event;
	p += sizeof(struct perf_event_header);

	if (size - sizeof(struct perf_event_header)) {
		err = readn(self->fd, p, size - sizeof(struct perf_event_header));
		if (err <= 0) {
			if (err == 0) {
				pr_err("unexpected end of event stream\n");
				goto done;
			}

			pr_err("failed to read event data\n");
			goto out_err;
		}
	}

	if ((skip = perf_session__process_event(self, event, tool, head)) < 0) {
		pr_err("%#" PRIx64 " [%#x]: failed to process type: %d\n",
		       head, event->header.size, event->header.type);
		err = -EINVAL;
		goto out_err;
	}

	head += size;

	if (skip > 0)
		head += skip;

	if (!session_done())
		goto more;
done:
	err = 0;
out_err:
	free(buf);
	perf_session__warn_about_errors(self, tool);
	perf_session_free_sample_buffers(self);
	return err;
}

static union perf_event *
fetch_mmaped_event(struct perf_session *session,
		   u64 head, size_t mmap_size, char *buf)
{
	union perf_event *event;

	if (head + sizeof(event->header) > mmap_size)
		return NULL;

	event = (union perf_event *)(buf + head);

	if (session->header.needs_swap)
		perf_event_header__bswap(&event->header);

	if (head + event->header.size > mmap_size) {
		
		if (session->header.needs_swap)
			perf_event_header__bswap(&event->header);
		return NULL;
	}

	return event;
}

#if BITS_PER_LONG == 64
#define MMAP_SIZE ULLONG_MAX
#define NUM_MMAPS 1
#else
#define MMAP_SIZE (32 * 1024 * 1024ULL)
#define NUM_MMAPS 128
#endif

int __perf_session__process_events(struct perf_session *session,
				   u64 data_offset, u64 data_size,
				   u64 file_size, struct perf_tool *tool)
{
	u64 head, page_offset, file_offset, file_pos, progress_next;
	int err, mmap_prot, mmap_flags, map_idx = 0;
	size_t	mmap_size;
	char *buf, *mmaps[NUM_MMAPS];
	union perf_event *event;
	uint32_t size;

	perf_tool__fill_defaults(tool);

	page_offset = page_size * (data_offset / page_size);
	file_offset = page_offset;
	head = data_offset - page_offset;

	if (data_size && (data_offset + data_size < file_size))
		file_size = data_offset + data_size;

	progress_next = file_size / 16;

	mmap_size = MMAP_SIZE;
	if (mmap_size > file_size)
		mmap_size = file_size;

	memset(mmaps, 0, sizeof(mmaps));

	mmap_prot  = PROT_READ;
	mmap_flags = MAP_SHARED;

	if (session->header.needs_swap) {
		mmap_prot  |= PROT_WRITE;
		mmap_flags = MAP_PRIVATE;
	}
remap:
	buf = mmap(NULL, mmap_size, mmap_prot, mmap_flags, session->fd,
		   file_offset);
	if (buf == MAP_FAILED) {
		pr_err("failed to mmap file\n");
		err = -errno;
		goto out_err;
	}
	mmaps[map_idx] = buf;
	map_idx = (map_idx + 1) & (ARRAY_SIZE(mmaps) - 1);
	file_pos = file_offset + head;

more:
	event = fetch_mmaped_event(session, head, mmap_size, buf);
	if (!event) {
		if (mmaps[map_idx]) {
			munmap(mmaps[map_idx], mmap_size);
			mmaps[map_idx] = NULL;
		}

		page_offset = page_size * (head / page_size);
		file_offset += page_offset;
		head -= page_offset;
		goto remap;
	}

	size = event->header.size;

	if (size < sizeof(struct perf_event_header) ||
	    perf_session__process_event(session, event, tool, file_pos) < 0) {
		pr_err("%#" PRIx64 " [%#x]: failed to process type: %d\n",
		       file_offset + head, event->header.size,
		       event->header.type);
		err = -EINVAL;
		goto out_err;
	}

	head += size;
	file_pos += size;

	if (file_pos >= progress_next) {
		progress_next += file_size / 16;
		ui_progress__update(file_pos, file_size,
				    "Processing events...");
	}

	err = 0;
	if (session_done())
		goto out_err;

	if (file_pos < file_size)
		goto more;

	
	session->ordered_samples.next_flush = ULLONG_MAX;
	err = flush_sample_queue(session, tool);
out_err:
	ui_progress__finish();
	perf_session__warn_about_errors(session, tool);
	perf_session_free_sample_buffers(session);
	return err;
}

int perf_session__process_events(struct perf_session *self,
				 struct perf_tool *tool)
{
	int err;

	if (perf_session__register_idle_thread(self) == NULL)
		return -ENOMEM;

	if (!self->fd_pipe)
		err = __perf_session__process_events(self,
						     self->header.data_offset,
						     self->header.data_size,
						     self->size, tool);
	else
		err = __perf_session__process_pipe_events(self, tool);

	return err;
}

bool perf_session__has_traces(struct perf_session *session, const char *msg)
{
	struct perf_evsel *evsel;

	list_for_each_entry(evsel, &session->evlist->entries, node) {
		if (evsel->attr.type == PERF_TYPE_TRACEPOINT)
			return true;
	}

	pr_err("No trace sample to read. Did you call 'perf %s'?\n", msg);
	return false;
}

int maps__set_kallsyms_ref_reloc_sym(struct map **maps,
				     const char *symbol_name, u64 addr)
{
	char *bracket;
	enum map_type i;
	struct ref_reloc_sym *ref;

	ref = zalloc(sizeof(struct ref_reloc_sym));
	if (ref == NULL)
		return -ENOMEM;

	ref->name = strdup(symbol_name);
	if (ref->name == NULL) {
		free(ref);
		return -ENOMEM;
	}

	bracket = strchr(ref->name, ']');
	if (bracket)
		*bracket = '\0';

	ref->addr = addr;

	for (i = 0; i < MAP__NR_TYPES; ++i) {
		struct kmap *kmap = map__kmap(maps[i]);
		kmap->ref_reloc_sym = ref;
	}

	return 0;
}

size_t perf_session__fprintf_dsos(struct perf_session *self, FILE *fp)
{
	return machines__fprintf_dsos(&self->machines, fp);
}

size_t perf_session__fprintf_dsos_buildid(struct perf_session *self, FILE *fp,
					  bool (skip)(struct dso *dso, int parm), int parm)
{
	return machines__fprintf_dsos_buildid(&self->machines, fp, skip, parm);
}

size_t perf_session__fprintf_nr_events(struct perf_session *session, FILE *fp)
{
	struct perf_evsel *pos;
	size_t ret = fprintf(fp, "Aggregated stats:\n");

	ret += events_stats__fprintf(&session->stats, fp);

	list_for_each_entry(pos, &session->evlist->entries, node) {
		ret += fprintf(fp, "%s stats:\n", perf_evsel__name(pos));
		ret += events_stats__fprintf(&pos->hists.stats, fp);
	}

	return ret;
}

size_t perf_session__fprintf(struct perf_session *session, FILE *fp)
{
	return machine__fprintf(&session->machines.host, fp);
}

struct perf_evsel *perf_session__find_first_evtype(struct perf_session *session,
					      unsigned int type)
{
	struct perf_evsel *pos;

	list_for_each_entry(pos, &session->evlist->entries, node) {
		if (pos->attr.type == type)
			return pos;
	}
	return NULL;
}

void perf_evsel__print_ip(struct perf_evsel *evsel, union perf_event *event,
			  struct perf_sample *sample, struct machine *machine,
			  unsigned int print_opts, unsigned int stack_depth)
{
	struct addr_location al;
	struct callchain_cursor_node *node;
	int print_ip = print_opts & PRINT_IP_OPT_IP;
	int print_sym = print_opts & PRINT_IP_OPT_SYM;
	int print_dso = print_opts & PRINT_IP_OPT_DSO;
	int print_symoffset = print_opts & PRINT_IP_OPT_SYMOFFSET;
	int print_oneline = print_opts & PRINT_IP_OPT_ONELINE;
	char s = print_oneline ? ' ' : '\t';

	if (perf_event__preprocess_sample(event, machine, &al, sample) < 0) {
		error("problem processing %d event, skipping it.\n",
			event->header.type);
		return;
	}

	if (symbol_conf.use_callchain && sample->callchain) {

		if (machine__resolve_callchain(machine, evsel, al.thread,
					       sample, NULL, NULL) != 0) {
			if (verbose)
				error("Failed to resolve callchain. Skipping\n");
			return;
		}
		callchain_cursor_commit(&callchain_cursor);

		while (stack_depth) {
			node = callchain_cursor_current(&callchain_cursor);
			if (!node)
				break;

			if (print_ip)
				printf("%c%16" PRIx64, s, node->ip);

			if (print_sym) {
				printf(" ");
				if (print_symoffset) {
					al.addr = node->ip;
					al.map  = node->map;
					symbol__fprintf_symname_offs(node->sym, &al, stdout);
				} else
					symbol__fprintf_symname(node->sym, stdout);
			}

			if (print_dso) {
				printf(" (");
				map__fprintf_dsoname(node->map, stdout);
				printf(")");
			}

			if (!print_oneline)
				printf("\n");

			callchain_cursor_advance(&callchain_cursor);

			stack_depth--;
		}

	} else {
		if (print_ip)
			printf("%16" PRIx64, sample->ip);

		if (print_sym) {
			printf(" ");
			if (print_symoffset)
				symbol__fprintf_symname_offs(al.sym, &al,
							     stdout);
			else
				symbol__fprintf_symname(al.sym, stdout);
		}

		if (print_dso) {
			printf(" (");
			map__fprintf_dsoname(al.map, stdout);
			printf(")");
		}
	}
}

int perf_session__cpu_bitmap(struct perf_session *session,
			     const char *cpu_list, unsigned long *cpu_bitmap)
{
	int i;
	struct cpu_map *map;

	for (i = 0; i < PERF_TYPE_MAX; ++i) {
		struct perf_evsel *evsel;

		evsel = perf_session__find_first_evtype(session, i);
		if (!evsel)
			continue;

		if (!(evsel->attr.sample_type & PERF_SAMPLE_CPU)) {
			pr_err("File does not contain CPU events. "
			       "Remove -c option to proceed.\n");
			return -1;
		}
	}

	map = cpu_map__new(cpu_list);
	if (map == NULL) {
		pr_err("Invalid cpu_list\n");
		return -1;
	}

	for (i = 0; i < map->nr; i++) {
		int cpu = map->map[i];

		if (cpu >= MAX_NR_CPUS) {
			pr_err("Requested CPU %d too large. "
			       "Consider raising MAX_NR_CPUS\n", cpu);
			return -1;
		}

		set_bit(cpu, cpu_bitmap);
	}

	return 0;
}

void perf_session__fprintf_info(struct perf_session *session, FILE *fp,
				bool full)
{
	struct stat st;
	int ret;

	if (session == NULL || fp == NULL)
		return;

	ret = fstat(session->fd, &st);
	if (ret == -1)
		return;

	fprintf(fp, "# ========\n");
	fprintf(fp, "# captured on: %s", ctime(&st.st_ctime));
	perf_header__fprintf_info(session, fp, full);
	fprintf(fp, "# ========\n#\n");
}


int __perf_session__set_tracepoints_handlers(struct perf_session *session,
					     const struct perf_evsel_str_handler *assocs,
					     size_t nr_assocs)
{
	struct perf_evsel *evsel;
	size_t i;
	int err;

	for (i = 0; i < nr_assocs; i++) {
		evsel = perf_evlist__find_tracepoint_by_name(session->evlist, assocs[i].name);
		if (evsel == NULL)
			continue;

		err = -EEXIST;
		if (evsel->handler.func != NULL)
			goto out;
		evsel->handler.func = assocs[i].handler;
	}

	err = 0;
out:
	return err;
}