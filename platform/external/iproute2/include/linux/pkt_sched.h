#ifndef __LINUX_PKT_SCHED_H
#define __LINUX_PKT_SCHED_H

#include <linux/types.h>


#define TC_PRIO_BESTEFFORT		0
#define TC_PRIO_FILLER			1
#define TC_PRIO_BULK			2
#define TC_PRIO_INTERACTIVE_BULK	4
#define TC_PRIO_INTERACTIVE		6
#define TC_PRIO_CONTROL			7

#define TC_PRIO_MAX			15


struct tc_stats {
	__u64	bytes;			
	__u32	packets;		
	__u32	drops;			
	__u32	overlimits;		
	__u32	bps;			
	__u32	pps;			
	__u32	qlen;
	__u32	backlog;
};

struct tc_estimator {
	signed char	interval;
	unsigned char	ewma_log;
};


#define TC_H_MAJ_MASK (0xFFFF0000U)
#define TC_H_MIN_MASK (0x0000FFFFU)
#define TC_H_MAJ(h) ((h)&TC_H_MAJ_MASK)
#define TC_H_MIN(h) ((h)&TC_H_MIN_MASK)
#define TC_H_MAKE(maj,min) (((maj)&TC_H_MAJ_MASK)|((min)&TC_H_MIN_MASK))

#define TC_H_UNSPEC	(0U)
#define TC_H_ROOT	(0xFFFFFFFFU)
#define TC_H_INGRESS    (0xFFFFFFF1U)

struct tc_ratespec {
	unsigned char	cell_log;
	unsigned char	__reserved;
	unsigned short	overhead;
	short		cell_align;
	unsigned short	mpu;
	__u32		rate;
};

#define TC_RTAB_SIZE	1024

struct tc_sizespec {
	unsigned char	cell_log;
	unsigned char	size_log;
	short		cell_align;
	int		overhead;
	unsigned int	linklayer;
	unsigned int	mpu;
	unsigned int	mtu;
	unsigned int	tsize;
};

enum {
	TCA_STAB_UNSPEC,
	TCA_STAB_BASE,
	TCA_STAB_DATA,
	__TCA_STAB_MAX
};

#define TCA_STAB_MAX (__TCA_STAB_MAX - 1)


struct tc_fifo_qopt {
	__u32	limit;	
};


#define TCQ_PRIO_BANDS	16
#define TCQ_MIN_PRIO_BANDS 2

struct tc_prio_qopt {
	int	bands;			
	__u8	priomap[TC_PRIO_MAX+1];	
};


struct tc_multiq_qopt {
	__u16	bands;			
	__u16	max_bands;		
};


#define TCQ_PLUG_BUFFER                0
#define TCQ_PLUG_RELEASE_ONE           1
#define TCQ_PLUG_RELEASE_INDEFINITE    2
#define TCQ_PLUG_LIMIT                 3

struct tc_plug_qopt {
	int             action;
	__u32           limit;
};


struct tc_tbf_qopt {
	struct tc_ratespec rate;
	struct tc_ratespec peakrate;
	__u32		limit;
	__u32		buffer;
	__u32		mtu;
};

enum {
	TCA_TBF_UNSPEC,
	TCA_TBF_PARMS,
	TCA_TBF_RTAB,
	TCA_TBF_PTAB,
	__TCA_TBF_MAX,
};

#define TCA_TBF_MAX (__TCA_TBF_MAX - 1)





struct tc_sfq_qopt {
	unsigned	quantum;	
	int		perturb_period;	
	__u32		limit;		
	unsigned	divisor;	
	unsigned	flows;		
};

struct tc_sfqred_stats {
	__u32           prob_drop;      
	__u32           forced_drop;	
	__u32           prob_mark;      
	__u32           forced_mark;    
	__u32           prob_mark_head; 
	__u32           forced_mark_head;
};

struct tc_sfq_qopt_v1 {
	struct tc_sfq_qopt v0;
	unsigned int	depth;		
	unsigned int	headdrop;
	__u32		limit;		
	__u32		qth_min;	
	__u32		qth_max;	
	unsigned char   Wlog;		
	unsigned char   Plog;		
	unsigned char   Scell_log;	
	unsigned char	flags;
	__u32		max_P;		
	struct tc_sfqred_stats stats;
};


struct tc_sfq_xstats {
	__s32		allot;
};


enum {
	TCA_RED_UNSPEC,
	TCA_RED_PARMS,
	TCA_RED_STAB,
	TCA_RED_MAX_P,
	__TCA_RED_MAX,
};

#define TCA_RED_MAX (__TCA_RED_MAX - 1)

struct tc_red_qopt {
	__u32		limit;		
	__u32		qth_min;	
	__u32		qth_max;	
	unsigned char   Wlog;		
	unsigned char   Plog;		
	unsigned char   Scell_log;	
	unsigned char	flags;
#define TC_RED_ECN		1
#define TC_RED_HARDDROP		2
#define TC_RED_ADAPTATIVE	4
};

struct tc_red_xstats {
	__u32           early;          
	__u32           pdrop;          
	__u32           other;          
	__u32           marked;         
};


#define MAX_DPs 16

enum {
       TCA_GRED_UNSPEC,
       TCA_GRED_PARMS,
       TCA_GRED_STAB,
       TCA_GRED_DPS,
       TCA_GRED_MAX_P,
	   __TCA_GRED_MAX,
};

#define TCA_GRED_MAX (__TCA_GRED_MAX - 1)

struct tc_gred_qopt {
	__u32		limit;        
	__u32		qth_min;      
	__u32		qth_max;      
	__u32		DP;           
	__u32		backlog;
	__u32		qave;
	__u32		forced;
	__u32		early;
	__u32		other;
	__u32		pdrop;
	__u8		Wlog;         
	__u8		Plog;         
	__u8		Scell_log;    
	__u8		prio;         
	__u32		packets;
	__u32		bytesin;
};

struct tc_gred_sopt {
	__u32		DPs;
	__u32		def_DP;
	__u8		grio;
	__u8		flags;
	__u16		pad1;
};


enum {
	TCA_CHOKE_UNSPEC,
	TCA_CHOKE_PARMS,
	TCA_CHOKE_STAB,
	TCA_CHOKE_MAX_P,
	__TCA_CHOKE_MAX,
};

#define TCA_CHOKE_MAX (__TCA_CHOKE_MAX - 1)

struct tc_choke_qopt {
	__u32		limit;		
	__u32		qth_min;	
	__u32		qth_max;	
	unsigned char   Wlog;		
	unsigned char   Plog;		
	unsigned char   Scell_log;	
	unsigned char	flags;		
};

struct tc_choke_xstats {
	__u32		early;          
	__u32		pdrop;          
	__u32		other;          
	__u32		marked;         
	__u32		matched;	
};

#define TC_HTB_NUMPRIO		8
#define TC_HTB_MAXDEPTH		8
#define TC_HTB_PROTOVER		3 

struct tc_htb_opt {
	struct tc_ratespec 	rate;
	struct tc_ratespec 	ceil;
	__u32	buffer;
	__u32	cbuffer;
	__u32	quantum;
	__u32	level;		
	__u32	prio;
};
struct tc_htb_glob {
	__u32 version;		
    	__u32 rate2quantum;	
    	__u32 defcls;		
	__u32 debug;		

	
	__u32 direct_pkts; 
};
enum {
	TCA_HTB_UNSPEC,
	TCA_HTB_PARMS,
	TCA_HTB_INIT,
	TCA_HTB_CTAB,
	TCA_HTB_RTAB,
	__TCA_HTB_MAX,
};

#define TCA_HTB_MAX (__TCA_HTB_MAX - 1)

struct tc_htb_xstats {
	__u32 lends;
	__u32 borrows;
	__u32 giants;	
	__u32 tokens;
	__u32 ctokens;
};


struct tc_hfsc_qopt {
	__u16	defcls;		
};

struct tc_service_curve {
	__u32	m1;		
	__u32	d;		
	__u32	m2;		
};

struct tc_hfsc_stats {
	__u64	work;		
	__u64	rtwork;		
	__u32	period;		
	__u32	level;		
};

enum {
	TCA_HFSC_UNSPEC,
	TCA_HFSC_RSC,
	TCA_HFSC_FSC,
	TCA_HFSC_USC,
	__TCA_HFSC_MAX,
};

#define TCA_HFSC_MAX (__TCA_HFSC_MAX - 1)



#define TC_CBQ_MAXPRIO		8
#define TC_CBQ_MAXLEVEL		8
#define TC_CBQ_DEF_EWMA		5

struct tc_cbq_lssopt {
	unsigned char	change;
	unsigned char	flags;
#define TCF_CBQ_LSS_BOUNDED	1
#define TCF_CBQ_LSS_ISOLATED	2
	unsigned char  	ewma_log;
	unsigned char  	level;
#define TCF_CBQ_LSS_FLAGS	1
#define TCF_CBQ_LSS_EWMA	2
#define TCF_CBQ_LSS_MAXIDLE	4
#define TCF_CBQ_LSS_MINIDLE	8
#define TCF_CBQ_LSS_OFFTIME	0x10
#define TCF_CBQ_LSS_AVPKT	0x20
	__u32		maxidle;
	__u32		minidle;
	__u32		offtime;
	__u32		avpkt;
};

struct tc_cbq_wrropt {
	unsigned char	flags;
	unsigned char	priority;
	unsigned char	cpriority;
	unsigned char	__reserved;
	__u32		allot;
	__u32		weight;
};

struct tc_cbq_ovl {
	unsigned char	strategy;
#define	TC_CBQ_OVL_CLASSIC	0
#define	TC_CBQ_OVL_DELAY	1
#define	TC_CBQ_OVL_LOWPRIO	2
#define	TC_CBQ_OVL_DROP		3
#define	TC_CBQ_OVL_RCLASSIC	4
	unsigned char	priority2;
	__u16		pad;
	__u32		penalty;
};

struct tc_cbq_police {
	unsigned char	police;
	unsigned char	__res1;
	unsigned short	__res2;
};

struct tc_cbq_fopt {
	__u32		split;
	__u32		defmap;
	__u32		defchange;
};

struct tc_cbq_xstats {
	__u32		borrows;
	__u32		overactions;
	__s32		avgidle;
	__s32		undertime;
};

enum {
	TCA_CBQ_UNSPEC,
	TCA_CBQ_LSSOPT,
	TCA_CBQ_WRROPT,
	TCA_CBQ_FOPT,
	TCA_CBQ_OVL_STRATEGY,
	TCA_CBQ_RATE,
	TCA_CBQ_RTAB,
	TCA_CBQ_POLICE,
	__TCA_CBQ_MAX,
};

#define TCA_CBQ_MAX	(__TCA_CBQ_MAX - 1)


enum {
	TCA_DSMARK_UNSPEC,
	TCA_DSMARK_INDICES,
	TCA_DSMARK_DEFAULT_INDEX,
	TCA_DSMARK_SET_TC_INDEX,
	TCA_DSMARK_MASK,
	TCA_DSMARK_VALUE,
	__TCA_DSMARK_MAX,
};

#define TCA_DSMARK_MAX (__TCA_DSMARK_MAX - 1)


enum {
	TCA_ATM_UNSPEC,
	TCA_ATM_FD,		
	TCA_ATM_PTR,		
	TCA_ATM_HDR,		
	TCA_ATM_EXCESS,		
	TCA_ATM_ADDR,		
	TCA_ATM_STATE,		
	__TCA_ATM_MAX,
};

#define TCA_ATM_MAX	(__TCA_ATM_MAX - 1)


enum {
	TCA_NETEM_UNSPEC,
	TCA_NETEM_CORR,
	TCA_NETEM_DELAY_DIST,
	TCA_NETEM_REORDER,
	TCA_NETEM_CORRUPT,
	TCA_NETEM_LOSS,
	TCA_NETEM_RATE,
	__TCA_NETEM_MAX,
};

#define TCA_NETEM_MAX (__TCA_NETEM_MAX - 1)

struct tc_netem_qopt {
	__u32	latency;	
	__u32   limit;		
	__u32	loss;		
	__u32	gap;		
	__u32   duplicate;	
	__u32	jitter;		
};

struct tc_netem_corr {
	__u32	delay_corr;	
	__u32	loss_corr;	
	__u32	dup_corr;	
};

struct tc_netem_reorder {
	__u32	probability;
	__u32	correlation;
};

struct tc_netem_corrupt {
	__u32	probability;
	__u32	correlation;
};

struct tc_netem_rate {
	__u32	rate;	
	__s32	packet_overhead;
	__u32	cell_size;
	__s32	cell_overhead;
};

enum {
	NETEM_LOSS_UNSPEC,
	NETEM_LOSS_GI,		
	NETEM_LOSS_GE,		
	__NETEM_LOSS_MAX
};
#define NETEM_LOSS_MAX (__NETEM_LOSS_MAX - 1)

struct tc_netem_gimodel {
	__u32	p13;
	__u32	p31;
	__u32	p32;
	__u32	p14;
	__u32	p23;
};

struct tc_netem_gemodel {
	__u32 p;
	__u32 r;
	__u32 h;
	__u32 k1;
};

#define NETEM_DIST_SCALE	8192
#define NETEM_DIST_MAX		16384


enum {
	TCA_DRR_UNSPEC,
	TCA_DRR_QUANTUM,
	__TCA_DRR_MAX
};

#define TCA_DRR_MAX	(__TCA_DRR_MAX - 1)

struct tc_drr_stats {
	__u32	deficit;
};

#define TC_QOPT_BITMASK 15
#define TC_QOPT_MAX_QUEUE 16

struct tc_mqprio_qopt {
	__u8	num_tc;
	__u8	prio_tc_map[TC_QOPT_BITMASK + 1];
	__u8	hw;
	__u16	count[TC_QOPT_MAX_QUEUE];
	__u16	offset[TC_QOPT_MAX_QUEUE];
};


enum {
	TCA_SFB_UNSPEC,
	TCA_SFB_PARMS,
	__TCA_SFB_MAX,
};

#define TCA_SFB_MAX (__TCA_SFB_MAX - 1)

struct tc_sfb_qopt {
	__u32 rehash_interval;	
	__u32 warmup_time;	
	__u32 max;		
	__u32 bin_size;		
	__u32 increment;	
	__u32 decrement;	
	__u32 limit;		
	__u32 penalty_rate;	
	__u32 penalty_burst;
};

struct tc_sfb_xstats {
	__u32 earlydrop;
	__u32 penaltydrop;
	__u32 bucketdrop;
	__u32 queuedrop;
	__u32 childdrop; 
	__u32 marked;
	__u32 maxqlen;
	__u32 maxprob;
	__u32 avgprob;
};

#define SFB_MAX_PROB 0xFFFF

enum {
	TCA_QFQ_UNSPEC,
	TCA_QFQ_WEIGHT,
	TCA_QFQ_LMAX,
	__TCA_QFQ_MAX
};

#define TCA_QFQ_MAX	(__TCA_QFQ_MAX - 1)

struct tc_qfq_stats {
	__u32 weight;
	__u32 lmax;
};

#endif
