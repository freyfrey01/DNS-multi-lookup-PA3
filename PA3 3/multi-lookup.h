#ifndef MULTI_LOOKUP_H
#define MULTI_LOOKUP_H

#include "util.h"
#include "array.h"
#include <semaphore.h>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define MAX_INPUT_FILES 100
#define MAX_REQUESTER_THREADS 10
#define MAX_RESOLVER_THREADS 10
#define MAX_IP_LENGTH INET6_ADDRSTRLEN

typedef struct 
{
    array *queue;
    FILE **inputFiles;
    FILE *outputFile;
    pthread_mutex_t* inputFileMutex;
    pthread_mutex_t* outputMutex; 
    pthread_mutex_t* printMutex;
    int* inputNumber;  
    int* current;
}requesterThread;
typedef struct 
{
    array *queue;
    FILE *outputFile;
    pthread_mutex_t* outputMutex; 
    pthread_mutex_t* printMutex;
}resolverThread;


void* requester(void* threadInfo);
void* resolver(void* threadInfo);

#endif
