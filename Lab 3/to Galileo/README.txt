Source files with main functions:
	main.c		:	Original source file to be compiled with 
				"make main".  Use for demo.
	acquire.c	:	Used to collect temperature data to be used 
				for processing and further sensor development.  
				Compile with "make acquire".
	take picture.c	:	Used as test file for capturing images.  
				Compile with "make take_picture_c".
	fun_version.c	:	Same as main file, except this file contains 
				insults and swears to make the user experience 
				more fun.  Compile with "make fun_version".

Other source files:
	i2c.c		: 	Contains functions used to communicate with 
				temperature sensor.

Header files:
	i2c.h		:	Header functions for i2c.c.
