#include "daemon.h"


RF24 radio(RPI_V2_GPIO_P1_15, RPI_V2_GPIO_P1_24, BCM2835_SPI_SPEED_8MHZ);
uint8_t addresses[][6] = {"motor","meteo"};
int fdspeed, fdturn, fdtemp, fdlight;
pthread_mutex_t radio_mutex = PTHREAD_MUTEX_INITIALIZER;

void * logthread(void * args){
	char ** filenames = (char **) args;
	int tempfd, lightfd;
	MeteoData d;
	Request req;

	//Open temperature logfile
	if((tempfd=open(filenames[0], 0_WRONLY | O_CREAT | O_APPEND))!=0){
		perror("open (templogfile)");
		exit(EXIT_FAILURE);
	}

	//Open light logfile
	if((lightfd=open(filenames[1],0_WRONLY | O_CREAT | O_APPEND))!=0){
		perror("open (lightlogfile)");
		exit(EXIT_FAILURE);
	}

	while(1){
		if(radio.available()){
			//Get data from arduino
			pthread_mutex_lock(&radio_mutex);
			radio.startListening();
			radio.read(&req, sizeof(Request));
			radio.stopListening();
			pthread_mutex_unlock(&radio_mutex);
			
			//Fill the fields of MeteoData
			d.data=req.data;
			gettimeofday(&(d.time),NULL);

			//Write in the right file
			switch(req.type){
			case TEMP:
				if(write(tempfd, &d, sizeof(MeteoData))!=sizeof(MeteoData)){
					perror("write (temp)");
					exit(EXIT_FAILURE);
				}

				//send to server
				if(write(fdtemp, &d, sizeof(MeteoData))!=sizeof(MeteoData)){
					perror("Meteo pipe");
					exit(EXIT_FAILURE);
				}
				break;

			case LIGHT: 
				if(write(lightfd, &d, sizeof(MeteoData))!=sizeof(MeteoData)){
					perror("write (light)");
					exit(EXIT_FAILURE);
				}

				//send to server
				if(write(fdlight, &d, sizeof(MeteoData))!=sizeof(MeteoData)){
					perror("Meteo pipe");
					exit(EXIT_FAILURE);
				}
				break;
			}



		}else{
			usleep(1000);
		}
	}

	return NULL;
}

int main (int argc, char ** argv){
	pthread_t tid;
	
	//Check arguments
	if(argc!=3){
		fprintf(stderr,"Usage : %s <templogfile> <lightlogfile>\n",argv[0]);
		exit(EXIT_FAILURE);
	}

	//Open command pipe
	if((fdspeed=mkfifo("speedpipe", O_RDWR))!=0){
		perror("mkfifo (speedpipe)");
		exit(EXIT_FAILURE);
	}

	if((fdturn=mkfifo("turnpipe", O_RDWR))!=0){
		perror("mkfifo (turnpipe)");
		exit(EXIT_FAILURE);
	}

	//Open meteo pipe
	if((fdtemp=mkfifo("temppipe",O_RDWR))!=0){
		perror("mkfifo (temppipe)");
		exit(EXIT_FAILURE);
	}

	if((fdlight=mkfifo("lightpipe",O_RDWR))!=0){
		perror("mkfifo (lightpipe)");
		exit(EXIT_FAILURE);
	}

	//Radio initialization
	radio.begin();
	radio.openWritingPipe(addresses[0]);
	radio.openReadingPipe(1,addresses[1]);

	//Create thread
	if(pthread_create(&tid, NULL, logthread, (void *) (&argv[1]))!=0){
		perror("pthread_create");
		exit(EXIT_FAILURE);
	}

	//Main loop
	while(1){

	}
	return EXIT_SUCCESS;
}
