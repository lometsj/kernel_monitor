#include <linux/init.h>
#include <linux/module.h>
#include <linux/types.h>
#include <net/sock.h>
#include <linux/netlink.h>


unsigned long arr[255];

int vul1_int(char *buf){
	char buf_over[16] = {0};
	int cmd = buf[0];
	int idx = *(int *)buf+4;
	unsigned long value = *(unsigned long *)buf+8;
	switch(cmd){
		case 1:
			if(idx > 255){
				return -1;
			}
			arr[idx] = value;
			break;
		case 2:
			pr_info("hello\n");
			break;
		case 3:
			memcpy(buf_over, buf+12, value);
			break;
		case 4:
			snprintf(buf_over, 15, buf+12, value);
			break;

	}
	return 0;
}

//tsj protocol
// | 1 bytes | 3 bytes | 
//  version     magic

// int vul_parse(char *buf){
// 	uint8_t version;
// 	char magic[3] = "tsj";
// 	int loop_count = 0;
// 	while(loop_count < 10000){

// 	}

// }

__init int vul_init(void)
{

    pr_info("tranfer init");
    return 0;
}

__exit void vul_exit(void)
{
    pr_info("transfer out");
    printk("netlink_test_exit!\n");
}

module_init(vul_init);
module_exit(vul_exit);
MODULE_LICENSE("GPL");