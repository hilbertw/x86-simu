#define  _CRT_SECURE_NO_WARNINGS 1
#include <conio.h>
#include <stdio.h>
#include "cpu.h"
#include "util.h"
#include "log.h"
extern CMCH mch;

extern CCPU cpu[CFG_N_CPUS];
char * linux_scall_fmt[256]=
{
	 "sys_ni_syscall\n",	/* 0  -  old "setup(\n"," system call*/
	 "sys_exit\n",
	 "sys_fork\n",
	 "sys_read: fd=%x,buf=%x,count=%x\n",
	 "sys_write: fd=%x,buf=%x,count=%x\n",
	 "sys_open:name=%x,flag=%x,mode=%x\n",		/* 5 */
	 "sys_close: fd=%x\n",
	 "sys_waitpid\n",
	 "sys_creat:pathname=%x,  mode=%x\n",
	 "sys_link\n",
	 "sys_unlink: pathname=%x\n",		/* 10 */
	 "sys_execve:name=%x, argv=%x,envp=%x, regsn=%x\n",
	 "sys_chdir\n",
	 "sys_time\n",
	 "sys_mknod\n",
	 "sys_chmod\n",		/* 15 */
	 "sys_lchown16\n",
	 "sys_ni_syscall\n",				/* old break syscall holder */
	 "sys_stat\n",
	 "sys_lseek:fd=%x, offset=%x,origin=%x\n",
	 "sys_getpid\n",		/* 20 */
	 "sys_mount\n",
	 "sys_oldumount\n",
	 "sys_setuid16\n",
	 "sys_getuid16\n",
	 "sys_stime\n",		/* 25 */
	 "sys_ptrace\n",
	 "sys_alarm\n",
	 "sys_fstat\n",
	 "sys_pause\n",
	 "sys_utime\n",		/* 30 */
	 "sys_ni_syscall\n",				/* old stty syscall holder */
	 "sys_ni_syscall\n",				/* old gtty syscall holder */
	 "sys_access\n",
	 "sys_nice\n",
	 "sys_ni_syscall\n",	/* 35 */		/* old ftime syscall holder */
	 "sys_sync\n",
	 "sys_kill\n",
	 "sys_rename\n",
	 "sys_mkdir\n",
	 "sys_rmdir\n",		/* 40 */
	 "sys_dup\n",
	 "sys_pipe\n",
	 "sys_times\n",
	 "sys_ni_syscall\n",				/* old prof syscall holder */
	 "sys_brk:brk=%x\n",		/* 45 */
	 "sys_setgid16\n",
	 "sys_getgid16\n",
	 "sys_signal\n",
	 "sys_geteuid16\n",
	 "sys_getegid16\n",	/* 50 */
	 "sys_acct\n",
	 "sys_umount\n",					/* recycled never used phys(\n", */
	 "sys_ni_syscall\n",				/* old lock syscall holder */
	 "sys_ioctl:fd=%x,cmd=%x,para=%x\n",
	 "sys_fcntl\n",		/* 55 */
	 "sys_ni_syscall\n",				/* old mpx syscall holder */
	 "sys_setpgid\n",
	 "sys_ni_syscall\n",				/* old ulimit syscall holder */
	 "sys_olduname\n",
	 "sys_umask\n",		/* 60 */
	 "sys_chroot\n",
	 "sys_ustat\n",
	 "sys_dup2\n",
	 "sys_getppid\n",
	 "sys_getpgrp\n",		/* 65 */
	 "sys_setsid\n",
	 "sys_sigaction\n",
	 "sys_sgetmask\n",
	 "sys_ssetmask\n",
	 "sys_setreuid16\n",	/* 70 */
	 "sys_setregid16\n",
	 "sys_sigsuspend\n",
	 "sys_sigpending\n",
	 "sys_sethostname\n",
	 "sys_setrlimit\n",	/* 75 */
	 "sys_old_getrlimit\n",
	 "sys_getrusage\n",
	 "sys_gettimeofday\n",
	 "sys_settimeofday\n",
	 "sys_getgroups16\n",	/* 80 */
	 "sys_setgroups16\n",
	 "old_select\n",
	 "sys_symlink\n",
	 "sys_lstat\n",
	 "sys_readlink\n",		/* 85 */
	 "sys_uselib\n",
	 "sys_swapon\n",
	 "sys_reboot\n",
	 "old_readdir\n",
	 "old_mmap\n",		/* 90 */
	 "sys_munmap\n",
	 "sys_truncate\n",
	 "sys_ftruncate\n",
	 "sys_fchmod\n",
	 "sys_fchown16\n",		/* 95 */
	 "sys_getpriority\n",
	 "sys_setpriority\n",
	 "sys_ni_syscall\n",				/* old profil syscall holder */
	 "sys_statfs\n",
	 "sys_fstatfs\n",		/* 100 */
	 "sys_ioperm\n",
	 "sys_socketcall\n",
	 "sys_syslog\n",
	 "sys_setitimer\n",
	 "sys_getitimer\n",	/* 105 */
	 "sys_newstat\n",
	 "sys_newlstat\n",
	 "sys_newfstat\n",
	 "sys_uname\n",
	 "sys_iopl\n",		/* 110 */
	 "sys_vhangup\n",
	 "sys_ni_syscall\n",	/* old "idle" system call */
	 "sys_vm86old\n",
	 "sys_wait4\n",
	 "sys_swapoff\n",		/* 115 */
	 "sys_sysinfo\n",
	 "sys_ipc\n",
	 "sys_fsync\n",
	 "sys_sigreturn\n",
	 "sys_clone\n",		/* 120 */
	 "sys_setdomainname\n",
	 "sys_newuname\n",
	 "sys_modify_ldt\n",
	 "sys_adjtimex\n",
	 "sys_mprotect\n",		/* 125 */
	 "sys_sigprocmask\n",
	 "sys_create_module:name=%x, size=%x\n",
	 "sys_init_module:name_user=%x, mod_user=%x\n",
	 "sys_delete_module\n",
	 "sys_get_kernel_syms\n",	/* 130 */
	 "sys_quotactl\n",
	 "sys_getpgid\n",
	 "sys_fchdir\n",
	 "sys_bdflush\n",
	 "sys_sysfs\n",		/* 135 */
	 "sys_personality\n",
	 "sys_ni_syscall\n",	/* for afs_syscall */
	 "sys_setfsuid16\n",
	 "sys_setfsgid16\n",
	 "sys_llseek\n",		/* 140 */
	 "sys_getdents\n",
	 "sys_select\n",
	 "sys_flock\n",
	 "sys_msync\n",
	 "sys_readv\n",		/* 145 */
	 "sys_writev\n",
	 "sys_getsid\n",
	 "sys_fdatasync\n",
	 "sys_sysctl\n",
	 "sys_mlock\n",		/* 150 */
	 "sys_munlock\n",
	 "sys_mlockall\n",
	 "sys_munlockall\n",
	 "sys_sched_setparam\n",
	 "sys_sched_getparam\n",   /* 155 */
	 "sys_sched_setscheduler\n",
	 "sys_sched_getscheduler\n",
	 "sys_sched_yield\n",
	 "sys_sched_get_priority_max\n",
	 "sys_sched_get_priority_min\n",  /* 160 */
	 "sys_sched_rr_get_interval\n",
	 "sys_nanosleep\n",
	 "sys_mremap\n",
	 "sys_setresuid16\n",
	 "sys_getresuid16\n",	/* 165 */
	 "sys_vm86\n",
	 "sys_query_module:name_user=%x, which=%x, buf=%x,bufsize=%x,ret=%x\n",
	 "sys_poll\n",
	 "sys_nfsservctl\n",
	 "sys_setresgid16\n",	/* 170 */
	 "sys_getresgid16\n",
	 "sys_prctl\n",
	 "sys_rt_sigreturn\n",
	 "sys_rt_sigaction\n",
	 "sys_rt_sigprocmask\n",	/* 175 */
	 "sys_rt_sigpending\n",
	 "sys_rt_sigtimedwait\n",
	 "sys_rt_sigqueueinfo\n",
	 "sys_rt_sigsuspend\n",
	 "sys_pread\n",		/* 180 */
	 "sys_pwrite\n",
	 "sys_chown16\n",
	 "sys_getcwd\n",
	 "sys_capget\n",
	 "sys_capset\n",           /* 185 */
	 "sys_sigaltstack\n",
	 "sys_sendfile\n",
	 "sys_ni_syscall\n",		/* streams1 */
	 "sys_ni_syscall\n",		/* streams2 */
	 "sys_vfork\n",            /* 190 */
	 "sys_getrlimit\n",
	 "sys_mmap2\n",
	 "sys_truncate64\n",
	 "sys_ftruncate64\n",
	 "sys_stat64: filename=%x, statbuf=%x,  flags=%x\n",		/* 195 */
	 "sys_lstat64: filename=%x, statbuf=%x,  flags=%x\n",
	 "sys_fstat64: fd=%x, statbuf=%x,  flags=%x\n",
	 "sys_lchown\n",
	 "sys_getuid\n",
	 "sys_getgid\n",		/* 200 */
	 "sys_geteuid\n",
	 "sys_getegid\n",
	 "sys_setreuid\n",
	 "sys_setregid\n",
	 "sys_getgroups\n",	/* 205 */
	 "sys_setgroups\n",
	 "sys_fchown\n",
	 "sys_setresuid\n",
	 "sys_getresuid\n",
	 "sys_setresgid\n",	/* 210 */
	 "sys_getresgid\n",
	 "sys_chown\n",
	 "sys_setuid\n",
	 "sys_setgid\n",
	 "sys_setfsuid\n",		/* 215 */
	 "sys_setfsgid\n",
	 "sys_pivot_root\n",
	 "sys_mincore\n",
	 "sys_madvise\n",
	 "sys_getdents64\n",	/* 220 */
	 "sys_fcntl64\n",
	 "sys_ni_syscall\n",	/* reserved for TUX */
	 "sys_ni_syscall\n",	/* Reserved for Security */
	 "sys_gettid\n",
	 "sys_readahead\n",	/* 225 */
	 "sys_setxattr\n",
	 "sys_lsetxattr\n",
	 "sys_fsetxattr\n",
	 "sys_getxattr\n",
	 "sys_lgetxattr\n",	/* 230 */
	 "sys_fgetxattr\n",
	 "sys_listxattr\n",
	 "sys_llistxattr\n",
	 "sys_flistxattr\n",
	 "sys_removexattr\n",	/* 235 */
	 "sys_lremovexattr\n",
	 "sys_fremovexattr\n",
 	 "sys_tkill\n",
	 "sys_ni_syscall\n",	/* reserved for sendfile64 */
	 "sys_ni_syscall\n",	/* 240 reserved for futex */
	 "sys_ni_syscall\n",	/* reserved for sched_setaffinity */
	 "sys_ni_syscall\n",	/* reserved for sched_getaffinity */
	 "sys_ni_syscall\n",	/* sys_set_thread_area */
	 "sys_ni_syscall\n",	/* sys_get_thread_area */
	 "sys_ni_syscall\n",	/* 245 sys_io_setup */
	 "sys_ni_syscall\n",	/* sys_io_destroy */
	 "sys_ni_syscall\n",	/* sys_io_getevents */
	 "sys_ni_syscall\n",	/* sys_io_submit */
	 "sys_ni_syscall\n",	/* sys_io_cancel */
	 "sys_ni_syscall\n",	/* 250 sys_alloc_hugepages */
	 "sys_ni_syscall\n",	/* sys_free_hugepages */
	 "sys_ni_syscall\n"	/* sys_exit_group */
};

void dump_children( unsigned long addr,unsigned long head,int level);

void dump_tsk( unsigned long addr,int level)
{
		unsigned long pid,state,grp_leader,ppid,real_p,children;


		addr&=~0x1fff;

		// state:
		cpu[0].mmu.lddw(BASE(cpu[0].ss_desc)+addr,0,& state);
		cpu[0].mmu.lddw(BASE(cpu[0].ss_desc)+addr+0x9c,0,& pid);
		cpu[0].mmu.lddw(BASE(cpu[0].ss_desc)+addr+0xac,0,& children);
		cpu[0].mmu.lddw(BASE(cpu[0].ss_desc)+addr+0xbc,0,& grp_leader);
		cpu[0].mmu.lddw(BASE(cpu[0].ss_desc)+grp_leader+0xa4,0,& real_p);
		cpu[0].mmu.lddw(BASE(cpu[0].ss_desc)+(real_p&~0x1fff)+0x9c,0,& ppid);

		for(int i =0;i<level;i++) log0("\t");
		log0(" %x ppid:%x,pid:%x,state:%x\n\n",addr,ppid,pid,state);
		dump_children(children,addr+0xac,level);

}

void dump_children( unsigned long addr,unsigned long head,int level )
{
	unsigned long sibling;
	
	int i=0;

	sibling=addr;

	while(sibling!=head&&i<30)
	{
	i++;
	dump_tsk(sibling,level+1);
	cpu[0].mmu.lddw(BASE(cpu[0].ss_desc)+sibling,0,& sibling);


	}


}

//
//c0364d00
void dump_rq(unsigned long rq)
{
}