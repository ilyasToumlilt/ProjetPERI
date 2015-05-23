#define _XOPEN_SOURCE 777

#include <stdlib.h>
#include <stdio.h>   
#include <sys/types.h>
#include <sys/stat.h>
#include <stdint.h>       
#include <fcntl.h>
#include <pthread.h>
#include <RF24/RF24.h>
#include <unistd.h>
#include <sys/select.h>
#include <time.h>
#include <signal.h>

typedef struct _meteodata {
	struct timeval time;
	float data;
} MeteoData;

enum { TEMP, LIGHT };

typedef struct request {
	uint16_t type;
	float data;
} Request;

