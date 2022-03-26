#include "util.h"
#include "array.h"
#include "multi-lookup.h"
#include <semaphore.h>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <sys/time.h>
void* requester(void* requesterInfo)
{
    requesterThread *thread=requesterInfo;
    array *sharedBuffer=thread->queue;
    pthread_mutex_t *outputMutex=thread->outputMutex;
    pthread_mutex_t *printMutex=thread->printMutex;
    pthread_mutex_t *inputFileMutex=thread->inputFileMutex;
    FILE **inputs=thread->inputFiles;
    FILE *outputFile=thread->outputFile;
    int *numberInputs=thread->inputNumber;
    pid_t tid=pthread_self();
    int count=0;
    int fileNumber;
    char hostname[MAX_NAME_LENGTH];
    pthread_mutex_lock(inputFileMutex);
    fileNumber=*thread->current;
    *thread->current=*thread->current+1;
    pthread_mutex_unlock(inputFileMutex);
    while(fileNumber<*numberInputs)
    {
        FILE *current=inputs[fileNumber];
        while(fgets(hostname,MAX_NAME_LENGTH,current))
        {
            int length=strlen(hostname);
            hostname[length-1]='\0';
            if(array_put(sharedBuffer,hostname)==0)
            {
                pthread_mutex_lock(outputMutex);
                fprintf(outputFile,"%s\n",hostname);
                pthread_mutex_unlock(outputMutex);
            }
        }
        count++;
        pthread_mutex_lock(inputFileMutex);
        fileNumber=*thread->current;
        *thread->current=*thread->current+1;
        pthread_mutex_unlock(inputFileMutex);
    }
    pthread_mutex_lock(printMutex);
    printf("thread %d serviced %d files\n",tid,count);
    pthread_mutex_unlock(printMutex);
    return NULL;
}
void* resolver(void* resolverInfo)
{
    resolverThread *thread=resolverInfo;
    array *sharedBuffer=thread->queue;
    pthread_mutex_t *outputMutex=thread->outputMutex;
    pthread_mutex_t *printMutex=thread->printMutex;
    FILE *outputFile=thread->outputFile;
    pid_t tid=pthread_self();
    int count=0;
    char name[MAX_NAME_LENGTH];
    char* hostname=name;
    char ip[MAX_IP_LENGTH];
    int dns;
    while(1)
    {
        array_get(sharedBuffer,&hostname);
        if(strcmp(hostname,"KILL")==0)
        {
            free(hostname);
            array_put(sharedBuffer,"KILL");
            break;
        } 
        dns=dnslookup(hostname,ip,sizeof(ip));
        if(dns == -1)
        {
            strncpy(ip,"NOT_RESOLVED",sizeof(ip));
        }
        else
        {
            count++;
        }
        pthread_mutex_lock(outputMutex);
        fprintf(outputFile,"%s, %s\n",hostname,ip);
        pthread_mutex_unlock(outputMutex);
        free(hostname);
    }
    pthread_mutex_lock(printMutex);
    printf("thread %d resolved %d hostnames\n",tid,count);
    pthread_mutex_unlock(printMutex);
    return NULL;
}

int main(int argc, char*argv[])
{
    struct timeval start, end;
    gettimeofday(&start,NULL);

    if (argc<6)
    {
        fprintf(stderr,"Not enough arguemnts. Use ./multi-lookup <# requester threads> <# resolver threads> <requester log file> <resolver log file> [ <input file> ... ]\n");
        return -1;
    }
    else if (argc>105)                      //checking for correct number of arguments
    {
        fprintf(stderr,"Too many input files. Must be 100 or less.\n");
        return -1;
    }

    int numberRequesters=atoi(argv[1]);
    int numberResolvers= atoi(argv[2]);
    if(numberRequesters>MAX_REQUESTER_THREADS || numberResolvers>MAX_RESOLVER_THREADS)
    {
        fprintf(stderr,"Too many requester or resolver threads. Must both be less than or equal to: %d.\n",MAX_REQUESTER_THREADS);
        return -1;
    }
    else if(numberRequesters<1 || numberResolvers<1)                        //too many or not enough threads
    {
        fprintf(stderr,"Not enough requester or resolver threads. Input a number for each from 1-10.\n");
        return -1;
    }

    int numberInputs=argc-5;
    int inputs=0;
    FILE* inputFiles[numberInputs];
    FILE* inFile;

    for (int i=0;i<numberInputs;i++)
    {
        inFile=fopen(argv[i+5],"r");
        if(inFile==NULL)
        {
            fprintf(stderr,"Invalid file: %s.\n",argv[i+5]);   //fill array of input files if they exist
        }
        else
        {
            inputFiles[inputs]=inFile;
            inputs++;
        }
    }
    if(inputs==0)
    {
        fprintf(stderr,"No valid input files to resolve.\n");
        return -1;
    }

    char* requesterLogName=argv[3];
    char* resolverLogName=argv[4];
    pthread_t requesters[numberRequesters];
    pthread_t resolvers[numberResolvers];
    FILE* requesterLog=fopen(requesterLogName,"w");
    FILE* resolverLog=fopen(resolverLogName,"w");

    int current=0;
    pthread_mutex_t printMutex;
    pthread_mutex_init(&printMutex,NULL);       //synchronization for stdout
    array my_array;
    array_init(&my_array);
    requesterThread rqThread;
    resolverThread rsThread;

    pthread_mutex_t inputFileMutex,rqOutputMutex;
    pthread_mutex_init(&inputFileMutex,NULL);
    pthread_mutex_init(&rqOutputMutex,NULL);
    rqThread.inputFileMutex=&inputFileMutex;
    rqThread.outputMutex=&rqOutputMutex;
    rqThread.queue=&my_array;
    rqThread.inputFiles=inputFiles;
    rqThread.outputFile=requesterLog;
    rqThread.inputNumber=&inputs;
    rqThread.current=&current;
    rqThread.printMutex=&printMutex; //initalize threads info

    pthread_mutex_t rsOutputMutex;
    pthread_mutex_init(&rsOutputMutex,NULL);
    rsThread.outputMutex=&rsOutputMutex;
    rsThread.queue=&my_array;
    rsThread.outputFile=resolverLog;
    rsThread.printMutex=&printMutex;

    for(int i=0;i<numberRequesters;i++)     //create requester threads
    {
        pthread_create(&requesters[i], NULL, requester, (void *)&rqThread);
    }
    for(int k=0;k<numberResolvers;k++)
    {
        pthread_create(&resolvers[k], NULL, resolver, (void *)&rsThread);           //create resolver threads
    }

    for(int l=0;l<numberRequesters;l++)
    {
        pthread_join(requesters[l],NULL);
    }
    char* poisonPill="KILL";
    array_put(&my_array,poisonPill);
    for(int m=0;m<numberResolvers;m++)
    {
        pthread_join(resolvers[m],NULL);
    }

    fclose(requesterLog);
    fclose(resolverLog);
    for(int j=0;j<inputs;j++)
    {
        fclose(inputFiles[j]);          //close all files
    }

    pthread_mutex_destroy(&rqOutputMutex);
    pthread_mutex_destroy(&inputFileMutex);
    pthread_mutex_destroy(&rsOutputMutex);   
    pthread_mutex_destroy(&printMutex);         //destory mutex's allocated and free array's memory
    array_free(&my_array);

    gettimeofday(&end,NULL);
    long seconds = end.tv_sec - start.tv_sec;
    long microseconds = end.tv_usec - start.tv_usec;                //calculate time elapsed
    double elapsed = seconds + microseconds*1e-6;
    printf("./multi-lookup: total time is %.6f seconds.\n",elapsed);
    return 0;
}
