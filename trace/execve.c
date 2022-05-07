#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/kprobes.h>
#include <linux/binfmts.h>
#include <linux/version.h>
#include <linux/kallsyms.h>


#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/kprobes.h>

#include "trace.h"
#include <linux/string.h>
#include <linux/skbuff.h>

extern int regist_probe(probe);
extern int unregist_probe(unsigned int);
extern probe* list_probe(void);
extern int send_msg(char *, uint16_t, __u32);

probe t;
int idx;

struct user_arg_ptr {
#ifdef CONFIG_COMPAT
	bool is_compat;
#endif
	union {
		const char __user *const __user *native;
#ifdef CONFIG_COMPAT
		const compat_uptr_t __user *compat;
#endif
	} ptr;
};

static const char __user *get_user_arg_ptr(struct user_arg_ptr argv, int nr)
{
	const char __user *native;

#ifdef CONFIG_COMPAT
	if (unlikely(argv.is_compat)) {
		compat_uptr_t compat;

		if (get_user(compat, argv.ptr.compat + nr))
			return ERR_PTR(-EFAULT);

		return compat_ptr(compat);
	}
#endif

	if (get_user(native, argv.ptr.native + nr))
		return ERR_PTR(-EFAULT);

	return native;
}





int my_execve(struct kprobe *p, struct pt_regs *regs){
	char buf[1024];
	struct filename *fn = regs->si;
	char *filename = fn->name;
	int argc,envc;
	char **argv = regs->cx;
	char **envp = regs->bx;
	int len=0;
	int i = 0;
	unsigned long ptr = 0;
	char buf_tmp[128];
	unsigned long native;
	// snprintf(buf, 255-1, "filename:%s, argv[0]=%s, envp=%s", filename, argv[0], envp[0]);
	// send_msg(idx, buf, 255, 100);
	pr_info("rdi=%lx, rsi=%lx, rdx=%lx, rcx=%lx, r10=%lx", regs->di, regs->si, regs->dx, regs->cx, regs->r10);
	len += snprintf(buf, 1024-1, "execve: uid=%u , pname=%s , pid=%d , ppname=%s , ppid=%d , path=%s , args=[",current->cred->uid, current->comm, current->pid, current->parent->comm, current->parent->pid, filename);
	if(!strncmp(filename, "./test", 4) || 1){
		pr_info("test execve??");

		while(!copy_from_user(&native, argv+i, 8) && native!=NULL){
			pr_info("native :%lx", native);
			strncpy_from_user(buf_tmp, native, 128);
			pr_info("arg %d : %s",i,buf_tmp);
			len += snprintf(buf+len, 1024-1-len, "\"%s\", ", buf_tmp);
			i++;
		}
		i=0;
		// while(!copy_from_user(&native, envp+i, 8) && native!=NULL){
		// 	pr_info("native :%lx", native);
		// 	strncpy_from_user(buf_tmp, native, 128);
		// 	pr_info("arg %d : %s",i,buf_tmp);
		// 	len += snprintf(buf+len, 1024-1-len, "\"%s\", ", buf_tmp);
		// 	i++;
		// }
	}

	len += snprintf(buf+len, 1024-1-len, " NULL])");
	buf[1023] = '\x00';
	pr_info("%s",buf);
	send_msg(buf, 1024, 101);
	return 0;
}

static int __init init_execve(void){
	strcpy(t.sym_name, "do_execveat_common.isra.0");
	t.entry = my_execve;
	idx = regist_probe(t);
	return 0;
}

static void __exit exit_execve(void){
	pr_info("exit execve");
	unregist_probe(idx);
}

module_init(init_execve);
module_exit(exit_execve);
MODULE_LICENSE("GPL");
