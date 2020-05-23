#include <stdio.h>
#include <error.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdbool.h>

void slave_signal_handler(int signum); 
void slave(int prefix,int p) ;
void explore(int prefix) ;
void write_result(int n_route, int best) ;
void start_a_new_slave(int prefix) ;
void store_slave_info(int pid, int p) ;
void master () ;
int read_problem() ;
int init(int W) ;
int next(int prefix) ;
void read_slave_result_and_update(int pid) ;
void master_signal_handler(int signum) ;

int W = 0; //W: weight
bool terminate = false;//termination argument
int city_num = 0;//Number of cities
int child_num = 0;//Number of children processes
int path[50];
int used[50];
int prefix = 0;
int m[50][50];
int min = -1;

//signal handler for child
void slave_signal_handler(int signum) 
{
   if (signum == SIGINT) //check the signal code is right
     terminate = true;//terminate the child process
}

void slave(int prefix,int p) {

int best = 0;
int n_routes = 0;

    signal(SIGINT, slave_signal_handler) ;
    best, n_routes = explore(prefix) ;
    write_result(n_route, best) ;
    exit(0) ;
}

//calculate the routes in processes
void explore(int prefix){

int i = 0;
	
if (prefix == city_num) {
	W += m[path[prefix-1]][path[0]] ;
		if (min == -1 || min > length) {
			min = W ;

		}
		W -= m[path[prefix-1]][path[0]] ;
	}
	else {
		for (i = 0 ; i < city_num ; i++) {
			if (used[i] == 0) {
				path[prefix] = i ;
				used[i] = 1 ;
				W += m[path[prefix-1]][i] ;
					explore(prefix+1) ;
				W -= m[path[prefix-1]][i] ;
				used[i] = 0 ;
			}
		}
	}
}

//write result from the child processes
void write_result(int n_route, int best){
	printf("The number of routes: %d\nThe best route: %d\n", n_route, best);
}

void start_a_new_slave(int prefix) {
   int p[2] = pipe() ;
   if ((pid = fork()) == 0) /* child */ {
      slave(pipe, p[1]) ;
   }
   else /* parent */ {
      close(p[1]) ;
      store_slave_info(pid, p[0]) ; 
   }
}

void store_slave_info(int pid, int p){
	
}

void master () {

int n_slaves;//Number of child processes

  signal(SIGINT, master_signal_handler) ;
  W = read_problem() ;
  n_slaves = 0 ;
  prefix = init(W) ;  
  while (prefix != end ) {
     if (n_slaves == 12) {
          pid = wait() ;
          n_slaves-- ;
          read_result_and_update_best(pid) ;    // #-routes, N 
     }
     if (terminate == false) {
         start_a_new_slave(prefix) ;
         n_slaves++ ;
         prefix = next(prefix) ;
     }
     else if (terminate == true) {
        if (n_slaves == 0) {
            print_result() ;
            exit(0) ;
        }
      }
  }
}

int read_problem(){
int W;
	return W;	
}

//S1:[0,1,2,3,4]
int init(int W){

	for (int i = 0; i < prefix < i++) {
		prefix = path[i];
		explore(prefix) ;
		return prefix;
	}
}

 //next([0,1,2,3,4]) = [0,1,2,3,5]
 //next([0,1,2,3,16]) = [0,1,2,4,3]
int next(int prefix){

	path[(prefix-1)++];
	if((prefix-1) == city_num){
		path[(prefix-2)++] ;
		path[(prefix-1) = (prefix-2)] ;
	}
	return prefix;
}

//get the result from child processes and update
void read_slave_result_and_update(int pid){

}

//signal handler for parent process
void master_signal_handler(int signum) {
   if (signum == SIGINT) {//check the sognal number is correct
     // for each slave pid  {
         kill(SIGINT, pid) 
     }
      terminate = true ;
   }
}

int main(int argc, char *argv[]){
  int num1, num2 = 0;
  int i,j,t;
  int runProcess = 0;

	if(argc < 3) {
		printf("Input filename and Number of child process\n");
	}	
	if(argc == 3) {
		//get the number of cities
		num1 = argv[1][2] - '0';
		num2 = argv[1][3] - '0';
		city_num = num1*10 + num2;
		//get the number of child and prefix
		child_num = atoi(argv[2]);	
		prefix = city_num - child_num;
		}
		
		//print the number of cities and children
		printf("Number of city: %d\n",city_num); 
		printf("Number of child processes: %d\n", child_num);

	//open the file
	FILE *fp = fopen(argv[1], "r");
		//get the intiger from file
		for (i = 0; i < city_num; i++){
			for (j = 0; j < city_num; j++) {
				fscanf(fp, "%d", &t);
				m[i][j] = t;	
			}
		} fclose(fp);
		
	//create child process
	pid_t pids[child_num], pid ;
	
	while(runProcess < child_num) {
		pids[runProcess] = fork();
			
		if(pids[runProcess] < 0) { //error
			return -1;
		}
		runProcess++;
	}
	for (i = 0; i < prefix; i++){
		init(i);
	}
}
}
