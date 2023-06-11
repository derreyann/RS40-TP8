/*-------------------------------------------------------
             prg1.c : creation, attente terminaison

             compiler avec l'option -lpthread
 --------------------------------------------------------*/

#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
int iGlobal = 1;
pthread_mutex_t mutex;

#define MarqueurFinArg -1

void *f(void *arg)
{
    int threadnum = *(int *)arg;
    printf("thread %d : iGlobal = %d\n", threadnum, iGlobal);
    pthread_mutex_lock(&mutex);
    iGlobal++;
    sleep(2);
    pthread_mutex_unlock(&mutex);
    printf("thread %d : iGlobal++ = %d\n", threadnum, iGlobal);
    return NULL;
}

int main(int argc, char *argv[], char *arge[])
{
    if (argc < 2)
    {
        printf("Usage: %s <num_threads>\n", argv[0]);
        return 1;
    }
    int num_threads = atoi(argv[1]);
    pthread_t *threads = malloc(num_threads * sizeof(pthread_t));
    pthread_mutex_init(&mutex, NULL);

    for (int i = 0; i < num_threads; i++)
    {
        int *threadnum = (int *)malloc(sizeof(int));
        *threadnum = i+1;
        if (pthread_create(&threads[i], NULL, f, threadnum) == -1)
        { // creation du thread
            printf("pb pthread_create\n");
            exit(1);
        }
    }
    for(int i = 0; i < num_threads; i++)
    {
        if(pthread_join(threads[i], NULL) == -1)
        {
            printf("pb pthread_join\n");
            exit(1);
        }
    }

    printf("final iGlobal = %d\n", iGlobal);

    pthread_mutex_destroy(&mutex);
    free(threads);
    return 0;
}
