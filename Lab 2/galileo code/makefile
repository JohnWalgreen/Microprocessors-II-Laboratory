start:testing.c
	mount /dev/sdal /media/usb-drive
	cd /media/usb-drive
	gcc testing.c -o gal
	./gal
	grep out /sys/class/gpio/gpio*/direction

end:
	umount /media/usb-drive
