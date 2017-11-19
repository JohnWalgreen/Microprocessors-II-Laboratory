main: main.c i2c.c pic.c
	gcc -I/usr/local/include/opencv -I/usr/local/include/opencv2 -L/usr/local/lib/ -Wall main.c i2c.c pic.c -o ./gal.out -lm -lopencv_core -lopencv_imgproc -lopencv_highgui -lopencv_ml -lopencv_video -lopencv_features2d -lopencv_calib3d -lopencv_objdetect -lopencv_contrib -lopencv_legacy -lopencv_stitching

clean:
	$(RM) ./gal.out
