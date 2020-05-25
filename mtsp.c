#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sched.h>
#include <error.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdbool.h>

void consumer_signal_handler(int signum) ;
void producer_signal_handler(int signum) ;
void write_result() ;

int n_route = 0; // the total number of route
int broute = 0; // best route
int blength = 0; // best length
bool terminate = false;//termination argument

typedef struct {
	pthread_cond_t queue_cv ;
	pthread_cond_t dequeue_cv ;

	pthread_mutex_t lock ;
	char ** elem ;
	int capacity ;
	int num ; 
	int front ;
	int rear ;
} bounded_buffer ;

bounded_buffer * buf = 0x0 ;
 
//Input
int main(char *argv[], int argc) /*./mstp gr17.tsp 8*/
{
	//argv is a filr name of a TSP instance data 
	//argc is the initial number of comsumer thread
	
	// if the initial number is larger than 8
	if (argc > 8) printf("The limit of the consumer thread is 8, please try again\n"); 		
	//initial number is smaller than 9
	else {
		//create a producer thread and consumer threads
		//to start parallel solving of the given TSP instance
		//single process running multiple threads
		pthread_t producer_thread, consumer_thread;
			
	}
}

//Output

//user raise a termination signam (ie.Ctrl+c)
//terminate the program
void consumer_signal_handler(int signum)
{
	if (signum == SIGINT) //check the signal code is right
	terminate = true;//terminate the child process			
}

//user raise a termination signam (ie.Ctrl+c)
//terminate the program
void producer_signal_handler(int signum)
{
	if (signum == SIGINT) {//check the signal number is correct
		// for each slave pid
		terminate = true ;
	}
}
			

//print out 
//the best solution (a route and its length) upto point
//total number of checked/covered routes upto the point
void write_result()
{
	printf("The best route is %d\n", broute);
	printf("It's length is %d\n", blength);
	printf("The total number of route is %d\n", n_route);
}

//Interactive user command

