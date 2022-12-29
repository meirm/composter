
#ifndef _COMPOSTER_CONFIG
#define _COMPOSTER_CONFIG

#define _esp32 1
#define _arduino 2
#define _arch 2
#define YES 1
#define NO 0

#define USESDCARD NO
// ESP
if _arch == 1
#define IO 41
#define SCLK 15
#define CE 32
#fi
if _arch == 2
//ARDUINO
#define IO 4
#define SCLK 5
#define CE 2


int thermistorPin = 7;
int thermistorOut = A1;
int refres = 10000;
float nominalres = 100000.0;
int nominaltemp = 25;
int bcoef = 3950;


#endif
