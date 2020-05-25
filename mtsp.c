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
void *producer (void *ptr) ;
void *consumer(void *ptr) ;

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

void 
bounded_buffer_init(bounded_buffer * buf, int capacity) {
	pthread_cond_init(&(buf->queue_cv), 0x0) ;
	pthread_cond_init(&(buf->dequeue_cv), 0x0) ;
	pthread_mutex_init(&(buf->lock), 0x0) ;
	buf->capacity = capacity ;
	buf->elem = (char **) calloc(sizeof(char *), capacity) ;
	buf->num = 0 ;
	buf->front = 0 ;
	buf->rear = 0 ;
}

void 
bounded_buffer_queue(bounded_buffer * buf, char * msg) 
{
	pthread_mutex_lock(&(buf->lock)) ;
		while (buf->num == buf->capacity) 
			pthread_cond_wait(&(buf->queue_cv), &(buf->lock)) ;
			
		buf->elem[buf->rear] = msg ;
		buf->rear = (buf->rear + 1) % buf->capacity ;
		buf->num += 1 ;
		
		pthread_cond_signal(&(buf->dequeue_cv)) ;
	pthread_mutex_unlock(&(buf->lock)) ;
}

char * 
bounded_buffer_dequeue(bounded_buffer * buf) 
{
	char * r = 0x0 ;

	pthread_mutex_lock(&(buf->lock)) ;
		while (buf->num == 0) 
			pthread_cond_wait(&(buf->dequeue_cv), &(buf->lock)) ;

		r = buf->elem[buf->front] ;
		buf->front = (buf->front + 1) % buf->capacity ;
		buf->num -= 1 ;

		pthread_cond_signal(&(buf->queue_cv)) ;
	pthread_mutex_unlock(&(buf->lock)) ;

	return r ;
} 

//Input
//main thread
int main(char *argv[], int argc) /*./mstp gr17.tsp 8*/
{
	//argv is a filr name of a TSP instance data 
	//argc is the initial number of comsumer thread
	
	// if the initial number is larger than 8
	if (argc > 8) printf("The limit of the consumer thread is 8, please try again\n"); 		
	//initial number is smaller than 9
	else {
		pthread_t prod; //producer thread
		pthread_t cons[argc]; //comsumer thread
		int i = 0; //loop
		
		buf = malloc(sizeof(bounded_buffer));
		bounded_buffer_init(buf, );

		//create a producer thread and consumer threads
		//to start parallel solving of the given TSP instance
		//single process running multiple threads
		pthread_create(&(prod), 0x0, producer, 0x0) ;
		for (i = 0; i < argc; i++) {
			pthread_create(&(cons[i]), 0x0, consumer, 0x0) ;
		}
		
		pthread_join(prod, 0x0);
		for (i = 0; i < argc; i++){
			pthread_join(cons[i], 0x0);
		}
				
	}
	exit(0);
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

//producer thread
void *producer (void *ptr)
{
	do{
		// produce an item in nextp
		wait(empty);
		wait(mutex);
		
		// add nextp to buffer
		signal(mutex);
		signal(full);
	}while (TRUE);
}

//comsumer thread
void *consumer(void *ptr)
{

}
