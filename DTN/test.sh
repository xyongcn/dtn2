#!/bin/bash

stop network-manager



	iw dev wlan0 del


iw phy phy0 interface add adhoc0 type ibss



	ifconfig adhoc0 up



	iw dev adhoc0 ibss join wujingbang 2412


ifconfig adhoc0 192.168.5.11
ifconfig adhoc0








