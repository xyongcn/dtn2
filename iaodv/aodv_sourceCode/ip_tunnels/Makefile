#
# Makefile for the Linux TCP/IP (INET) layer.
#

ifneq ($(KERNELRELEASE),)
#ipip-objs := ipip.o
obj-m := ipip.o
else
KERNELDIR ?= /home/zhangchi/桌面/linaro-kernel
PWD := $(shell pwd)
KVR ?= $(shell uname -r)
KDIR := /home/zhangchi/桌面/linaro-kernel
all:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) modules
clean:
	rm -rf .*.cmd *.o *.mod.c
endif
