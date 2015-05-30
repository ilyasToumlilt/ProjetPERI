#include "daemon.h"

RF24 radio(22, 0);

int fdspeed, fdturn, fdtemp, fdlight;
int templog, lightlog;
pthread_mutex_t radio_mutex = PTHREAD_MUTEX_INITIALIZER;
int temp_index = 0;
int light_index = 0;
MeteoData tempvalues[MAX_VALUES];
MeteoData lightvalues[MAX_VALUES];

/*Close all files when SIGINT*/
void handler(int sig) {
	printf("\nWriting datas in files ... ");

	//Write datas in files
	if(tempvalues[temp_index].time.tv_sec==0){
		if(write(templog, tempvalues, temp_index*sizeof(MeteoData))!=(sizeof(MeteoData)*temp_index)){
			perror("Write templogfile");
			exit(EXIT_FAILURE);
		}
	}else{
		if(write(templog, tempvalues, temp_index*sizeof(MeteoData))!=(sizeof(MeteoData)*temp_index)){
			perror("Write templogfile");
			exit(EXIT_FAILURE);
		}
		if(write(templog,
			 tempvalues+temp_index, 
			 (MAX_VALUES-temp_index)*sizeof(MeteoData))
		   !=(sizeof(MeteoData)*(MAX_VALUES-temp_index))){
			perror("Write templogfile");
			exit(EXIT_FAILURE);
		}

	}

	//Write datas in files
	if(lightvalues[light_index].time.tv_sec==0){
		if(write(lightlog, lightvalues, light_index*sizeof(MeteoData))!=(sizeof(MeteoData)*light_index)){
			perror("Write lightlogfile");
			exit(EXIT_FAILURE);
		}
	}else{
		if(write(lightlog, lightvalues, light_index*sizeof(MeteoData))!=(sizeof(MeteoData)*light_index)){
			perror("Write lightlogfile");
			exit(EXIT_FAILURE);
		}
		if(write(lightlog,
			 lightvalues+light_index, 
			 (MAX_VALUES-light_index)*sizeof(MeteoData))
		   !=(sizeof(MeteoData)*(MAX_VALUES-light_index))){
			perror("Write lightlogfile");
			exit(EXIT_FAILURE);
		}

	}

	printf("OK !\n");

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
	if (unlink(LIGHT_PIPE) == -1) {
		perror("close lightpipe");
		exit(EXIT_FAILURE);
	}

	if (unlink(SPEED_PIPE) == -1) {
		perror("close speedpipe");
		exit(EXIT_FAILURE);
	}

	if (unlink(TURN_PIPE) == -1) {
		perror("close turnpipe");
		exit(EXIT_FAILURE);
	}

	if (unlink(TEMP_PIPE) == -1) {
		perror("close temppipe");
		exit(EXIT_FAILURE);
	}

	printf("All files closed");
	exit(EXIT_SUCCESS);
}

void * logthread(void * args){
	char ** filenames = (char **) args;
	MeteoData d;
	Request req;
	sigset_t mask;
	int16_t l=0,t=0;
	struct stat st;

	//Set right mask
	sigemptyset(&mask);
	pthread_sigmask(SIG_SETMASK, &mask, NULL);

	//Get values from files, if there are any
	if(stat(filenames[0],&st)==0){
		int fd;
		if((fd=open(filenames[0],O_RDONLY))==-1){
			perror("open templogfile (read)");
			exit(EXIT_FAILURE);
		}

		if(read(fd,tempvalues, st.st_size)!=st.st_size){
			perror("read templogfile");
			exit(EXIT_FAILURE);
		}

		temp_index=(st.st_size/sizeof(MeteoData))%MAX_VALUES;
	}

	if(stat(filenames[1],&st)==0){
		int fd;
		if((fd=open(filenames[1],O_RDONLY))==-1){
			perror("open lightlogfile (read)");
			exit(EXIT_FAILURE);
		}

		if(read(fd,lightvalues, st.st_size)!=st.st_size){
			perror("read lightlogfile");
			exit(EXIT_FAILURE);
		}

		light_index=(st.st_size/sizeof(MeteoData))%MAX_VALUES;
	}

	//Open temperature logfile
	if((templog=open(filenames[0], O_WRONLY | O_CREAT))==-1){
		perror("open (templogfile)");
		exit(EXIT_FAILURE);
	}

	//Open light logfile
	if((lightlog=open(filenames[1], O_WRONLY | O_CREAT))==-1){
		perror("open (lightlogfile)");
		exit(EXIT_FAILURE);
	}

	while(1){
		//if there are bytes to be read
		pthread_mutex_lock(&radio_mutex);
		if(radio.available()){
			//Get data from arduino
			radio.read(&req, sizeof(Request));
			pthread_mutex_unlock(&radio_mutex);

			printf("Data received from arduino : %s %d\n", 
			       req.type==TEMP ? "TEMP" : "LIGHT", req.data);

			//Fill the fields of MeteoData
			d.data=(int16_t)req.data;
			gettimeofday(&(d.time),NULL);

			//Write in the right file
			switch(req.type){
			case TEMP:
				tempvalues[temp_index]=d;
				temp_index=(temp_index+1)%MAX_VALUES;

				//Send to server
				if(write(fdtemp, &(d.data), sizeof(int16_t))!=sizeof(int16_t)){
					perror("temppipe");
					exit(EXIT_FAILURE);
				}
				break;

			case LIGHT: 
				lightvalues[light_index]=d;
				light_index=(light_index+1)%MAX_VALUES;
				d.data=d.data/10;

				//Send to server
				if(write(fdlight, &(d.data), sizeof(int16_t))!=sizeof(int16_t)){
					perror("lightpipe");
					exit(EXIT_FAILURE);
				}
				break;
			}

			printf("Data sent successfully !\n");

		}else{
			pthread_mutex_unlock(&radio_mutex);
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
	sigset_t mask;

	//Check arguments
	if(argc!=3){
		fprintf(stderr,"Usage : %s <templogfile> <lightlogfile>\n",argv[0]);
		exit(EXIT_FAILURE);
	}

	/*Sigaction to catch SIGINT*/
	struct sigaction sigact;
	sigact.sa_handler = handler;
	sigaction(SIGINT, &sigact, NULL);

	//Blocking all signals
	sigfillset(&mask);
	sigprocmask(SIG_SETMASK, &mask, NULL);

	//Open command pipes
	if(mkfifo(SPEED_PIPE, 0766)==-1){
		perror("mkfifo (speedpipe)");
		exit(EXIT_FAILURE);
	}

	//Chmod the pipe, otherwise the server won't be able to read it
	if(chmod(SPEED_PIPE, 0766)==-1){
	  perror("chmod (speedpipe)");
	  exit(EXIT_FAILURE);
	}
	
	if((fdspeed=open(SPEED_PIPE, O_RDWR | O_TRUNC))==-1){
		perror("open (speedpipe)");
		exit(EXIT_FAILURE);
	}

	if(mkfifo(TURN_PIPE, 0766)==-1){
		perror("mkfifo (turnpipe)");
		exit(EXIT_FAILURE);
	}

	if(chmod(TURN_PIPE, 0766)==-1){
	  perror("chmod (turnpipe)");
	  exit(EXIT_FAILURE);
	}

	if((fdturn=open(TURN_PIPE, O_RDWR | O_TRUNC))==-1){
		perror("open (turnpipe)");
		exit(EXIT_FAILURE);
	}


	//Open meteo pipe
	if(mkfifo(TEMP_PIPE, 0766)==-1){
		perror("mkfifo (temppipe)");
		exit(EXIT_FAILURE);
	}

	if((fdtemp=open(TEMP_PIPE, O_RDWR | O_TRUNC))==-1){
		perror("open (temppipe)");
		exit(EXIT_FAILURE);
	}

	if(mkfifo(LIGHT_PIPE, 0766)==-1){
		perror("mkfifo (lightpipe)");
		exit(EXIT_FAILURE);
	}

	if((fdlight=open(LIGHT_PIPE, O_RDWR | O_TRUNC))==-1){
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
	radio.openWritingPipe(0x000000000002LL);
	radio.openReadingPipe(1,0x000000000001LL);
	radio.startListening();

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

		radio.stopListening();
		Command c;
		c.speed=speed;
		c.turn=turn;
		radio.write(&c, sizeof(Command));

		radio.startListening();

		pthread_mutex_unlock(&radio_mutex);

		printf("Speed and turn values successfully sent to arduino\n");
	}
	
	return EXIT_SUCCESS;
}
