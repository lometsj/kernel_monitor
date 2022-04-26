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
extern int send_msg(char *, uint16_t, __u32);

probe t;
int idx;

int my_open(struct kprobe *p, struct pt_regs *regs){
	char buf[255] = {0};
	char filename[255] = {0};
	strncpy_from_user(filename, regs->si, 255-1);
	int flags = regs->dx;
	int mode = regs->r10;
	snprintf(buf, 255-1, "detect open[%p], uid=%u, proc_name=%s, pid=%d, filename=%s",p->addr,current->cred->uid, current->comm, current->pid, filename);
	send_msg(buf, 255, 100);
	return 0;
}


static int __init init_open(void){
	strcpy(t.sym_name, "do_sys_open");
	t.entry = my_open;
	idx = regist_probe(t);
	return 0;
}

static void __exit exit_open(void){
	unregist_probe(idx);
}

module_init(init_open);
module_exit(exit_open);
MODULE_LICENSE("GPL");