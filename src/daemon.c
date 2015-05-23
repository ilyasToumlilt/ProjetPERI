#include "daemon.h"


RF24 radio(RPI_V2_GPIO_P1_15, RPI_V2_GPIO_P1_24, BCM2835_SPI_SPEED_8MHZ);
uint8_t addresses[][6] = {"motor","meteo"};
int fdspeed, fdturn, fdtemp, fdlight;
pthread_mutex_t radio_mutex = PTHREAD_MUTEX_INITIALIZER;

int random(int min, int max) {
	int low_num=0, hi_num=0;

	if(min < max) {
		 low_num = min;
		 hi_num = max +1;
    	}
	else {
		 low_num = max + 1;
		 hi_num = min;
   	 }

	struct timespec ts;
	clock_gettime(CLOCK_MONOTONIC, &ts);

	 /* using nano-seconds instead of seconds */
	 srand((time_t)ts.tv_nsec);

	 return (rand()%(hi_num-low_num)) + low_num;
}

void * logthread(void * args){
	char ** filenames = (char **) args;
	int tempfd, lightfd;
	MeteoData d;
	Request req;

	//Open temperature logfile
	if((tempfd=open(filenames[0], O_WRONLY | O_CREAT | O_APPEND))==-1){
		perror("open (templogfile)");
		exit(EXIT_FAILURE);
	}

	//Open light logfile
	if((lightfd=open(filenames[1], O_WRONLY | O_CREAT | O_APPEND))==-1){
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
			int r = random(0, 100);

			if (write(fdlight, &r, sizeof(int)) == -1) {
				perror("write lightpipe");
				exit(EXIT_FAILURE);
			}

			printf("Sending lightpipe : %d\n", r);
			usleep(100000);
		}
	}

	return NULL;
}

int main (int argc, char ** argv){
	pthread_t tid;
	fd_set active_fd;
	int data;
	int fdmax;
	int16_t speed = 50, turn = 50;

	//Check arguments
	if(argc!=3){
		fprintf(stderr,"Usage : %s <templogfile> <lightlogfile>\n",argv[0]);
		exit(EXIT_FAILURE);
	}

	//Open command pipe
	if(mkfifo("/tmp/speedpipe", 0766)==-1){
		perror("mkfifo (speedpipe)");
		exit(EXIT_FAILURE);
	}

	if((fdspeed=open("/tmp/speedpipe", O_RDWR | O_TRUNC))==-1){
		perror("open (speedpipe)");
		exit(EXIT_FAILURE);
	}

	if(mkfifo("/tmp/turnpipe", 0766)==-1){
		perror("mkfifo (turnpipe)");
		exit(EXIT_FAILURE);
	}

	if((fdturn=open("/tmp/turnpipe", O_RDWR | O_TRUNC))==-1){
		perror("open (turnpipe)");
		exit(EXIT_FAILURE);
	}


	//Open meteo pipe

	if(mkfifo("/tmp/temppipe", 0766)==-1){
		perror("mkfifo (temppipe)");
		exit(EXIT_FAILURE);
	}

	if((fdtemp=open("/tmp/temppipe", O_RDWR | O_TRUNC))==-1){
		perror("open (temppipe)");
		exit(EXIT_FAILURE);
	}

	if(mkfifo("/tmp/lightpipe", 0766)==-1){
		perror("mkfifo (lightpipe)");
		exit(EXIT_FAILURE);
	}

	if((fdlight=open("/tmp/lightpipe", O_RDWR | O_TRUNC))==-1){
		perror("open (lightpipe)");
		exit(EXIT_FAILURE);
	}

	fdmax = (fdspeed<fdturn) ? (fdturn+1) : (fdspeed+1);
	
	//FD set
	FD_ZERO(&active_fd);
	FD_SET(fdspeed, &active_fd);
	FD_SET(fdturn, &active_fd);


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
		FD_ZERO(&active_fd);
		FD_SET(fdspeed, &active_fd);
		FD_SET(fdturn, &active_fd);
		if (select(fdmax, &active_fd, NULL, NULL, NULL) < 0){
			perror ("select");
			exit (EXIT_FAILURE);
	        }

		if (FD_ISSET (fdturn, &active_fd)){
			if (read(fdturn, &data, sizeof(int16_t)) < 0){
				perror("read");
				exit(EXIT_FAILURE);
			}

		}
		if (FD_ISSET (fdspeed, &active_fd)){
			if (read(fdspeed, &data, sizeof(int16_t)) < 0){
				perror("read");
				exit(EXIT_FAILURE);
			}
		}
	
		pthread_mutex_lock(&radio_mutex);
		if (!radio.write(&speed, sizeof(int16_t))){
			perror("radio write");
			exit(EXIT_FAILURE);
		}

		if (!radio.write(&turn, sizeof(int16_t))){
			perror("radio write");
			exit(EXIT_FAILURE);
		}
		pthread_mutex_unlock(&radio_mutex);

	}
	
	return EXIT_SUCCESS;
}
