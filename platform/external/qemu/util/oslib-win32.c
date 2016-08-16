/*
 * os-win32.c
 *
 * Copyright (c) 2003-2008 Fabrice Bellard
 * Copyright (c) 2010 Red Hat, Inc.
 *
 * QEMU library functions for win32 which are shared between QEMU and
 * the QEMU tools.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */
#include <windows.h>
#include <glib.h>
#include <stdlib.h>
#include "config-host.h"
#include "sysemu/sysemu.h"
#include "trace.h"
#include "qemu/sockets.h"

#include <shlobj.h>

void *qemu_oom_check(void *ptr)
{
    if (ptr == NULL) {
        fprintf(stderr, "Failed to allocate memory: %lu\n", GetLastError());
        abort();
    }
    return ptr;
}

void *qemu_memalign(size_t alignment, size_t size)
{
    void *ptr;

    if (!size) {
        abort();
    }
    ptr = qemu_oom_check(VirtualAlloc(NULL, size, MEM_COMMIT, PAGE_READWRITE));
    
    return ptr;
}

void *qemu_vmalloc(size_t size)
{
    void *ptr;

    if (!size) {
        abort();
    }
    ptr = qemu_oom_check(VirtualAlloc(NULL, size, MEM_COMMIT, PAGE_READWRITE));
    
    return ptr;
}

void *qemu_anon_ram_alloc(size_t size)
{
    void *ptr;

    ptr = VirtualAlloc(NULL, size, MEM_COMMIT, PAGE_READWRITE);
    
    return ptr;
}

void qemu_vfree(void *ptr)
{
    
    if (ptr) {
        VirtualFree(ptr, 0, MEM_RELEASE);
    }
}

void qemu_anon_ram_free(void *ptr, size_t size)
{
    
    if (ptr) {
        VirtualFree(ptr, 0, MEM_RELEASE);
    }
}

struct tm *gmtime_r(const time_t *timep, struct tm *result)
{
    struct tm *p = gmtime(timep);
    memset(result, 0, sizeof(*result));
    if (p) {
        *result = *p;
        p = result;
    }
    return p;
}

struct tm *localtime_r(const time_t *timep, struct tm *result)
{
    struct tm *p = localtime(timep);
    memset(result, 0, sizeof(*result));
    if (p) {
        *result = *p;
        p = result;
    }
    return p;
}

void qemu_set_block(int fd)
{
    unsigned long opt = 0;
    WSAEventSelect(fd, NULL, 0);
    ioctlsocket(fd, FIONBIO, &opt);
}

void qemu_set_nonblock(int fd)
{
    unsigned long opt = 1;
    ioctlsocket(fd, FIONBIO, &opt);
#ifndef CONFIG_ANDROID
    qemu_fd_register(fd);
#endif
}

int socket_set_fast_reuse(int fd)
{
    return 0;
}

int inet_aton(const char *cp, struct in_addr *ia)
{
    uint32_t addr = inet_addr(cp);
    if (addr == 0xffffffff) {
	return 0;
    }
    ia->s_addr = addr;
    return 1;
}

void qemu_set_cloexec(int fd)
{
}

#define _W32_FT_OFFSET (116444736000000000ULL)

int qemu_gettimeofday(qemu_timeval *tp)
{
  union {
    unsigned long long ns100; 
    FILETIME ft;
  }  _now;

  if(tp) {
      GetSystemTimeAsFileTime (&_now.ft);
      tp->tv_usec=(long)((_now.ns100 / 10ULL) % 1000000ULL );
      tp->tv_sec= (long)((_now.ns100 - _W32_FT_OFFSET) / 10000000ULL);
  }
  return 0;
}

int qemu_get_thread_id(void)
{
    return GetCurrentThreadId();
}

char *
qemu_get_local_state_pathname(const char *relative_pathname)
{
    HRESULT result;
    char base_path[MAX_PATH+1] = "";

    result = SHGetFolderPath(NULL, CSIDL_COMMON_APPDATA, NULL,
                              0, base_path);
    if (result != S_OK) {
        
        g_critical("CSIDL_COMMON_APPDATA unavailable: %ld", (long)result);
        abort();
    }
    return g_strdup_printf("%s" G_DIR_SEPARATOR_S "%s", base_path,
                           relative_pathname);
}

#ifdef CONFIG_ANDROID
int ffs(int x) {
    int result = 0;
    unsigned ux = (unsigned)x;
    while (ux) {
        if (ux & 1)
            return result + 1;
        ux >>= 1;
    }
    return 0;
}
#endif