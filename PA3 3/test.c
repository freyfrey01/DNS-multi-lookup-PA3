#include "array.h"
#include <stdlib.h>
#include <stdio.h>
#include <sys/time.h>

void* writer(void* arg)
{
    array* buffer=(array*)arg;
    char* bufferTest[50];
    for (int j=0;j<50;j++)
    {
        bufferTest[j]="Test";
    }
    int i=0;
    while (i<50)
    {   
        array_put(buffer,bufferTest[i]);
        i++;
    }
    return NULL;
}
void* reader(void* arg)
{
    array* buffer=(array*)arg;
    char* hostname="";
    int i=0;
    while (i<100)
    {
        array_get(buffer,&hostname);
        i++;            
    }
    return NULL;
}

int main() {
    struct timeval start, end;
    gettimeofday(&start,NULL);
    array my_array;
    array_init(&my_array);

    pthread_t w1, w2, w3, w4, w5, w6, w7, w8, w9, w10, r1, r2, r3, r4, r5;
    
    pthread_create (&w1, NULL, writer, &my_array);
    pthread_create (&w2, NULL, writer, &my_array);
    pthread_create (&w3, NULL, writer, &my_array);
    pthread_create (&w4, NULL, writer, &my_array);  
    pthread_create (&w5, NULL, writer, &my_array);
    pthread_create (&w6, NULL, writer, &my_array);  //10 writers 500 items 50 items each
    pthread_create (&w7, NULL, writer, &my_array);
    pthread_create (&w8, NULL, writer, &my_array);
    pthread_create (&w9, NULL, writer, &my_array);
    pthread_create (&w10, NULL, writer, &my_array);

    pthread_create (&r1, NULL, reader, &my_array);
    pthread_create (&r2, NULL, reader, &my_array); //5 readers 100 items each
    pthread_create (&r3, NULL, reader, &my_array);
    pthread_create (&r4, NULL, reader, &my_array);
    pthread_create (&r5, NULL, reader, &my_array);
		                   
    pthread_join (w1, NULL);	        
    pthread_join (w2, NULL);
    pthread_join (w3, NULL);	        
    pthread_join (w4, NULL);
    pthread_join (w5, NULL);	        
    pthread_join (w6, NULL); 
    pthread_join (w7, NULL);	        
    pthread_join (w8, NULL);
    pthread_join (w9, NULL);	        
    pthread_join (w10, NULL);

    pthread_join (r1, NULL);
    pthread_join (r2, NULL);
    pthread_join (r3, NULL);
    pthread_join (r4, NULL);
    pthread_join (r5, NULL);

    array_free(&my_array); 
    gettimeofday(&end,NULL);
    long seconds = end.tv_sec - start.tv_sec;
    long microseconds = end.tv_usec - start.tv_usec;
    double elapsed = seconds + microseconds*1e-6;
    printf("./multi-lookup: total time is %.6f seconds.\n",elapsed);
    exit(0);
}
