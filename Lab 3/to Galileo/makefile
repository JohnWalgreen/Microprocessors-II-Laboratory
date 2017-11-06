start:main.c gpio.c bus_transfer.c
	cd /media/usb-drive
	gcc -Wall main.c gpio.c bus_transfer.c -o gal
	./gal
	grep out /sys/class/gpio/gpio*/direction
	grep in /sys/class/gpio/gpio*/direction

now: main.c i2c.c
	g++ -I/usr/local/include/opencv -I/usr/local/include/opencv2 -L/usr/local/lib/ -Wall "take picture.c" i2c.c -o ./gal.out -lopencv_core -lopencv_imgproc -lopencv_highgui -lopencv_ml -lopencv_video -lopencv_features2d -lopencv_calib3d -lopencv_objdetect -lopencv_contrib -lopencv_legacy -lopencv_stitching

other:
	g++ -Wall "take picture.c" i2c.c -o ./gal.out

now2: main.c i2c.c
	gcc -I/usr/local/include/opencv -I/usr/local/include/opencv2 -L/usr/local/lib/ -Wall "take picture.c" i2c.c -o ./gal.out -lopencv_core -lopencv_imgproc -lopencv_highgui -lopencv_ml -lopencv_video -lopencv_features2d -lopencv_calib3d -lopencv_objdetect -lopencv_contrib -lopencv_legacy -lopencv_stitching
