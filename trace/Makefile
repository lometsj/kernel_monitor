CONFIG_MODULE_SIG=n

obj-m := transfer.o probe.o execve.o kill.o open.o shell.o vul.o
EXTRA_CFLAGS := -g

KDIR := /lib/modules/$(shell uname -r)/build
all:
	make -C $(KDIR) M=$(shell pwd) modules
clean:
	rm -f *.ko *.o *.mod.o *.mod.c .*.cmd *.symvers  modul*

