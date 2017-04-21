#将设备重新挂载，使其可以修改的权限
adb -s "Medfield3FC1ECA3" shell mount -o remount rw /system

#将change.sh传到相应的设备中，这个是用来修改设备的名字的，需要重启adbshell才行
adb -s "Medfield3FC1ECA3" push C:\Users\ZhangChi\Desktop\20150501\ywl\change1.sh /system/change.sh

#把adhoc-config-intel.sh脚本放进相应设备，该脚本是用来完成设备的adhoc模式打开
adb -s "Medfield3FC1ECA3" push C:\Users\ZhangChi\Desktop\20150501\ywl\adhoc-config-intel1.sh /system/adhoc-config-intel.sh

adb -s "Medfield3FC1ECA3" push C:\Users\ZhangChi\Desktop\20150501\ywl\fbaodv_intel.ko /system/fbaodv_intel.ko

#运行设备中的change.sh脚本来修改设备名字
adb -s "Medfield3FC1ECA3" shell sh /system/change.sh

#运行上述命令后exit退出，然后运行以下命令看名字更改结果。
adb kill-server
adb devices

#清空设备中的DTN相关的文件，如果只是调试AODV可以不需要这段
adb -s "med-192.168.1.1" shell rm /sdcard/dtn/storage/*
adb -s "med-192.168.1.1" shell rm /sdcard/dtn_test_data/*

#启动adhoc-config-intel.sh脚本，完成对
adb -s "med-192.168.1.1" shell sh /system/adhoc-config-intel.sh 192.168.1.1

#这个是卸载aodv模块，对于DTN处单跳试验时，可以加上这段，后续多跳实验删掉这段即可。
#adb -s "med-192.168.1.1" shell rmmod fbaodv_intel

pause
