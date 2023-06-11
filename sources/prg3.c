/*-------------------------------------------------------
             prg3.c : exclusion mutuelle

             compiler avec l'option -pthread
 --------------------------------------------------------*/

#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

#define DureeSectionCritiqueEnSecondes 3

pthread_mutex_t mutex;

void sectionCritique(const char* msg) 
{
  pthread_mutex_lock(&mutex);
  printf("entree en section critique de %s\n",msg);
  sleep(DureeSectionCritiqueEnSecondes);
  printf("sortie de section critique de %s\n",msg);
  pthread_mutex_unlock(&mutex);
}

void* f (void* arg) 
{
  sleep(1);
  sectionCritique("thread2");
  return NULL;            // bidon
}

int main(int argc,char* argv[],char* arge[])
{
  pthread_t thread2;
  const char* msgThread2 = "thread2";
  pthread_mutex_init(&mutex,NULL);
  pthread_create(&thread2,NULL ,f,NULL);
  sleep(2);            // assure entree thread2 en SC en 1er
  sectionCritique("thread1");
}
  
