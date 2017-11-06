start:main.c gpio.c bus_transfer.c
	cd /media/usb-drive
	gcc -Wall main.c gpio.c bus_transfer.c -o gal
	./gal
	grep out /sys/class/gpio/gpio*/direction
	grep in /sys/class/gpio/gpio*/direction

now: main.c i2c.c
	gcc -Wall main.c i2c.c -o ./gal.out
