#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <dlfcn.h>
#include <execinfo.h>

struct Node{
	int count;
	pthread_mutex_t *mutex;
	struct Node *next;
}Node;

struct Node* head;

// Print the value in linked list
void printer(){
	struct Node* current = head;	
	while(current != NULL){
		printf(" %d->", current->count);
		current = current->next;
	}
	printf("\n");
	return;
}

// Detect the deadlock
void detect(){
	printer();
	struct Node* current = head;
	if(current != NULL){
		if(current->count >= 0){
			return;
		}
		current = current->next;
	}
	char buf[50];
	snprintf(buf, 50, "deadlock\n");
	fputs(buf, stderr);
	return;
}

// Push item in linked list
void push(struct Node** head, pthread_mutex_t *mutex){
	//allocate node
	struct Node* newNode;
	newNode = (struct Node*) malloc(sizeof(struct Node));

	//put in the key
	newNode->mutex = mutex;
	newNode->count = 0;
	
	//link the old list to new node
	newNode->next = (*head);
	
	//move the head to point to new node
	(*head) = newNode;
}

int lock_mutex_find(pthread_mutex_t *mutex){
	struct Node* current = head;
	int find = 0;
	while (current != NULL){
		if (current->mutex == mutex){
			current->count = current->count-1;
			return 1;
		}
		current = current->next;
	}
	push(&head, mutex);
	return 0;
}

int unlock_mutex_find(pthread_mutex_t *mutex){
	struct Node* current = head;
	int find = 0;
	while (current != NULL){
		if (current->mutex == mutex){
			if(current->count <= 0){
				current->count = current -> count + 1;
				return 1;
			}
			else return 1;
		}
		current = current->next;
	}
	return 0;
}

int pthread_mutex_lock(pthread_mutex_t *mutex){
	int (*lockP)(pthread_mutex_t *mutex) ;
	char * error ;

	lockP = dlsym(RTLD_NEXT, "pthread_mutex_lock") ;
	if ((error = dlerror()) != 0x0)
		exit(1) ;

	//find the mutex in linked list and update status or push new node
	lock_mutex_find(mutex);

	//detect deadlock
	detect();

	lockP(mutex);
	return 0;
}

int pthread_mutex_unlock(pthread_mutex_t *mutex){
	int (*unlockP)(pthread_mutex_t *mutex) ;
	char * error ;

	unlockP = dlsym(RTLD_NEXT, "pthread_mutex_unlock") ;
	if ((error = dlerror()) != 0x0) 
		exit(1);

	unlock_mutex_find(mutex);
	char buf[50] ;
	snprintf(buf, 50, "out\n") ;
	fputs(buf, stderr) ;
	unlockP(mutex);
	return 77 ;
}
