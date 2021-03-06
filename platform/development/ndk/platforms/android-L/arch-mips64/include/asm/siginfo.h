/****************************************************************************
 ****************************************************************************
 ***
 ***   This header was automatically generated from a Linux kernel header
 ***   of the same name, to make information necessary for userspace to
 ***   call into the kernel available to libc.  It contains only constants,
 ***   structures, and macros generated from the original header, and thus,
 ***   contains no copyrightable information.
 ***
 ***   To edit the content of this header, modify the corresponding
 ***   source file (e.g. under external/kernel-headers/original/) then
 ***   run bionic/libc/kernel/tools/update_all.py
 ***
 ***   Any manual change here will be lost the next time this script will
 ***   be run. You've been warned!
 ***
 ****************************************************************************
 ****************************************************************************/
#ifndef _UAPI_ASM_SIGINFO_H
#define _UAPI_ASM_SIGINFO_H
#define __ARCH_SIGEV_PREAMBLE_SIZE (sizeof(long) + 2*sizeof(int))
#undef __ARCH_SI_TRAPNO
#define HAVE_ARCH_SIGINFO_T
#define HAVE_ARCH_COPY_SIGINFO
struct siginfo;
#ifdef _ABIO32
#define __ARCH_SI_PREAMBLE_SIZE (3 * sizeof(int))
#endif
#ifdef __LP64__
#undef __ARCH_SI_PREAMBLE_SIZE
#define __ARCH_SI_PREAMBLE_SIZE (4 * sizeof(int))
#endif
#include <asm-generic/siginfo.h>
typedef struct siginfo {
 int si_signo;
 int si_code;
 int si_errno;
 int __pad0[SI_MAX_SIZE / sizeof(int) - SI_PAD_SIZE - 3];
 union {
 int _pad[SI_PAD_SIZE];
 struct {
 pid_t _pid;
 __ARCH_SI_UID_T _uid;
 } _kill;
 struct {
 timer_t _tid;
 int _overrun;
 char _pad[sizeof( __ARCH_SI_UID_T) - sizeof(int)];
 sigval_t _sigval;
 int _sys_private;
 } _timer;
 struct {
 pid_t _pid;
 __ARCH_SI_UID_T _uid;
 sigval_t _sigval;
 } _rt;
 struct {
 pid_t _pid;
 __ARCH_SI_UID_T _uid;
 int _status;
 clock_t _utime;
 clock_t _stime;
 } _sigchld;
 struct {
 pid_t _pid;
 clock_t _utime;
 int _status;
 clock_t _stime;
 } _irix_sigchld;
 struct {
 void __user *_addr;
#ifdef __ARCH_SI_TRAPNO
 int _trapno;
#endif
 short _addr_lsb;
 } _sigfault;
 struct {
 __ARCH_SI_BAND_T _band;
 int _fd;
 } _sigpoll;
 } _sifields;
} siginfo_t;
#undef SI_ASYNCIO
#undef SI_TIMER
#undef SI_MESGQ
#define SI_ASYNCIO -2
#define SI_TIMER __SI_CODE(__SI_TIMER, -3)
#define SI_MESGQ __SI_CODE(__SI_MESGQ, -4)
#endif
