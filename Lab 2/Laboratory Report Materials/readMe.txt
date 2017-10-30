Readme file

Part 1)

Files needed for MPlabs IDE(taken from PIC16F18857 Project.X folder):

1) main.c
2) Queue.c
3) Queue.h
4) adc_led.c
5) adc_led.h
6) gpio.c
7) gpio.h
8) pwm.c
9) pwm.h

Use all of this selected files in the MPlabs interface along with all of the skeleton code for lab1.
main.c will overwrite the test code that is located in main.c


Part 2)



Files needed for Galileo board, all files located in the Galileo folder:

All of these files must be uploaded to a usb-drive or located in the same directory when compiling.

1) bus_transfer.c
2) bus_transfer.h
3) gpio.c
4) gpio.h
5) main.c
6) makefile


Part 3)


If all Galileo files are put to a usb device this can be performed by mounting the usb and then going into the directory with the files.
Our group uses the commands of: root@galileo:~# mount /dev/sda1 /media/usb-drive
				root@galileo:~# cd /media/usb-drive
				root@galileo:~/media/usb-drive# make start

This will run our makefile that will compile all of the files at once then run the program.


Part 4)

The user can enter the command with 7 different options(0-5 and -1)

0) will reset the pic microcontroller
1) will ping the pic microcontroller
2) will return an ADC value from the pic microcontroller
3) will turn the servo motor 30 degrees
4) will turn the servo motor 90 degrees
5) will turn the servo motor 120 degrees
-1) will exit the program

Can restart the program if in the /media/usb-device menu by running make start again
				
