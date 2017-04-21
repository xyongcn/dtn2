#将设备重新挂载，使其可以修改的权限
adb -s "0123456789ABCDEF" shell mount -o remount rw /system

#将ipip.ko传到相应的设备中
adb -s "0123456789ABCDEF" push C:\Users\ZhangChi\Desktop\20150501\ipip\ipip.ko /system/modules/ipip.ko
adb -s "0123456789ABCDEF" push C:\Users\ZhangChi\Desktop\20150501\ipip\gre.ko /system/modules/gre.ko
#adb -s "0123456789ABCDEF" push C:\Users\ZhangChi\Desktop\20150501\ipip\tunnel4.ko /system/modules/tunnel4.ko
#adb -s "pan-192.168.1.2" push C:\Users\ZhangChi\Desktop\20150501\ipip\ipip.ko /system/modules/ipip.ko

#将change.sh传到相应的设备中，这个是用来修改设备的名字的，需要重启adb shell才行
adb -s "0123456789ABCDEF" push C:\Users\ZhangChi\Desktop\20150501\ywl\changep2.sh /system/change.sh
#adb -s "pan-192.168.1.2" push C:\Users\ZhangChi\Desktop\20150501\ywl\changep2.sh /system/change.sh

#启动有线网(校园网)DHCP
adb -s "0123456789ABCDEF" push C:\Users\ZhangChi\Desktop\20150501\ywl\pandaboard\eth-8.sh /system/eth-up.sh
#adb -s "pan-192.168.1.2" push C:\Users\ZhangChi\Desktop\20150501\ywl\pandaboard\eth-8.sh /system/eth-up.sh

#把adhoc-config-panda.sh脚本放进相应设备，该脚本是用来完成设备的adhoc模式打开
adb -s "0123456789ABCDEF" push C:\Users\ZhangChi\Desktop\20150501\ywl\adhoc-config-panda2.sh /system/adhoc-config-panda.sh
#adb -s "pan-192.168.1.2" push C:\Users\ZhangChi\Desktop\20150501\ywl\adhoc-config-panda2.sh /system/adhoc-config-panda.sh

adb -s "0123456789ABCDEF" push C:\Users\ZhangChi\Desktop\20150501\ywl\fbaodv_panda.ko /system/fbaodv_panda.ko
#adb -s "pan-192.168.1.2" push C:\Users\ZhangChi\Desktop\20150501\ywl\fbaodv_panda.ko /system/fbaodv_panda.ko

#运行设备中的change.sh脚本来修改设备名字

adb -s "0123456789ABCDEF" shell sh /system/change.sh
#开启有线网卡并设置ip和掩码
adb -s "0123456789ABCDEF" shell sh /system/eth-up.sh
#运行上述命令后exit退出，然后运行以下命令看名字更改结果。
adb kill-server
adb devices

#清空设备中的DTN相关的文件，如果只是调试AODV可以不需要这段
adb -s "pan-192.168.1.2" shell rm /sdcard/dtn/storage/*
adb -s "pan-192.168.1.2" shell rm /sdcard/dtn_test_data/*

#启动adhoc-config-panda.sh脚本，完成对
#adb -s "pan-192.168.1.2" shell sh /system/adhoc-config-panda.sh 192.168.1.2
#adb -s "0123456789ABCDEF" shell sh /system/adhoc-config-panda.sh 192.168.1.2
#这个是卸载aodv模块，对于DTN处单跳试验时，可以加上这段，后续多跳实验删掉这段即可。
#adb -s "pan-192.168.1.2" shell rmmod fbaodv_panda

#
adb -s "pan-192.168.1.2" shell insmod /system/modules/ipip.ko
adb -s "pan-192.168.1.2" shell insmod /system/modules/gre.ko
adb -s "pan-192.168.1.2" shell netcfg eth0 up dhcp
#pause
adb -s "pan-192.168.1.2" shell
