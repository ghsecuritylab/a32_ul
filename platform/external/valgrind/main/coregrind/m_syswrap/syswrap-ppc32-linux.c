

/*
   This file is part of Valgrind, a dynamic binary instrumentation
   framework.

   Copyright (C) 2005-2013 Nicholas Nethercote <njn@valgrind.org>
   Copyright (C) 2005-2013 Cerion Armour-Brown <cerion@open-works.co.uk>

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public License as
   published by the Free Software Foundation; either version 2 of the
   License, or (at your option) any later version.

   This program is distributed in the hope that it will be useful, but
   WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
   02111-1307, USA.

   The GNU General Public License is contained in the file COPYING.
*/

#if defined(VGP_ppc32_linux)

#include "pub_core_basics.h"
#include "pub_core_vki.h"
#include "pub_core_vkiscnums.h"
#include "pub_core_libcsetjmp.h"    
#include "pub_core_threadstate.h"
#include "pub_core_aspacemgr.h"
#include "pub_core_debuglog.h"
#include "pub_core_libcbase.h"
#include "pub_core_libcassert.h"
#include "pub_core_libcprint.h"
#include "pub_core_libcproc.h"
#include "pub_core_libcsignal.h"
#include "pub_core_options.h"
#include "pub_core_scheduler.h"
#include "pub_core_sigframe.h"      
#include "pub_core_signals.h"
#include "pub_core_syscall.h"
#include "pub_core_syswrap.h"
#include "pub_core_tooliface.h"
#include "pub_core_stacks.h"        

#include "priv_types_n_macros.h"
#include "priv_syswrap-generic.h"   
#include "priv_syswrap-linux.h"     
#include "priv_syswrap-main.h"



__attribute__((noreturn))
void ML_(call_on_new_stack_0_1) ( Addr stack,
                                  Addr retaddr,
                                  void (*f)(Word),
                                  Word arg1 );
asm(
".text\n"
".globl vgModuleLocal_call_on_new_stack_0_1\n"
"vgModuleLocal_call_on_new_stack_0_1:\n"
"   mr    %r1,%r3\n\t"     
"   mtlr  %r4\n\t"         
"   mtctr %r5\n\t"         
"   mr %r3,%r6\n\t"        
"   li 0,0\n\t"            
"   li 4,0\n\t"
"   li 5,0\n\t"
"   li 6,0\n\t"
"   li 7,0\n\t"
"   li 8,0\n\t"
"   li 9,0\n\t"
"   li 10,0\n\t"
"   li 11,0\n\t"
"   li 12,0\n\t"
"   li 13,0\n\t"
"   li 14,0\n\t"
"   li 15,0\n\t"
"   li 16,0\n\t"
"   li 17,0\n\t"
"   li 18,0\n\t"
"   li 19,0\n\t"
"   li 20,0\n\t"
"   li 21,0\n\t"
"   li 22,0\n\t"
"   li 23,0\n\t"
"   li 24,0\n\t"
"   li 25,0\n\t"
"   li 26,0\n\t"
"   li 27,0\n\t"
"   li 28,0\n\t"
"   li 29,0\n\t"
"   li 30,0\n\t"
"   li 31,0\n\t"
"   mtxer 0\n\t"           
"   mtcr 0\n\t"            
"   bctr\n\t"              
"   trap\n"                
".previous\n"
);


#define __NR_CLONE        VG_STRINGIFY(__NR_clone)
#define __NR_EXIT         VG_STRINGIFY(__NR_exit)

extern
ULong do_syscall_clone_ppc32_linux ( Word (*fn)(void *), 
                                     void* stack, 
                                     Int   flags, 
                                     void* arg,
                                     Int*  child_tid, 
                                     Int*  parent_tid, 
                                     vki_modify_ldt_t * );
asm(
".text\n"
".globl do_syscall_clone_ppc32_linux\n"
"do_syscall_clone_ppc32_linux:\n"
"       stwu    1,-32(1)\n"
"       stw     29,20(1)\n"
"       stw     30,24(1)\n"
"       stw     31,28(1)\n"
"       mr      30,3\n"              
"       mr      31,6\n"              

        
"       rlwinm  4,4,0,~0xf\n"        
"       li      0,0\n"
"       stwu    0,-16(4)\n"          
"       mr      29,4\n"              

        
"       li      0,"__NR_CLONE"\n"    
"       mr      3,5\n"               
        
"       mr      5,8\n"               
"       mr      6,2\n"               
"       mr      7,7\n"               
"       mr      8,8\n"               
"       mr      9,9\n"               

"       sc\n"                        

"       mfcr    4\n"                 
"       cmpwi   3,0\n"               
"       bne     1f\n"                

        
"       mr      1,29\n"
"       mtctr   30\n"                
"       mr      3,31\n"              
"       bctrl\n"                     

        
"       li      0,"__NR_EXIT"\n"
"       sc\n"

        
"       .long   0\n"

        
"1:     lwz     29,20(1)\n"
"       lwz     30,24(1)\n"
"       lwz     31,28(1)\n"
"       addi    1,1,32\n"
"       blr\n"
".previous\n"
);

#undef __NR_CLONE
#undef __NR_EXIT

static void setup_child ( ThreadArchState*, ThreadArchState* );

static SysRes do_clone ( ThreadId ptid, 
                         UInt flags, Addr sp, 
                         Int *parent_tidptr, 
                         Int *child_tidptr, 
                         Addr child_tls)
{
   const Bool debug = False;

   ThreadId     ctid = VG_(alloc_ThreadState)();
   ThreadState* ptst = VG_(get_ThreadState)(ptid);
   ThreadState* ctst = VG_(get_ThreadState)(ctid);
   ULong        word64;
   UWord*       stack;
   NSegment const* seg;
   SysRes       res;
   vki_sigset_t blockall, savedmask;

   VG_(sigfillset)(&blockall);

   vg_assert(VG_(is_running_thread)(ptid));
   vg_assert(VG_(is_valid_tid)(ctid));

   stack = (UWord*)ML_(allocstack)(ctid);
   if (stack == NULL) {
      res = VG_(mk_SysRes_Error)( VKI_ENOMEM );
      goto out;
   }



   setup_child( &ctst->arch, &ptst->arch );

   { UInt old_cr = LibVEX_GuestPPC32_get_CR( &ctst->arch.vex );
     
     ctst->arch.vex.guest_GPR3 = 0;
     
     LibVEX_GuestPPC32_put_CR( old_cr & ~(1<<28), &ctst->arch.vex );
   }

   if (sp != 0)
      ctst->arch.vex.guest_GPR1 = sp;

   ctst->os_state.parent = ptid;

   
   ctst->sig_mask = ptst->sig_mask;
   ctst->tmp_sig_mask = ptst->sig_mask;

   ctst->os_state.threadgroup = ptst->os_state.threadgroup;

   seg = VG_(am_find_nsegment)(sp);
   if (seg && seg->kind != SkResvn) {
      ctst->client_stack_highest_word = (Addr)VG_PGROUNDUP(sp);
      ctst->client_stack_szB = ctst->client_stack_highest_word - seg->start;

      VG_(register_stack)(seg->start, ctst->client_stack_highest_word);

      if (debug)
	 VG_(printf)("\ntid %d: guessed client stack range %#lx-%#lx\n",
		     ctid, seg->start, VG_PGROUNDUP(sp));
   } else {
      VG_(message)(Vg_UserMsg,
                   "!? New thread %d starts with R1(%#lx) unmapped\n",
		   ctid, sp);
      ctst->client_stack_szB  = 0;
   }

   vg_assert(VG_(owns_BigLock_LL)(ptid));
   VG_TRACK ( pre_thread_ll_create, ptid, ctid );

   if (flags & VKI_CLONE_SETTLS) {
      if (debug)
         VG_(printf)("clone child has SETTLS: tls at %#lx\n", child_tls);
      ctst->arch.vex.guest_GPR2 = child_tls;
   }

   flags &= ~VKI_CLONE_SETTLS;

   
   VG_(sigprocmask)(VKI_SIG_SETMASK, &blockall, &savedmask);

   
   word64 = do_syscall_clone_ppc32_linux(
               ML_(start_thread_NORETURN), stack, flags, &VG_(threads)[ctid],
               child_tidptr, parent_tidptr, NULL
            );
   
   res = VG_(mk_SysRes_ppc32_linux)( 
            (UInt)(word64 >> 32), 
             (((UInt)word64) >> 28) & 1 
         );

   VG_(sigprocmask)(VKI_SIG_SETMASK, &savedmask, NULL);

  out:
   if (sr_isError(res)) {
      
      VG_(cleanup_thread)(&ctst->arch);
      ctst->status = VgTs_Empty;
      
      VG_TRACK( pre_thread_ll_exit, ctid );
   }

   return res;
}




void VG_(cleanup_thread) ( ThreadArchState* arch )
{
}  

void setup_child (  ThreadArchState *child,
                     ThreadArchState *parent )
{
   
   child->vex = parent->vex;
   child->vex_shadow1 = parent->vex_shadow1;
   child->vex_shadow2 = parent->vex_shadow2;
}



#define PRE(name)       DEFN_PRE_TEMPLATE(ppc32_linux, name)
#define POST(name)      DEFN_POST_TEMPLATE(ppc32_linux, name)


DECL_TEMPLATE(ppc32_linux, sys_mmap);
DECL_TEMPLATE(ppc32_linux, sys_mmap2);
DECL_TEMPLATE(ppc32_linux, sys_stat64);
DECL_TEMPLATE(ppc32_linux, sys_lstat64);
DECL_TEMPLATE(ppc32_linux, sys_fstatat64);
DECL_TEMPLATE(ppc32_linux, sys_fstat64);
DECL_TEMPLATE(ppc32_linux, sys_clone);
DECL_TEMPLATE(ppc32_linux, sys_sigreturn);
DECL_TEMPLATE(ppc32_linux, sys_rt_sigreturn);
DECL_TEMPLATE(ppc32_linux, sys_sigsuspend);
DECL_TEMPLATE(ppc32_linux, sys_spu_create);
DECL_TEMPLATE(ppc32_linux, sys_spu_run);

PRE(sys_mmap)
{
   SysRes r;

   PRINT("sys_mmap ( %#lx, %llu, %ld, %ld, %ld, %ld )",
         ARG1, (ULong)ARG2, ARG3, ARG4, ARG5, ARG6 );
   PRE_REG_READ6(long, "mmap",
                 unsigned long, start, unsigned long, length,
                 unsigned long, prot,  unsigned long, flags,
                 unsigned long, fd,    unsigned long, offset);

   r = ML_(generic_PRE_sys_mmap)( tid, ARG1, ARG2, ARG3, ARG4, ARG5, 
                                       (Off64T)ARG6 );
   SET_STATUS_from_SysRes(r);
}

PRE(sys_mmap2)
{
   SysRes r;

   
   
   
   PRINT("sys_mmap2 ( %#lx, %llu, %ld, %ld, %ld, %ld )",
         ARG1, (ULong)ARG2, ARG3, ARG4, ARG5, ARG6 );
   PRE_REG_READ6(long, "mmap2",
                 unsigned long, start, unsigned long, length,
                 unsigned long, prot,  unsigned long, flags,
                 unsigned long, fd,    unsigned long, offset);

   r = ML_(generic_PRE_sys_mmap)( tid, ARG1, ARG2, ARG3, ARG4, ARG5, 
                                       4096 * (Off64T)ARG6 );
   SET_STATUS_from_SysRes(r);
}

PRE(sys_stat64)
{
   PRINT("sys_stat64 ( %#lx, %#lx )",ARG1,ARG2);
   PRE_REG_READ2(long, "stat64", char *, file_name, struct stat64 *, buf);
   PRE_MEM_RASCIIZ( "stat64(file_name)", ARG1 );
   PRE_MEM_WRITE( "stat64(buf)", ARG2, sizeof(struct vki_stat64) );
}

POST(sys_stat64)
{
   POST_MEM_WRITE( ARG2, sizeof(struct vki_stat64) );
}

PRE(sys_lstat64)
{
   PRINT("sys_lstat64 ( %#lx(%s), %#lx )",ARG1,(char*)ARG1,ARG2);
   PRE_REG_READ2(long, "lstat64", char *, file_name, struct stat64 *, buf);
   PRE_MEM_RASCIIZ( "lstat64(file_name)", ARG1 );
   PRE_MEM_WRITE( "lstat64(buf)", ARG2, sizeof(struct vki_stat64) );
}

POST(sys_lstat64)
{
   vg_assert(SUCCESS);
   if (RES == 0) {
      POST_MEM_WRITE( ARG2, sizeof(struct vki_stat64) );
   }
}

PRE(sys_fstatat64)
{
  PRINT("sys_fstatat64 ( %ld, %#lx(%s), %#lx )",ARG1,ARG2,(char*)ARG2,ARG3);
   PRE_REG_READ3(long, "fstatat64",
                 int, dfd, char *, file_name, struct stat64 *, buf);
   PRE_MEM_RASCIIZ( "fstatat64(file_name)", ARG2 );
   PRE_MEM_WRITE( "fstatat64(buf)", ARG3, sizeof(struct vki_stat64) );
}

POST(sys_fstatat64)
{
   POST_MEM_WRITE( ARG3, sizeof(struct vki_stat64) );
}

PRE(sys_fstat64)
{
  PRINT("sys_fstat64 ( %ld, %#lx )",ARG1,ARG2);
  PRE_REG_READ2(long, "fstat64", unsigned long, fd, struct stat64 *, buf);
  PRE_MEM_WRITE( "fstat64(buf)", ARG2, sizeof(struct vki_stat64) );
}

POST(sys_fstat64)
{
  POST_MEM_WRITE( ARG2, sizeof(struct vki_stat64) );
}




PRE(sys_clone)
{
   UInt cloneflags;

   PRINT("sys_clone ( %lx, %#lx, %#lx, %#lx, %#lx )",ARG1,ARG2,ARG3,ARG4,ARG5);
   PRE_REG_READ5(int, "clone",
                 unsigned long, flags,
                 void *,        child_stack,
                 int *,         parent_tidptr,
                 void *,        child_tls,
                 int *,         child_tidptr);

   if (ARG1 & VKI_CLONE_PARENT_SETTID) {
      PRE_MEM_WRITE("clone(parent_tidptr)", ARG3, sizeof(Int));
      if (!VG_(am_is_valid_for_client)(ARG3, sizeof(Int), 
                                             VKI_PROT_WRITE)) {
         SET_STATUS_Failure( VKI_EFAULT );
         return;
      }
   }
   if (ARG1 & (VKI_CLONE_CHILD_SETTID | VKI_CLONE_CHILD_CLEARTID)) {
      PRE_MEM_WRITE("clone(child_tidptr)", ARG5, sizeof(Int));
      if (!VG_(am_is_valid_for_client)(ARG5, sizeof(Int), 
                                             VKI_PROT_WRITE)) {
         SET_STATUS_Failure( VKI_EFAULT );
         return;
      }
   }

   cloneflags = ARG1;

   if (!ML_(client_signal_OK)(ARG1 & VKI_CSIGNAL)) {
      SET_STATUS_Failure( VKI_EINVAL );
      return;
   }

   
   switch (cloneflags & (VKI_CLONE_VM | VKI_CLONE_FS 
                         | VKI_CLONE_FILES | VKI_CLONE_VFORK)) {
   case VKI_CLONE_VM | VKI_CLONE_FS | VKI_CLONE_FILES:
      
      SET_STATUS_from_SysRes(
         do_clone(tid,
                  ARG1,         
                  (Addr)ARG2,   
                  (Int *)ARG3,  
                  (Int *)ARG5,  
                  (Addr)ARG4)); 
      break;

   case VKI_CLONE_VFORK | VKI_CLONE_VM: 
      
      cloneflags &= ~(VKI_CLONE_VFORK | VKI_CLONE_VM);

   case 0: 
      SET_STATUS_from_SysRes(
         ML_(do_fork_clone)(tid,
                       cloneflags,      
                       (Int *)ARG3,     
                       (Int *)ARG5));   
      break;

   default:
      
      VG_(message)(Vg_UserMsg, "Unsupported clone() flags: 0x%lx\n", ARG1);
      VG_(message)(Vg_UserMsg, "\n");
      VG_(message)(Vg_UserMsg, "The only supported clone() uses are:\n");
      VG_(message)(Vg_UserMsg, " - via a threads library (LinuxThreads or NPTL)\n");
      VG_(message)(Vg_UserMsg, " - via the implementation of fork or vfork\n");
      VG_(unimplemented)
         ("Valgrind does not support general clone().");
   }

   if (SUCCESS) {
      if (ARG1 & VKI_CLONE_PARENT_SETTID)
         POST_MEM_WRITE(ARG3, sizeof(Int));
      if (ARG1 & (VKI_CLONE_CHILD_SETTID | VKI_CLONE_CHILD_CLEARTID))
         POST_MEM_WRITE(ARG5, sizeof(Int));

      *flags |= SfYieldAfter;
   }
}

PRE(sys_sigreturn)
{

   
   PRINT("sys_sigreturn ( )");

   vg_assert(VG_(is_valid_tid)(tid));
   vg_assert(tid >= 1 && tid < VG_N_THREADS);
   vg_assert(VG_(is_running_thread)(tid));

   
   
   
   
   

   
   
   
   

   
   VG_(sigframe_destroy)(tid, False);

   *flags |= SfNoWriteResult;
   SET_STATUS_Success(0);

   
   *flags |= SfPollAfter;
}

PRE(sys_rt_sigreturn)
{

   
   PRINT("rt_sigreturn ( )");

   vg_assert(VG_(is_valid_tid)(tid));
   vg_assert(tid >= 1 && tid < VG_N_THREADS);
   vg_assert(VG_(is_running_thread)(tid));

   
   
   
   
   

   
   
   
   

   
   VG_(sigframe_destroy)(tid, True);

   *flags |= SfNoWriteResult;
   SET_STATUS_Success(0);

   
   *flags |= SfPollAfter;
}







PRE(sys_sigsuspend)
{
   *flags |= SfMayBlock;
   PRINT("sys_sigsuspend ( %ld )", ARG1 );
   PRE_REG_READ1(int, "sigsuspend", vki_old_sigset_t, mask);
}

PRE(sys_spu_create)
{
   PRE_MEM_RASCIIZ("stat64(filename)", ARG1);
}
POST(sys_spu_create)
{
   vg_assert(SUCCESS);
}

PRE(sys_spu_run)
{
   *flags |= SfMayBlock;
   if (ARG2 != 0)
      PRE_MEM_WRITE("npc", ARG2, sizeof(unsigned int));
   PRE_MEM_READ("event", ARG3, sizeof(unsigned int));
}
POST(sys_spu_run)
{
   if (ARG2 != 0)
      POST_MEM_WRITE(ARG2, sizeof(unsigned int));
}

#undef PRE
#undef POST


#define PLAX_(sysno, name)    WRAPPER_ENTRY_X_(ppc32_linux, sysno, name) 
#define PLAXY(sysno, name)    WRAPPER_ENTRY_XY(ppc32_linux, sysno, name)


static SyscallTableEntry syscall_table[] = {
   GENX_(__NR_exit,              sys_exit),              
   GENX_(__NR_fork,              sys_fork),              
   GENXY(__NR_read,              sys_read),              
   GENX_(__NR_write,             sys_write),             

   GENXY(__NR_open,              sys_open),              
   GENXY(__NR_close,             sys_close),             
   GENXY(__NR_waitpid,           sys_waitpid),           
   GENXY(__NR_creat,             sys_creat),             
   GENX_(__NR_link,              sys_link),              

   GENX_(__NR_unlink,            sys_unlink),            
   GENX_(__NR_execve,            sys_execve),            
   GENX_(__NR_chdir,             sys_chdir),             
   GENXY(__NR_time,              sys_time),              
   GENX_(__NR_mknod,             sys_mknod),             
   GENX_(__NR_chmod,             sys_chmod),             
   GENX_(__NR_lchown,            sys_lchown),          
   LINX_(__NR_lseek,             sys_lseek),             
   GENX_(__NR_getpid,            sys_getpid),            
   LINX_(__NR_mount,             sys_mount),             
   LINX_(__NR_umount,            sys_oldumount),         
   GENX_(__NR_setuid,            sys_setuid),            
   GENX_(__NR_getuid,            sys_getuid),            
   GENX_(__NR_alarm,             sys_alarm),             
   GENX_(__NR_pause,             sys_pause),             
   LINX_(__NR_utime,             sys_utime),                  
   GENX_(__NR_access,            sys_access),            
   GENX_(__NR_kill,              sys_kill),              
   GENX_(__NR_rename,            sys_rename),            
   GENX_(__NR_mkdir,             sys_mkdir),             

   GENX_(__NR_rmdir,             sys_rmdir),             
   GENXY(__NR_dup,               sys_dup),               
   LINXY(__NR_pipe,              sys_pipe),              
   GENXY(__NR_times,             sys_times),             
   GENX_(__NR_brk,               sys_brk),               
   GENX_(__NR_setgid,            sys_setgid),            
   GENX_(__NR_getgid,            sys_getgid),            
   GENX_(__NR_geteuid,           sys_geteuid),           

   GENX_(__NR_getegid,           sys_getegid),           
   LINX_(__NR_umount2,           sys_umount),            
   LINXY(__NR_ioctl,             sys_ioctl),             
   LINXY(__NR_fcntl,             sys_fcntl),             
   GENX_(__NR_setpgid,           sys_setpgid),           

   GENX_(__NR_umask,             sys_umask),             
   GENX_(__NR_chroot,            sys_chroot),            
   GENXY(__NR_dup2,              sys_dup2),              
   GENX_(__NR_getppid,           sys_getppid),           

   GENX_(__NR_getpgrp,           sys_getpgrp),           
   GENX_(__NR_setsid,            sys_setsid),            
   LINXY(__NR_sigaction,         sys_sigaction),         
   GENX_(__NR_setreuid,          sys_setreuid),          
   GENX_(__NR_setregid,          sys_setregid),          
   PLAX_(__NR_sigsuspend,        sys_sigsuspend),        
   LINXY(__NR_sigpending,        sys_sigpending),        
   GENX_(__NR_setrlimit,         sys_setrlimit),              
   GENXY(__NR_getrusage,         sys_getrusage),         
   GENXY(__NR_gettimeofday,      sys_gettimeofday),           
   GENXY(__NR_getgroups,         sys_getgroups),         
   GENX_(__NR_setgroups,         sys_setgroups),         
   GENX_(__NR_symlink,           sys_symlink),           
   GENX_(__NR_readlink,          sys_readlink),          

   PLAX_(__NR_mmap,              sys_mmap),                   
   GENXY(__NR_munmap,            sys_munmap),                 
   GENX_(__NR_truncate,          sys_truncate),          
   GENX_(__NR_ftruncate,         sys_ftruncate),         
   GENX_(__NR_fchmod,            sys_fchmod),            

   GENX_(__NR_fchown,            sys_fchown),            
   GENX_(__NR_getpriority,       sys_getpriority),       
   GENX_(__NR_setpriority,       sys_setpriority),       
   GENXY(__NR_statfs,            sys_statfs),            
   GENXY(__NR_fstatfs,           sys_fstatfs),           
   LINXY(__NR_socketcall,        sys_socketcall),        
   LINXY(__NR_syslog,            sys_syslog),            
   GENXY(__NR_setitimer,         sys_setitimer),         

   GENXY(__NR_getitimer,         sys_getitimer),         
   GENXY(__NR_stat,              sys_newstat),           
   GENXY(__NR_lstat,             sys_newlstat),          
   GENXY(__NR_fstat,             sys_newfstat),          
   LINX_(__NR_vhangup,           sys_vhangup),           
   GENXY(__NR_wait4,             sys_wait4),             
   LINXY(__NR_sysinfo,           sys_sysinfo),           
   LINXY(__NR_ipc,               sys_ipc),               
   GENX_(__NR_fsync,             sys_fsync),             
   PLAX_(__NR_sigreturn,         sys_sigreturn),         
   PLAX_(__NR_clone,             sys_clone),             
   GENXY(__NR_uname,             sys_newuname),          
   LINXY(__NR_adjtimex,          sys_adjtimex),          

   GENXY(__NR_mprotect,          sys_mprotect),          
   LINXY(__NR_sigprocmask,       sys_sigprocmask),       
   GENX_(__NR_create_module,     sys_ni_syscall),        
   LINX_(__NR_init_module,       sys_init_module),       
   LINX_(__NR_delete_module,     sys_delete_module),     
   GENX_(__NR_getpgid,           sys_getpgid),           
   GENX_(__NR_fchdir,            sys_fchdir),            
   LINX_(__NR_personality,       sys_personality),       
   LINX_(__NR_setfsuid,          sys_setfsuid),          
   LINX_(__NR_setfsgid,          sys_setfsgid),          

   LINXY(__NR__llseek,           sys_llseek),            
   GENXY(__NR_getdents,          sys_getdents),          
   GENX_(__NR__newselect,        sys_select),            
   GENX_(__NR_flock,             sys_flock),             
   GENX_(__NR_msync,             sys_msync),             
   GENXY(__NR_readv,             sys_readv),             
   GENX_(__NR_writev,            sys_writev),            
   GENX_(__NR_getsid,            sys_getsid),            
   GENX_(__NR_fdatasync,         sys_fdatasync),         
   LINXY(__NR__sysctl,           sys_sysctl),            
   GENX_(__NR_mlock,             sys_mlock),             
   GENX_(__NR_munlock,           sys_munlock),           
   GENX_(__NR_mlockall,          sys_mlockall),          
   LINX_(__NR_munlockall,        sys_munlockall),        
   LINXY(__NR_sched_setparam,    sys_sched_setparam),    
   LINXY(__NR_sched_getparam,         sys_sched_getparam),        
   LINX_(__NR_sched_setscheduler,     sys_sched_setscheduler),    
   LINX_(__NR_sched_getscheduler,     sys_sched_getscheduler),    
   LINX_(__NR_sched_yield,            sys_sched_yield),           
   LINX_(__NR_sched_get_priority_max, sys_sched_get_priority_max),

   LINX_(__NR_sched_get_priority_min, sys_sched_get_priority_min),
   LINXY(__NR_sched_rr_get_interval,  sys_sched_rr_get_interval), 
   GENXY(__NR_nanosleep,         sys_nanosleep),         
   GENX_(__NR_mremap,            sys_mremap),            
   LINX_(__NR_setresuid,         sys_setresuid),         

   LINXY(__NR_getresuid,         sys_getresuid),         

   GENXY(__NR_poll,              sys_poll),              
   LINX_(__NR_setresgid,         sys_setresgid),         
   LINXY(__NR_getresgid,         sys_getresgid),         
   LINXY(__NR_prctl,             sys_prctl),             
   PLAX_(__NR_rt_sigreturn,      sys_rt_sigreturn),      
   LINXY(__NR_rt_sigaction,      sys_rt_sigaction),      

   LINXY(__NR_rt_sigprocmask,    sys_rt_sigprocmask),    
   LINXY(__NR_rt_sigpending,     sys_rt_sigpending),     
   LINXY(__NR_rt_sigtimedwait,   sys_rt_sigtimedwait),   
   LINXY(__NR_rt_sigqueueinfo,   sys_rt_sigqueueinfo),   
   LINX_(__NR_rt_sigsuspend,     sys_rt_sigsuspend),     

   GENXY(__NR_pread64,           sys_pread64),           
   GENX_(__NR_pwrite64,          sys_pwrite64),          
   GENX_(__NR_chown,             sys_chown),             
   GENXY(__NR_getcwd,            sys_getcwd),            
   LINXY(__NR_capget,            sys_capget),            
   LINX_(__NR_capset,            sys_capset),            
   GENXY(__NR_sigaltstack,       sys_sigaltstack),       
   LINXY(__NR_sendfile,          sys_sendfile),          

   
   GENX_(__NR_vfork,             sys_fork),              
   GENXY(__NR_ugetrlimit,        sys_getrlimit),         
   LINX_(__NR_readahead,         sys_readahead),         
   PLAX_(__NR_mmap2,             sys_mmap2),             
   GENX_(__NR_truncate64,        sys_truncate64),        
   GENX_(__NR_ftruncate64,       sys_ftruncate64),       

   PLAXY(__NR_stat64,            sys_stat64),            
   PLAXY(__NR_lstat64,           sys_lstat64),           
   PLAXY(__NR_fstat64,           sys_fstat64),           


   GENXY(__NR_getdents64,        sys_getdents64),        
   LINXY(__NR_fcntl64,           sys_fcntl64),           
   GENX_(__NR_madvise,           sys_madvise),           
   GENXY(__NR_mincore,           sys_mincore),           
   LINX_(__NR_gettid,            sys_gettid),            
   LINX_(__NR_setxattr,          sys_setxattr),          
   LINX_(__NR_lsetxattr,         sys_lsetxattr),         
   LINX_(__NR_fsetxattr,         sys_fsetxattr),         
   LINXY(__NR_getxattr,          sys_getxattr),          
   LINXY(__NR_lgetxattr,         sys_lgetxattr),         
   LINXY(__NR_fgetxattr,         sys_fgetxattr),         
   LINXY(__NR_listxattr,         sys_listxattr),         
   LINXY(__NR_llistxattr,        sys_llistxattr),        
   LINXY(__NR_flistxattr,        sys_flistxattr),        
   LINX_(__NR_removexattr,       sys_removexattr),       
   LINX_(__NR_lremovexattr,      sys_lremovexattr),      
   LINX_(__NR_fremovexattr,      sys_fremovexattr),      

   LINXY(__NR_futex,             sys_futex),                  
   LINX_(__NR_sched_setaffinity, sys_sched_setaffinity), 
   LINXY(__NR_sched_getaffinity, sys_sched_getaffinity), 


   LINXY(__NR_sendfile64,        sys_sendfile64),        
   LINX_(__NR_io_setup,          sys_io_setup),          
   LINX_(__NR_io_destroy,        sys_io_destroy),        
   LINXY(__NR_io_getevents,      sys_io_getevents),      
   LINX_(__NR_io_submit,         sys_io_submit),         
   LINXY(__NR_io_cancel,         sys_io_cancel),         
   LINX_(__NR_set_tid_address,   sys_set_tid_address),   

   LINX_(__NR_fadvise64,         sys_fadvise64),         
   LINX_(__NR_exit_group,        sys_exit_group),        
   LINXY(__NR_epoll_create,      sys_epoll_create),      
   LINX_(__NR_epoll_ctl,         sys_epoll_ctl),         
   LINXY(__NR_epoll_wait,        sys_epoll_wait),        

   LINXY(__NR_timer_create,      sys_timer_create),      
   LINXY(__NR_timer_settime,     sys_timer_settime),     
   LINXY(__NR_timer_gettime,     sys_timer_gettime),     
   LINX_(__NR_timer_getoverrun,  sys_timer_getoverrun),  
   LINX_(__NR_timer_delete,      sys_timer_delete),      
   LINX_(__NR_clock_settime,     sys_clock_settime),     
   LINXY(__NR_clock_gettime,     sys_clock_gettime),     
   LINXY(__NR_clock_getres,      sys_clock_getres),      
   LINXY(__NR_clock_nanosleep,   sys_clock_nanosleep),   


   LINXY(__NR_tgkill,            sys_tgkill),            
   GENXY(__NR_statfs64,          sys_statfs64),          
   GENXY(__NR_fstatfs64,         sys_fstatfs64),         
   LINX_(__NR_fadvise64_64,      sys_fadvise64_64),      


   LINX_(__NR_mbind,             sys_mbind),             
   LINXY(__NR_get_mempolicy,     sys_get_mempolicy),          
   LINX_(__NR_set_mempolicy,     sys_set_mempolicy),          

   LINXY(__NR_mq_open,           sys_mq_open),           
   LINX_(__NR_mq_unlink,         sys_mq_unlink),         
   LINX_(__NR_mq_timedsend,      sys_mq_timedsend),      
   LINXY(__NR_mq_timedreceive,   sys_mq_timedreceive),   
   LINX_(__NR_mq_notify,         sys_mq_notify),         
   LINXY(__NR_mq_getsetattr,     sys_mq_getsetattr),     

   LINX_(__NR_ioprio_set,        sys_ioprio_set),         
   LINX_(__NR_ioprio_get,        sys_ioprio_get),         

   LINX_(__NR_inotify_init,  sys_inotify_init),               
   LINX_(__NR_inotify_add_watch,  sys_inotify_add_watch),     
   LINX_(__NR_inotify_rm_watch,   sys_inotify_rm_watch),      
   PLAXY(__NR_spu_run,            sys_spu_run),               
   PLAX_(__NR_spu_create,         sys_spu_create),            

   LINX_(__NR_pselect6,          sys_pselect6),          
   LINXY(__NR_ppoll,             sys_ppoll),             

   LINXY(__NR_openat,            sys_openat),            
   LINX_(__NR_mkdirat,           sys_mkdirat),           
   LINX_(__NR_mknodat,           sys_mknodat),           
   LINX_(__NR_fchownat,          sys_fchownat),          
   LINX_(__NR_futimesat,         sys_futimesat),         
   PLAXY(__NR_fstatat64,         sys_fstatat64),         
   LINX_(__NR_unlinkat,          sys_unlinkat),          
   LINX_(__NR_renameat,          sys_renameat),          
   LINX_(__NR_linkat,            sys_linkat),            
   LINX_(__NR_symlinkat,         sys_symlinkat),         
   LINX_(__NR_readlinkat,        sys_readlinkat),        
   LINX_(__NR_fchmodat,          sys_fchmodat),          
   LINX_(__NR_faccessat,         sys_faccessat),         
   LINX_(__NR_set_robust_list,   sys_set_robust_list),   
   LINXY(__NR_get_robust_list,   sys_get_robust_list),   
   LINXY(__NR_move_pages,        sys_move_pages),        
   LINXY(__NR_getcpu,            sys_getcpu),            
   LINXY(__NR_epoll_pwait,       sys_epoll_pwait),       
   LINX_(__NR_utimensat,         sys_utimensat),         
   LINXY(__NR_signalfd,          sys_signalfd),          
   LINXY(__NR_timerfd_create,    sys_timerfd_create),    
   LINXY(__NR_eventfd,           sys_eventfd),           
   LINX_(__NR_sync_file_range2,  sys_sync_file_range2),  
   LINX_(__NR_fallocate,         sys_fallocate),         
   LINXY(__NR_timerfd_settime,   sys_timerfd_settime),  
   LINXY(__NR_timerfd_gettime,   sys_timerfd_gettime),  
   LINXY(__NR_signalfd4,         sys_signalfd4),        
   LINXY(__NR_eventfd2,          sys_eventfd2),         
   LINXY(__NR_epoll_create1,     sys_epoll_create1),    
   LINXY(__NR_dup3,              sys_dup3),             
   LINXY(__NR_pipe2,             sys_pipe2),            
   LINXY(__NR_inotify_init1,     sys_inotify_init1),    
   LINXY(__NR_perf_event_open,   sys_perf_event_open),  
   LINXY(__NR_preadv,            sys_preadv),           
   LINX_(__NR_pwritev,           sys_pwritev),          
   LINXY(__NR_rt_tgsigqueueinfo, sys_rt_tgsigqueueinfo),

   LINX_(__NR_clock_adjtime,     sys_clock_adjtime),    

   LINXY(__NR_process_vm_readv,  sys_process_vm_readv), 
   LINX_(__NR_process_vm_writev, sys_process_vm_writev) 
};

SyscallTableEntry* ML_(get_linux_syscall_entry) ( UInt sysno )
{
   const UInt syscall_table_size
      = sizeof(syscall_table) / sizeof(syscall_table[0]);

   
   if (sysno < syscall_table_size) {
      SyscallTableEntry* sys = &syscall_table[sysno];
      if (sys->before == NULL)
         return NULL; 
      else
         return sys;
   }

   
   return NULL;
}

#endif 
