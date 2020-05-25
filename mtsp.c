#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sched.h>

void consumer_signal_handler(int signum) ;
void producer_signal_handler(int sugnum) ;
void write_result(int n_route, int broute, int blength) ;

//Input
int main(char *argv[], int argc) /*./mstp gr17.tsp 8*/
{
	//argv is a filr name of a TSP instance data 
	//argc is the initial number of comsumer thread
	
	// if the initial number is larger than 8
	if (argc > 8) printf("the limit is 8, please try again\n"); 		
	//initial number is smaller than 9
	else {
		
	}
}

//Output

//user raise a termination signam (ie.Ctrl+c)
//terminate the program
void consumer_signal_handler(int signum)
{
			
}

//user raise a termination signam (ie.Ctrl+c)
//terminate the program
void producer_signal_handler(int sugnum)
{

}

//print out 
//the best solution (a route and its length) upto point
//total number of checked/covered routes upto the point
void write_result(int n_route, int broute, int blength)
{
	printf("The best route is %d\n", broute);
	printf("It's length is %d\n", blength);
	printf("The total number of route is %d\n", n_route);
}
