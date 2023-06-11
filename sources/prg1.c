/*-------------------------------------------------------
             prg1.c : creation, attente terminaison

             compiler avec l'option -lpthread
 --------------------------------------------------------*/

#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
int iGlobal = 1;

#define MarqueurFinArg -1                               

void* f (void* arg) 
  {
  int i=0;
  int *p = (int*) arg;
  printf("thread 2 : iGlobal = %d  ",iGlobal);
  do  
     printf("arg[%d] = %d  ",i,p[i]);
  while (p[++i]!= MarqueurFinArg);
  printf("\n");
  iGlobal++;
  
  sleep(2);
  
  printf("thread 2 : iGlobal = %d\n",iGlobal);
  printf("thread 2 : terminaison\n");
  return (void*) iGlobal;             
  }

int main(int argc,char* argv[],char* arge[])
  {
  pthread_t thread2;			 //declaration d'un thread
  int param[]={0,1,MarqueurFinArg};
  int codeRetour;
  if (pthread_create(&thread2,NULL,f,param)==-1) { //creation du thread
      printf("pb pthread_create\n"); exit(1);
  }
  printf("thread 1 : thread 2 created,    iGlobal = %d\n",iGlobal);

  sleep(1);

  printf("thread 1 : thread2 end, iGlobal = %d\n",iGlobal);
  pthread_join(thread2,(void**) &codeRetour);
  printf("thread 1 : thread2 joined, iGlobal = %d   codeRetour = %d\n",iGlobal,codeRetour);
  }
  
