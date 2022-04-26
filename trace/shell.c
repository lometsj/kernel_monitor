#include <linux/init.h>
#include <linux/module.h>
#include <linux/types.h>
#include <net/sock.h>
#include <linux/netlink.h>

#define NETLINK_SHELL    31
#define USER_PORT        100
int netlink_count = 0;
char netlink_kmsg[30];

struct sock *nlsk = NULL;
extern struct net init_net;




int send_usrmsg(char *pbuf, uint16_t len, __u32 port_id)
{
    struct sk_buff *nl_skb;
    struct nlmsghdr *nlh;

    int ret;

    if(nlsk == 0){
        pr_info("nlsk is null,check init");
        return -1;
    }

    //Create sk_buff using nlmsg_new().
    nl_skb = nlmsg_new(len, GFP_ATOMIC);
    if(!nl_skb)
    {
        printk("netlink alloc failure\n");
        return -1;
    }

    //Set up nlmsghdr.
    nlh = nlmsg_put(nl_skb, 0, 0, NETLINK_SHELL, len, 0);
    if(nlh == NULL)
    {
        printk("nlmsg_put failaure \n");
        nlmsg_free(nl_skb);  //If nlmsg_put() failed, nlmsg_free() will free sk_buff.
        return -1;
    }

    //Copy pbuf to nlmsghdr payload.
    memcpy(nlmsg_data(nlh), pbuf, len);
    ret = netlink_unicast(nlsk, nl_skb, port_id, MSG_DONTWAIT);

    return ret;
}

int memory_display(char *msg){
	char buf[255] = {0};
	unsigned long addr = 0;
	size_t len,idx=0;
	char *p;
	char *tmp;
	int ret;
	p = strsep(&msg, " ");
	if(!p) return -1;
	ret = kstrtoul(p, 16, &addr);
	if(ret < 0)return -1;
	p = strsep(&msg, " ");
	if(!p) return -1;
	ret = kstrtoul(p, 16, &len);
	if(ret < 0)return -1;
	tmp = (char *)addr;
	pr_info("md addr=%lx count=%lu",addr, len);
    ret = 0;
	while(idx<len){
		ret += snprintf(buf+ret, 254-ret, "%.2x ", tmp[idx]&0xff);
        idx++;
	}
    send_usrmsg(buf, ret, 100);
	return 0;
}


static void netlink_rcv_msg(struct sk_buff *skb)
{
	pr_info("rcv msg");
    struct nlmsghdr *nlh = NULL;
    char *umsg = NULL;
    //char *kmsg = "hello users!!!";
    char *kmsg,*p,*msg;
    int ret = 0;

    if(skb->len >= nlmsg_total_size(0))
    {
    	kmsg = netlink_kmsg;
        nlh = nlmsg_hdr(skb);  //Get nlmsghdr from sk_buff.
        umsg = NLMSG_DATA(nlh); //Get payload from nlmsghdr.
        if(umsg)
        {
        	msg = umsg;
        	p = strsep(&msg, " ");

			if(p){
				if(!strcmp(p, "md")){
					pr_info("md order");
					ret = memory_display(msg);
				}
			}

            //printk("kernel recv from user: %s\n", umsg);
            //send_usrmsg(kmsg, strlen(kmsg), 100);
        }
    }
}

struct netlink_kernel_cfg cfg = { 
        .input = netlink_rcv_msg
}; 

__init int netlink_test_init(void)
{

    pr_info("tranfer init");
    nlsk = (struct sock *)netlink_kernel_create(&init_net, NETLINK_SHELL, &cfg);
    if(nlsk == NULL)
    {   
        printk("netlink_kernel_create error !\n");
        return -1; 
    }
    return 0;
}

__exit void netlink_test_exit(void)
{
    pr_info("transfer out");
    netlink_kernel_release(nlsk);
    printk("netlink_test_exit!\n");
}

module_init(netlink_test_init);
module_exit(netlink_test_exit);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("netlink test");