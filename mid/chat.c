#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <signal.h>

/* TODO: you can freely edit this file */

int
main (int argc, char ** argv)
{
	if (argc != 3) {
		fprintf(stderr, "Wrong number of arguments\n") ;
		exit(1) ;
	}

	char * r_fifo = argv[1] ;
	char * w_fifo = argv[2] ;

	/*TODO*/
	if (r_fifo != 0){
		char * buf[256] = 0x0;
		int fd = O_NONBLOCK(r_fifo);
		reader(r_fifo);
		handler(0);
	}

	else if (w_fifo != 0){
		char * buf[256] = 0x0;
		int fd = O_NONBLOCK(w_fifo);	
		writer(w_fifo);
		handler(0);
	}
}

int reader(char * r){
if (mkfifo("channel", 0666)) {
		if (errno != EEXIST) {
			perror("fail to open fifo: ") ;
			exit(1) ;
		}
	}

	int fd = open("channel", O_WRONLY | O_SYNC) ;

	while (1) {
		char s[128] ;
		char newline ;

		int i = 0 ;
		while (i < 128 && ((s[i] = getchar()) != EOF && s[i++] != '\n')) ;
		s[i - 1] = 0x0 ;
		
		if (s[0] == 0x0) 
			break ;

		for (int i = 0 ; i < 128 ; ) {
			i += write(fd, s + i, 128) ;
		} 
	}
	close(fd) ;
	return 0 ;
}
void handler(int sig){
	printf("Goodbye");
	SIGTERM;
}

int writer(char * w) {
	int fd = open("channel", O_RDONLY | O_SYNC) ;

	while (1) {
		char s[128] ;
		int len ;
		len = read(fd, s, 128) ;
		if (len > 0) 
			printf("%s\n", s) ;
	}
	close(fd) ;
	return 0 ;
}
