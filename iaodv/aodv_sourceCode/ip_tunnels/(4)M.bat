#���豸���¹��أ�ʹ������޸ĵ�Ȩ��
adb -s "Medfield4154E1D7" shell mount -o remount rw /system

#��change.sh������Ӧ���豸�У�����������޸��豸�����ֵģ���Ҫ����adb shell����
adb -s "Medfield4154E1D7" push E:\Coding\iaodv\change4.sh /system/change.sh

#��adhoc-config-intel.sh�ű��Ž���Ӧ�豸���ýű�����������豸��adhocģʽ��
adb -s "Medfield4154E1D7" push E:\Coding\iaodv\adhoc-config-intel4.sh /system/adhoc-config-intel.sh

adb -s "Medfield4154E1D7" push E:\Coding\iaodv\fbaodv_intel.ko /system/fbaodv_intel.ko

#�����豸�е�change.sh�ű����޸��豸����
adb -s "Medfield4154E1D7" shell sh /system/change.sh

#�������������exit�˳���Ȼ����������������ָ��Ľ����
adb kill-server
adb devices

#����豸�е�DTN��ص��ļ������ֻ�ǵ���AODV���Բ���Ҫ���
adb -s "med-192.168.1.4" shell rm /sdcard/dtn/storage/*
adb -s "med-192.168.1.4" shell rm /sdcard/dtn_test_data/*

#����adhoc-config-intel.sh�ű�����ɶ�
adb -s "med-192.168.1.4" shell sh /system/adhoc-config-intel.sh 192.168.1.4

#�����ж��aodvģ�飬����DTN����������ʱ�����Լ�����Σ���������ʵ��ɾ����μ��ɡ�
#adb -s "med-192.168.1.4" shell rmmod fbaodv_intel

pause
