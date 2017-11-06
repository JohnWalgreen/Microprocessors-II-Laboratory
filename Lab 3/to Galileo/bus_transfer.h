#ifndef BUS_TRANSFER_H
#define BUS_TRANSFER_H

/*Assuming that the pins have already been set as outputs, value is put on the data bus (bus[0 - 4]) using GPIO functions*/
void writeBus(int value, int *bus);

/*Assuming that pins are inputs, data bus specified by bus array is read and value is returned*/
int readBus(int *bus);

#endif
