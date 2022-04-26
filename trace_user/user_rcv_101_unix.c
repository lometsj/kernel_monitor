#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <string.h>
#include <linux/netlink.h>
#include <stdint.h>
#include <unistd.h>
#include <errno.h>
#include <sys/un.h>

#define NETLINK_TEST    30
#define USER_PORT    101
#define MAX_PLOAD    125
#define MSG_LEN        1024

typedef struct _user_msg_info
{
    struct nlmsghdr hdr;
    char  msg[MSG_LEN];
} user_msg_info;




int init_unix_domian_socket(char *path){
    struct  sockaddr_un srv_addr;
    int ret;
    int connect_fd = socket(PF_UNIX, SOCK_STREAM, 0);
    if(connect_fd < 0) {
        perror("cannot create communication socket");
        return 1;
    }
    srv_addr.sun_family = AF_UNIX;
    strcpy(srv_addr.sun_path,path);
    //connect server
    ret = connect(connect_fd, (struct sockaddr*)&srv_addr, sizeof(srv_addr));
    if(ret == -1) {
        perror("cannot connect to the server"); 
        close(connect_fd);
        return 1;
    }
    return connect_fd;
}

int main(int argc, char **argv)
{
    if(argc < 3){
        printf("usage: ./101.elf [unxi_socket_path] [node_id]\n");
        exit(0);
    }
    int out_fd = init_unix_domian_socket(argv[1]);
    int skfd;
    int ret;
    user_msg_info u_info;
    socklen_t len;
    struct nlmsghdr *nlh = NULL;
    struct sockaddr_nl saddr, daddr;
    char *umsg = "hello netlink!!";
    int loop_count = 0;

    /*Create netlink socket*/
    skfd = socket(AF_NETLINK, SOCK_RAW, NETLINK_TEST); //Create a socket using user defined protocol NETLINK_TEST.
    if(skfd == -1)
    {
        perror("create socket error\n");
        return -1;
    }

    //Source address.
    memset(&saddr, 0, sizeof(saddr));
    saddr.nl_family = AF_NETLINK; //AF_NETLINK
    saddr.nl_pid = USER_PORT;  //netlink portid, same as kernel.
    saddr.nl_groups = 0;
    if(bind(skfd, (struct sockaddr *)&saddr, sizeof(saddr)) != 0) //bind to skfd with saddr.
    {
        perror("bind() error\n");
        close(skfd);
        return -1;
    }

    //Destination address.
    memset(&daddr, 0, sizeof(daddr));
    daddr.nl_family = AF_NETLINK;
    daddr.nl_pid = 0;    // to kernel 
    daddr.nl_groups = 0;

    nlh = (struct nlmsghdr *)malloc(NLMSG_SPACE(MAX_PLOAD));
    memset(nlh, 0, sizeof(struct nlmsghdr));
    nlh->nlmsg_len = NLMSG_SPACE(MAX_PLOAD);
    nlh->nlmsg_flags = 0;
    nlh->nlmsg_type = 0;
    nlh->nlmsg_seq = 0;
    nlh->nlmsg_pid = saddr.nl_pid; //self port

    memcpy(NLMSG_DATA(nlh), umsg, strlen(umsg));
    while(1) {
        ret = recvfrom(skfd, &u_info, sizeof(user_msg_info), 0, (struct sockaddr *)&daddr, &len);
        if(!ret)
        {
            perror("recv form kernel error\n");
            close(skfd);
            exit(-1);
        }

        printf("from kernel:%s\n", u_info.msg);
        char buf[1024] = {0};
        snprintf(buf, 1023, "%s nodeid=%s",u_info.msg, argv[2]);
        write(out_fd, buf, strlen(buf));
    //usleep(1000);
    loop_count++;
    }
    close(skfd);

    free((void *)nlh);
    return 0;
}
