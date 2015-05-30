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

#define LIGHT_PIPE "/tmp/lightpipe"
#define SPEED_PIPE "/tmp/speedpipe"
#define TURN_PIPE  "/tmp/turnpipe"
#define TEMP_PIPE  "/tmp/temppipe"
#define MAX_VALUES 10000

//What is written on the logfiles
typedef struct _meteodata {
	struct timeval time;
	int16_t data;
} MeteoData;

enum { TEMP, LIGHT };

//What is received from the arduino
typedef struct request {
	uint16_t type;
	int16_t data;
} Request;

//What we send to the arduino
typedef struct cmd {
	int16_t speed;
	int16_t turn;
} Command;
