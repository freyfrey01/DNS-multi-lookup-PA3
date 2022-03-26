#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "array.h"


int array_init(array *s)
{
    s->array=(char**)malloc(sizeof(char)*ARRAY_SIZE*MAX_NAME_LENGTH);
    int i;
    for (i=0;i<ARRAY_SIZE;i++)
    {
        s->array[i]="";
    }
    s->arraySize=ARRAY_SIZE;
    s->front=0;
    s->end=0;
    pthread_mutex_init(&s->buffermutex, NULL);
	sem_init (&s->space_avail, 0, ARRAY_SIZE);
	sem_init (&s->items_avail, 0, 0);
    return ARRAY_SIZE;
}        

int array_put(array *s, char *hostname)
{
    int length=strlen(hostname);
    if(length>MAX_NAME_LENGTH)
    {
        return -1;
    }
    sem_wait(&s->space_avail);
    pthread_mutex_lock(&s->buffermutex);
    char* name=(char*)malloc(MAX_NAME_LENGTH);
    strncpy(name,hostname,MAX_NAME_LENGTH);
    s->array[s->end]=name;
    s->end=(((s->end)+1)%ARRAY_SIZE);
    pthread_mutex_unlock(&s->buffermutex);
    sem_post(&s->items_avail);
    return 0;
}   
int  array_get (array *s, char **hostname)
{  
    sem_wait(&s->items_avail);
    pthread_mutex_lock(&s->buffermutex);
    *hostname=s->array[s->front];
    s->array[s->front]="";
    s->front=(((s->front)+1) % ARRAY_SIZE);
    pthread_mutex_unlock(&s->buffermutex);
    sem_post(&s->space_avail);
    return 0;
} 
void array_free(array *s)
{
    for(int i=0;i<ARRAY_SIZE;i++)
    {
        if(strcmp(s->array[i],"")!=0)
        {
            free(s->array[i]);
        }
    }
    free(s->array);
    sem_destroy(&s->items_avail);
    sem_destroy(&s->space_avail);
    pthread_mutex_destroy(&s->buffermutex);
    return;
}
