#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

void handler (int sig);

int main(int argc, char *argv[]){
  int city_num = 0;
  int num1, num2 = 0;
  int child_num = 0;
  int prefix = 0;
  int m[50][50];
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
		} else if(pids[runProcess] == 0) { /*child process*/
			//printf("child %ld\n", (long)getpid());
			

			sleep(10);
			exit(0);		
		} else { /*parent process*/
			//printf("parent %ld, child %ld\n", (long)getpid(), (long)pids[runProcess]);
		
		}
		runProcess++;
	}
	
	//create signal
	signal(SIGINT, handler) ;
	while(1);	
}

//signal handler
void handler(int sig){
	if (sig == SIGINT) {
		printf("Do you want to quit?(enter 'y')\n");
		if (getchar() == 'y')
			exit(0) ;
	}
}
