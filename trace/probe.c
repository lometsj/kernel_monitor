#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/kprobes.h>


#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/kprobes.h>

#include "trace.h"
#include <linux/string.h>
#include <linux/slab.h>

static probe PROBE[64];
static unsigned int probe_num = 0;

extern int send_usrmsg(char *, uint16_t , __u32);


int handler_fault(struct kprobe *p, struct pt_regs *regs, int trapnr)
{
    pr_info("fault_handler: p->addr = %pF, trap #%dn", p->addr, trapnr);
    return 0;
}

int find_probe(const char *symbol_name, unsigned long long address){
    int idx = -1;
    int i=0;
    while(i<64){
        if((PROBE[i].address != 0 &&PROBE[i].address == address ) || (PROBE[i].sym_name !=0 && !strcmp(PROBE[i].sym_name,symbol_name) ) ){
            idx = i;
            break;
        }
        i++;
    }
    return idx;

}



probe *list_probe(void){
    return &PROBE[0];
}




int find_probe_slot(void){
    int idx = -1;
    int i=0;
    while(i<64){
        if(PROBE[i].is_use == 0){
            idx = i;
            break;
        }
        i++;
    }
    return idx;
}

static int regist_probe(probe t){
    //kallsyms_lookup_name
    int idx, ret, nlsk_idx;
    struct kprobe *kp;
    

    if(probe_num >= 64){
        goto invalid;
    }
    kp = kmalloc(sizeof(struct kprobe), GFP_KERNEL);
    // jp = kmalloc(sizeof(struct jprobe), GFP_KERNEL);

    if((t.sym_name[0] != 0 && t.address !=0) || (t.sym_name[0] ==0 && t.address ==0)){
        goto invalid;
    }
    if(t.sym_name[0] != 0){
        char *sym_name = kmalloc(strlen(t.sym_name)+1, GFP_KERNEL);
        strcpy(sym_name, t.sym_name);
        kp->symbol_name = sym_name;
        // jp->kp->symbol_name = sym_name;
    }else if(t.sym_name[0] == 0&& t.address != 0){
        // memcpy(&(kp->addr), &(t.address), sizeof(unsigned long long));
        memcpy(&(kp->addr), &(t.address), sizeof(unsigned long long));
    }

    // jp->entry = t.entry;
    kp->pre_handler = t.entry;
    kp->fault_handler = handler_fault;

    idx = find_probe_slot();
    if(idx < 0){
        goto invalid;
    }
    strcpy(PROBE[idx].sym_name, t.sym_name);
    PROBE[idx].address = t.address;
    PROBE[idx].is_use = 1;
    // PROBE[idx].private.jp = jp;
    PROBE[idx].private.kp = kp;
    PROBE[idx].entry = t.entry;

    pr_info("prepare reg kprobe: sym_name:%s, addr:%p, entry:%p\n",kp->symbol_name, kp->addr, PROBE[idx].entry);

    ret = register_kprobe(PROBE[idx].private.kp);
    // ret = register_jprobe(jp);
    if (ret < 0) {
        pr_err("register_kprobe failed, returned %d\n", ret);
        goto invalid;
    }

    return idx;

    invalid:
        kfree(kp);
        return -1;

}

int send_msg(char *pbuf, uint16_t len, __u32 port_id){
    return send_usrmsg(pbuf, len, port_id);
}
EXPORT_SYMBOL(send_msg);


static int unregist_probe(unsigned int idx){
    if(idx > 64){
        return -1;
    }
    if(PROBE[idx].is_use != 0){

        unregister_kprobe(PROBE[idx].private.kp);
        PROBE[idx].is_use = 0;
        kfree(PROBE[idx].private.kp);
        PROBE[idx].private.kp = 0;
        return 0;
    }else{
        return -1;
    }
}



static int __init kprobe_init(void)
{

    pr_info("tsj init\n");
    return 0;
}

static void __exit kprobe_exit(void)
{
    pr_info("tsj exit\n");
}


EXPORT_SYMBOL(unregist_probe);
EXPORT_SYMBOL(regist_probe);
EXPORT_SYMBOL(list_probe);
module_init(kprobe_init);
module_exit(kprobe_exit);
MODULE_LICENSE("GPL");
