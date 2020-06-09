#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <fcntl.h>
#include <time.h>
#include <errno.h>
#include <unistd.h>
#include <signal.h>
/*TODO*/

#define BUF_SIZE 1024

typedef struct {
	//TODO
	sem_t sem1;
	sem_t sem2;
	size_t cnt;
	char buf[BUF_SIZE];
} mysem_t ;

#define handle_error(msg) \
           do { perror(msg); exit(EXIT_FAILURE); } while (0)

void
mysem_init (mysem_t * sem, int init)
{
	//TODO
	mtsem_t sem;
	
	if (sem_init(&shmp->sem1, 0) == -1)
                   errExit("sem_init-sem1");
               if (sem_init(&shmp->sem2, 0) == -1)
                   errExit("sem_init-sem2");
}

void
mysem_post (mysem_t * sem)
{
	//TODO
           write(STDOUT_FILENO, "sem_post() from handler\n", 24);
           if (sem_post(&sem) == -1) {
               write(STDERR_FILENO, "sem_post() failed\n", 18);
               _exit(EXIT_FAILURE);
           }
}

void
mysem_wait (mysem_t * sem)
{
	//TODO
	struct mysem_t sa;
	if(mysem_init(&sem, 0) == -1) handler_error("sem_init");

	sa.sa_handler = handler;
	sigemptyset(&sa.sa_mask);
           sa.sa_flags = 0;
           if (sigaction(SIGALRM, &sa, NULL) == -1)
               handle_error("sigaction");

           if (clock_gettime(CLOCK_REALTIME, &ts) == -1)
               handle_error("clock_gettime");

           while ((s = sem_timedwait(&sem, &ts)) == -1 && errno == EINTR)
               continue;       /* Restart if interrupted by handler */

           /* Check what happened */

           if (s == -1) {
               if (errno == ETIMEDOUT)
                   printf("sem_timedwait() timed out\n");
               else
                   perror("sem_timedwait");
           } else
               printf("sem_timedwait() succeeded\n");

           exit((s == 0) ? EXIT_SUCCESS : EXIT_FAILURE);
}

