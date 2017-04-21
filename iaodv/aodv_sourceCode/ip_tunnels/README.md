#pandaboard的IP隧道模块ipip.ko编译流程#

###准备工作###

（1）ubuntu64位的系统，安装交叉编译器arm-none-linux-gneabi-gcc。下载交叉编译器：arm-2010q1-202-arm-none-linux-gnueabi-i686-pc-linux-gnu.tar.bz2

    先将编译器解压缩，然后将编译器里面的bin目录添加到 ~/.bashrc 文件的最后一行。
    交叉编译器目录为<arm-crosscompile> ，那么就运行命令

gedit ~/.bashrc
 
export PATH=$PATH:/<arm-crosscompile>/bin
	
	这样就可以完成交叉编译器安装了。由于是64位系统，需要单独安装另一些库才可以运行该编译器，输入命令：
sudo apt-get install libc6:i386 libgcc1:i386 gcc-4.6-base:i386 libstdc++5:i386 libstdc++6:i386

    这样就可以运行32位的交叉编译器了。可以重新打开一个控制台，输入arm-none-然后按TAB键看是否补全来检验一下。
（2）下载pandaboard内核：

wget http://releases.linaro.org/12.07/android/images/panda-ics-gcc47-tilt-stable-blob/linaro_kernel_build_cmds.sh

wget http://releases.linaro.org/12.07/android/images/panda-ics-gcc47-tilt-stable-blob/kernel_config

chmod +x linaro_kernel_build_cmds.sh

./linaro_kernel_build_cmds.sh

	运行到最后一步会报错，这是因为linaro_kernel_build_cmds.sh里面设置的交叉编译器与安装的不符合，不过这不要紧。接下来将下载的kernel_config文件拷贝到linaro-kernel里面，运行命令：

cp kernel_config ./linaro-kernel/.config

cd linaro-kernel

（3）设置ARCH=目标体系结构

export ARCH=arm

（4）设置交叉编译器

export CROSS_COMPILE=arm-none-linux-gnueabi-

（5）在ipip.c的目录下写Makefile

 在linux根目录下

find ./ -name ipip.c

进入ipip.c的目录，在当前目录下写Makefile，内容如下：

gedit Makefile

      #
      # Makefile for the pandaboard ip to ip tunnel.(by: ywl)
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

（6）在当前目录下运行make命令

make

（7）把生成的模块ipip.ko加载到pandaboard的system/modules目录（可使用adb）

adb -s "0123456789ABCDEF" push C:\Users\ZhangChi\Desktop\20150501\ipip\ipip.ko /system/modules/ipip.ko

（8）在pandaboard中加载模块ipip.ko

insmod /system/modules/ipip.ko

（9）通过adb运行脚本,刷入相关模块及脚本

注意：修改(1)M.bat、(2)P.bat、(3)P.bat、(4)M.bat脚本中相关文件在本机的路径

USB连接Intel手机1 运行脚本(1)M.bat

USB连接Intel手机4 运行脚本(4)M.bat

USB连接pandaboard2 连接有线（校园网） 运行脚本(2)P.bat

USB连接pandaboard3 连接有线（校园网） 运行脚本(3)P.bat

（10）按脚本panA和panB配置pandaboard的IP隧道(adb shell)
 
按以下命令在两台pandaboard上配置
	  
**#panda2**

insmod /system/modules/ipip.ko

ip tunnel add tun1 mode ipip remote *10.108.20.50* local *10.108.20.174* ttl 64

ip addr add *192.168.1.2* peer *192.168.1.3* dev tun1

ip link set tun1 up

**#panda3**

insmod /system/modules/ipip.ko

ip tunnel add tun1 mode ipip remote *10.108.20.174* local *10.108.20.50* ttl 64

ip addr add *192.168.1.3* peer *192.168.1.2* dev tun1

ip link set tun1 up


    检查与关闭隧道命令

    ip link set ${TUN_NAME} down
    ip tunnel del ${TUN_NAME}

注意：

10.108.20.174是panda2的有线IP 即eth0 (netcfg eth0 up dhcp) 192.168.1.2是panda2的虚拟IP 即tun1 

10.108.20.50 是panda3的有线eth0的IP 192.168.1.3是panda3的虚拟IP 即tun1

先建立IP隧道再启用adhoc 使panda2的adhoc IP=panda2 tun1 IP，使panda3的adhoc IP=panda3 tun1 IP

	如图1,4可通  
	  1----2——————3-----4
	1(Intel1)：adhoc0 192.168.1.1 black list 192.168.1.3,192.168.1.4
	2(panda2): eth0 10.108.20.174 tun1 192.168.1.2 adhoc0 192.168.1.2 black list 192.168.1.4
	3(panda3): eth0 10.108.20.50 tun1 192.168.1.3 adhoc0 192.168.1.3 black list 192.168.1.1
	4(Intel4)：adhoc0 192.168.1.4 black list 192.168.1.1,192.168.1.2

（11）打开pandaboard的adhoc模式（adb shell）

sh /system/adhoc-config-panda.sh 192.168.1.2

sh /system/adhoc-config-panda.sh 192.168.1.3


	
