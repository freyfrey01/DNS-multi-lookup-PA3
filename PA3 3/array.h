
#ifndef ARRAY_H
#define ARRAY_H

#define MAX_NAME_LENGTH 50 
#define ARRAY_SIZE 8

#include <semaphore.h>
#include <pthread.h>


typedef struct 
{
    int front,end;   
    int arraySize;   
    char** array;  
    pthread_mutex_t buffermutex;
	sem_t space_avail;
	sem_t items_avail;                    
} array;

int  array_init(array *s);                  
int  array_put(array *s, char *hostname);    
int  array_get (array *s, char **hostname);   
void array_free(array *s);

#endif