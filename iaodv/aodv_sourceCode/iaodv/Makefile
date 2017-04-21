#AODV for kernel 3.4

#FLAGS
#DEBUG: show debug messages
#NOMIPS: used for non-based mipsel systems

#intel_arch="i386"
#intel_compile="i686-android-linux-"

#panda_arch="arm"
#panda_compile="arm-none-linux-gnueabi-"

#ARCH		="i386"
#CROSS_COMPILE	="i686-android-linux-"

CC 		= $(CROSS_COMPILE)gcc
AS		= $(CROSS_COMPILE)as
LD		= $(CROSS_COMPILE)ld
CPP		= $(CC) -E
AR		= $(CROSS_COMPILE)ar
NM		= $(CROSS_COMPILE)nm
STRIP		= $(CROSS_COMPILE)strip
OBJCOPY		= $(CROSS_COMPILE)objcopy
OBJDUMP		= $(CROSS_COMPILE)objdump

#obj-m := fbaodv.o
#fbaodv-objs := fbaodv_protocol.o aodv_dev.o aodv_neigh.o aodv_route.o aodv_thread.o hello.o packet_in.o packet_out.o rerr.o rrep.o rreq.o socket.o task_queue.o timer_queue.o utils.o gw_list.o rpdb.o src_list.o aodv_neigh_2h.o route_alg.o st_rreq.o tos.o packet_queue.o brk_list.o rcvp.o aodv_config.o tcnp.o app_request_queue.o

#obj-m := fbaodv_panda.o
fbaodv_panda-objs := fbaodv_protocol.o aodv_dev.o aodv_neigh.o aodv_route.o aodv_thread.o hello.o packet_in.o packet_out.o rerr.o rrep.o rreq.o socket.o task_queue.o timer_queue.o utils.o gw_list.o rpdb.o src_list.o aodv_neigh_2h.o route_alg.o st_rreq.o tos.o packet_queue.o brk_list.o rcvp.o aodv_config.o tcnp.o app_request_queue.o dis_history.o
#obj-m := fbaodv_intel.o
fbaodv_intel-objs := fbaodv_protocol.o aodv_dev.o aodv_neigh.o aodv_route.o aodv_thread.o hello.o packet_in.o packet_out.o rerr.o rrep.o rreq.o socket.o task_queue.o timer_queue.o utils.o gw_list.o rpdb.o src_list.o aodv_neigh_2h.o route_alg.o st_rreq.o tos.o packet_queue.o brk_list.o rcvp.o aodv_config.o tcnp.o app_request_queue.o dis_history.o
#nexus5
fbaodv_nexus-objs := fbaodv_protocol.o aodv_dev.o aodv_neigh.o aodv_route.o aodv_thread.o hello.o packet_in.o packet_out.o rerr.o rrep.o rreq.o socket.o task_queue.o timer_queue.o utils.o gw_list.o rpdb.o src_list.o aodv_neigh_2h.o route_alg.o st_rreq.o tos.o packet_queue.o brk_list.o rcvp.o aodv_config.o tcnp.o app_request_queue.o dis_history.o
#ubuntu
fbaodv_ubuntu-objs := fbaodv_protocol.o aodv_dev.o aodv_neigh.o aodv_route.o aodv_thread.o hello.o packet_in.o packet_out.o rerr.o rrep.o rreq.o socket.o task_queue.o timer_queue.o utils.o gw_list.o rpdb.o src_list.o aodv_neigh_2h.o route_alg.o st_rreq.o tos.o packet_queue.o brk_list.o rcvp.o aodv_config.o tcnp.o app_request_queue.o dis_history.o

#panda_KDIR :=/home/wwtao/Desktop/compiled-kernel/pandaboard_kernel/linaro-kernel/
#intel_KDIR :=/home/wwtao/Desktop/compiled-kernel/linux-2.6
panda_KDIR :=/home/cai/exp/pandaboard_kernel/linaro-kernel
intel_KDIR :=/home/cai/exp/linux-2.6
nexus_KDIR :=/home/cai/exp/nexus_kernel
#ubuntu_KDIR :=/usr/src/kernel/linux-3.5
ubuntu_KDIR :=/usr/src/linux-headers-3.2.0-120-generic


PWD := $(shell pwd)

EXTRA_CFLAGS += -DNOMIPS -fno-pic

default:
	#export ARCH=i386
	#export CROSS_COMPILE=i686-android-linux-

	$(MAKE)	-C $(ubuntu_KDIR) SUBDIRS=$(PWD) obj-m=fbaodv_ubuntu.o ARCH=x86 modules
	rm *.o
	rm *.mod.c

pandaboard:
	$(MAKE)	-C $(panda_KDIR) SUBDIRS=$(PWD) obj-m=fbaodv_panda.o ARCH=arm CROSS_COMPILE=arm-none-linux-gnueabi- modules
	#mv ./fbaodv.ko ./fbaodv_panda.ko

intel:
	$(MAKE)	-C $(intel_KDIR) SUBDIRS=$(PWD) obj-m=fbaodv_intel.o ARCH=i386 CROSS_COMPILE=i686-android-linux- modules 

nexus:
	$(MAKE)	-C $(nexus_KDIR) SUBDIRS=$(PWD) obj-m=fbaodv_nexus.o ARCH=arm CROSS_COMPILE=arm-none-linux-gnueabi- modules
ubuntu:
	$(MAKE)	-C $(ubuntu_KDIR) SUBDIRS=$(PWD) obj-m=fbaodv_ubuntu.o ARCH=i386 modules
clean:
	#export ARCH=""
	#export CROSS_COMPILE=""
	rm *.o *.ko 
	rm *.mod.c
