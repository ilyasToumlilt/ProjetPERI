#include "daemon.h"

RF24 radio(22,0);//RPI_V2_GPIO_P1_15, RPI_V2_GPIO_P1_24, BCM2835_SPI_SPEED_8MHZ);

uint8_t addresses[][6] = {"motor","meteo"};
int fdspeed, fdturn, fdtemp, fdlight;
int templog, lightlog;
pthread_mutex_t radio_mutex = PTHREAD_MUTEX_INITIALIZER;


/*Close all files when SIGINT*/
void handler(int sig) {
	//Close all log files
	if (close(templog) == -1) {
		perror("close templog");
		exit(EXIT_FAILURE);
	}

	if (close(lightlog) == -1) {
		perror("close lightlog");
		exit(EXIT_FAILURE);
	}
	
	//Close all fifo descriptors
	if (close(fdspeed) == -1) {
		perror("close fdspeed");
		exit(EXIT_FAILURE);
	}

	if (close(fdturn) == -1) {
		perror("close fdturn");
		exit(EXIT_FAILURE);
	}
	if (close(fdtemp) == -1) {
		perror("close fdtemp");
		exit(EXIT_FAILURE);
	}
	if (close(fdlight) == -1) {
		perror("close fdlight");
		exit(EXIT_FAILURE);
	}

	//Unlink fifo
	if (unlink("/tmp/lightpipe") == -1) {
		perror("close lightpipe");
		exit(EXIT_FAILURE);
	}

	if (unlink("/tmp/speedpipe") == -1) {
		perror("close speedpipe");
		exit(EXIT_FAILURE);
	}

	if (unlink("/tmp/turnpipe") == -1) {
		perror("close turnpipe");
		exit(EXIT_FAILURE);
	}

	if (unlink("/tmp/temppipe") == -1) {
		perror("close temppipe");
		exit(EXIT_FAILURE);
	}

	printf("All files closed");
	exit(EXIT_SUCCESS);
}

/*Returns random number between min and max*/
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
	MeteoData d;
	Request req;
	int16_t l=0,t=0;
	//Open temperature logfile
	if((templog=open(filenames[0], O_WRONLY | O_CREAT | O_APPEND))==-1){
		perror("open (templogfile)");
		exit(EXIT_FAILURE);
	}

	//Open light logfile
	if((lightlog=open(filenames[1], O_WRONLY | O_CREAT | O_APPEND))==-1){
		perror("open (lightlogfile)");
		exit(EXIT_FAILURE);
	}

	while(1){
		//if there are bytes to be read
		if(radio.available()){
			//Get data from arduino
			pthread_mutex_lock(&radio_mutex);
			radio.startListening();
			radio.read(&req, sizeof(Request));
			radio.stopListening();
			pthread_mutex_unlock(&radio_mutex);

			printf("Data received from arduino : %s %f\n", 
			       req.type==TEMP ? "TEMP" : "LIGHT", req.data);

			//Fill the fields of MeteoData
			d.data=(int16_t)req.data;
			gettimeofday(&(d.time),NULL);

			//Write in the right file
			switch(req.type){
			case TEMP:
				//Temperature data
				if(write(templog, &d, sizeof(MeteoData))!=sizeof(MeteoData)){
					perror("write (temp)");
					exit(EXIT_FAILURE);
				}

				//Send to server
				if(write(fdtemp, &(d.data), sizeof(int16_t))!=sizeof(int16_t)){
					perror("Meteo pipe");
					exit(EXIT_FAILURE);
				}
				break;

			case LIGHT: 
				//Light data
				if(write(lightlog, &d, sizeof(MeteoData))!=sizeof(MeteoData)){
					perror("write (light)");
					exit(EXIT_FAILURE);
				}

				//Send to server
				if(write(fdlight, &(d.data), sizeof(MeteoData))!=sizeof(int16_t)){
					perror("Meteo pipe");
					exit(EXIT_FAILURE);
				}
				break;
			}

			printf("Data sent successfully !\n");

		}else{

			/*Writing light*/
			if (write(fdlight, &l, sizeof(int16_t)) == -1) {
				perror("write lightpipe");
				exit(EXIT_FAILURE);
			}

			printf("Sending lightpipe : %d\n", l);



			/*Writing temperature*/
			if (write(fdtemp, &t, sizeof(int16_t)) == -1) {
				perror("write temppipe");
				exit(EXIT_FAILURE);
			}

			printf("Sending temppipe : %d\n", t);
			t=(t+1)==101?0:t+1;
			l=(l+1)==101?0:l+1;
			usleep(100000);
		}
	}

	return NULL;
}

int main (int argc, char ** argv){
	pthread_t tid;
	fd_set active_fd;
	int fdmax;
	int16_t speed = 50, turn = 50;

	/*Sigaction to catch SIGINT*/
	struct sigaction sigact;
	sigact.sa_handler = handler;
	sigaction(SIGINT, &sigact, NULL);


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
	//	radio.openWritingPipe(addresses[0]);
	radio.openReadingPipe(1,0x000000000001LL);

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

		//If there are data in fdturn or fdspeed
		if (FD_ISSET (fdturn, &active_fd)){
			if (read(fdturn, &turn, sizeof(int16_t)) < 0){
				perror("read");
				exit(EXIT_FAILURE);
			}
			printf("New value for turn : %d\n", turn);
		}
		if (FD_ISSET (fdspeed, &active_fd)){
			if (read(fdspeed, &speed, sizeof(int16_t)) < 0){
				perror("read");
				exit(EXIT_FAILURE);
			}
			printf("New value for speed : %d\n", speed);
		}
	
		//Sends the speed and turn data
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
		printf("Speed and turn values successfully sent to arduino\n");
	}
	
	return EXIT_SUCCESS;
}
