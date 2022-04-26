#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/kprobes.h>


#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/kprobes.h>

#include "trace.h"
#include <linux/string.h>



extern int regist_probe(probe);
extern int unregist_probe(unsigned int);
extern probe* list_probe(void);
extern int send_msg(int, char *, uint16_t, __u32);

probe t;
int idx;

int my_execve(struct kprobe *p, struct pt_regs *regs){
	char buf[255];
	char filename[100] = {0};
	strncpy_from_user(filename, (char *)regs->di, 100);
	snprintf(buf, 255-1, "ebx:%p ecx:%p edx:%p",regs->bx, regs->cx, regs->dx);
	send_msg(idx, buf, 255, 100);
	snprintf(buf, 255-1, "detect kill, uid=%u, proc_name=%s, pid=%d, filename=%s",current->cred->uid, current->comm, current->pid, filename);
	send_msg(idx, buf, 255, 100);
	return 0;
}


static int __init init_execve(void){
	strcpy(t.sym_name, "__x64_sys_kill");
	t.entry = my_execve;
	idx = regist_probe(t);
	return 0;
}

static void __exit exit_execve(void){
	unregist_probe(idx);
}

module_init(init_execve);
module_exit(exit_execve);
MODULE_LICENSE("GPL");
