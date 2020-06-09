#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>
#include <execinfo.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>
#include <sys/timeb.h>  /* ftime, timeb (for timestamp in millisecond) */
#include <sys/time.h>
#define _POSIX_C_SOURCE 200809L
#include <inttypes.h>
#include <math.h>

#define Nthreads 10
#define Nmutexs 10

struct timespec spec;

// This is node fir moniter array
struct mo_node{
	long time;
	pthread_t thid; //0
	pthread_mutex_t *mutex;
};

struct th_edge{
	long time;
	pthread_t src;
	pthread_t dest;
};

//monitor array
struct mo_node monitor[Nthreads][Nmutexs];

//array for mutex
//mutex will get id number
pthread_mutex_t *mo_array[Nmutexs];

//array to make adjcent array
int **adj_array;

//the edge of the thread
struct th_edge th_edge[Nthreads];

// Add mutex to monitor array
// same row has same thread_id
// new mutex goes next column index
void addToMonitor(pthread_t thid, pthread_mutex_t *mutex){
	for(int i = 0; i < Nthreads; i++){
		if(monitor[i][0].thid == thid){
			for(int j = 0; j < Nmutexs; j++){
				if(monitor[i][j].mutex == NULL){
					monitor[i][j].mutex = mutex;
					monitor[i][j].thid = thid;
					clock_gettime(CLOCK_REALTIME, &spec);
						monitor[i][j].time = (spec.tv_nsec);
						return;
				}
			}
		} else if(monitor[i][0].mutex == NULL){
			monitor[i][0].thid = thid;
			monitor[i][0].mutex = mutex;
				clock_gettime(CLOCK_REALTIME, &spec);
				monitor[i][0].time = (spec.tv_nsec);
				return;
		}
	}
}

void printer(){
	FILE *fp = fopen("ddmon.trace", "w");
		//file write
		//monitor.thid
		for(int i = 0; i < Nthreads; i++){
			for(int j = 0; j < Nmutexs; j++){
				fprintf(fp, "%lu ", monitor[i][j].thid);
			}
		}
		//monitor.time
		fprintf(fp, "\n");
		for(int i = 0; i<Nthreads; i++){
			for (int j = 0; j<Nmutexs; j++){
				if(monitor[i][j].mutex == NULL) fprintf(fp, "0 ");
				else fprintf(fp, "%p ", monitor[i][j].mutex);
			}
		}
		//thread.src
		fprintf(fp, "\n");
		for(int i = 0; i < Nthreads; i++){
			fprintf(fp, "%lu ", th_edge[i].src);
		}
		//thread.dest
		fprintf(fp, "\n");
		for(int i = 0; i < Nthreads; i++) {
			fprintf(fp, "%lu ", th_edge[i].dest);
		}
		//thread.time
		fprintf(fp, "\n");
		for (int i = 0; i < Nthreads; i++){
			fprintf(fp, "%lu ", th_edge[i].time);
		}
		fprintf(fp, "\n");

		//backtrace
		void * arr[10];
		char **stack ;

		size_t size = backtrace(arr, 10) ;
		stack = backtrace_symbols(arr, size) ;

		fprintf(fp, "%s\n", stack[2]) ;

		fclose(fp);

}

//mutex for mutex_lock hooking
pthread_mutex_t localmutex = PTHREAD_MUTEX_INITIALIZER;
int pthread_mutex_lock (pthread_mutex_t * mutex){
	static __thread int n_mutex = 0;
	n_mutex += 1 ;
	
	int (*lockP)(pthread_mutex_t *mutex) ;
	char * error ;

	lockP = dlsym(RTLD_NEXT, "pthread_mutex_lock") ;
	if ((error = dlerror()) != 0x0)
		exit(1);

	if (n_mutex == 1) {
		pthread_mutex_lock(&localmutex);
		//add th monitor array
		addToMonitor(pthread_self(), mutex);
		printer();
		pthread_mutex_unlock(&localmutex);
	}
	n_mutex -= 1;
	return 0;
}

// Insert pthread_id to th_arr
void addToEdge(pthread_t presentThread, pthread_t newThread){
	for(int i = 0; i < Nthreads; i++){
		if(th_edge[i].src == 0){
			th_edge[i].src = presentThread;
			th_edge[i].dest = newThread;
			clock_gettime(CLOCK_REALTIME, &spec);
			th_edge[i].time = (spec.tv_nsec) ;

			return;
		}
	}
}

int pthread_create(pthread_t * thread, const pthread_attr_t * attr, void *(*start)(void *), void *arg){
	static int (*pthread_createP)(pthread_t *thread, const pthread_attr_t *attr, void*(*start)(void *), void *arg) ;
	char * error ;

	pthread_createP = dlsym(RTLD_NEXT, "pthread_create") ;
	if ((error = dlerror()) != 0x0)
		exit(1);

	int return_value = pthread_createP(thread, attr, start, arg);
	pthread_t src = pthread_self();
	addToEdge(src, *thread);

	return return_value;
}
