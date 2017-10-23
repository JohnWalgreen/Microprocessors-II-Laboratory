/*Galileo Derek Teixeira
Commands for PING, RESET, get ADC, Turn Motor Blade
Date 10/22/2017 time 16:30
*/
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

int main()
{
int fd;               //file variable
char buf[256];
int gpio = 54;        //Pin A0

//simple command to enable pin A0

fd = open("/sys/class/gpio/export", O_WRONLY);

sprintf(buf, "%d", gpio);

write(fd, buf, strlen(buf));

close(fd);

sprintf(buf, "/sys/class/gpio/gpio%d/direction", gpio);

fd = open(buf, O_WRONLY);

// Set out direction
write(fd, "out", 3);
// Set in direction


close(fd);


sprintf(buf, "/sys/class/gpio/gpio%d/value", gpio);

fd = open(buf, O_WRONLY);

// Set GPIO high status
write(fd, "1", 1);
// Set GPIO low status


close(fd);
}
