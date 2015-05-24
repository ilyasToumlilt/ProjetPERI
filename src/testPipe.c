#define _XOPEN_SOURCE 777

#include <stdlib.h>
#include <stdio.h>   
#include <sys/types.h>
#include <sys/stat.h>
#include <stdint.h>       
#include <fcntl.h>
#include <unistd.h>
#include <sys/select.h>
#include <time.h>

int main(int argc, char** argv)
{
  if(mkfifo("/tmp/speedpipe", 0766) == -1){
    perror("mkfifo");
    exit(EXIT_FAILURE);
  }

  int fd;
  if((fd=open("/tmp/speedpipe", O_RDWR | O_TRUNC))==-1){
    perror("open");
    exit(EXIT_FAILURE);
  }

  char value = 1; int dir = 0;
  char send;
  uint16_t rr;
  while(1){
    /*
    dir = (value==0) ? 0 : (value==100) ? 1 : dir;
    value = (dir==0) ? value+1 : value-1;
    send = 100;
    if(write(fd, &send, 2)!= 2){
      perror("piiiipiiiii");
      exit(EXIT_FAILURE);
    }
    */
    
    read(fd, &rr, 2);
    printf("%d\n", rr);
    /*
    usleep(100000);
    */
  }

  return EXIT_SUCCESS;
}
