/*-------------------------------------------------------
             prg4.c : variables condition
             
             main     : attend valeur max d'un entier
             thread 2 : incremente l'entier et signale condition
                        satisfaite 

             compiler avec l'option -lpthread
 --------------------------------------------------------*/

#include <stdio.h>
#include <limits.h>
#include <pthread.h>

#define sortieOK 0
#define INT_QQUE (INT_MAX/10000)

/*------------ package condition (a adapter a chaque pb selon
                                  la proposition a evaluer)    */

typedef enum {Faux=0,Vrai} bool;
typedef struct {
  pthread_cond_t  cond;
  pthread_mutex_t mutex;
  int i ;            // terme constituant la proposition a evaluer
  }  tpack1;         // ne pas acceder directement
typedef void (*tfonction1) (tpack1*);
typedef bool (*tfonction2) (tpack1*);


tpack1 pack;
     
void pack1_init(tpack1 *ppack,tfonction1 initTermes)
  { 
  initTermes(ppack);
  pthread_cond_init(&pack.cond,NULL);
  pthread_mutex_init(&pack.mutex,NULL);
  }
  
void pack1_attendre(tpack1 *ppack,tfonction2 propositionVraie,tfonction1 action)
  {
  pthread_mutex_lock(&ppack->mutex);  
  while (! propositionVraie(ppack))
     pthread_cond_wait(&ppack->cond,&ppack->mutex); // deverrouille mutex pendant blocage
  if (action!=NULL) action(ppack);
  pthread_mutex_unlock(&ppack->mutex);  
  }

bool pack1_signalerUn(tpack1 *ppack,tfonction1 action,tfonction2 propositionVraie)
  {
  bool signalerEffectue = Faux;
  pthread_mutex_lock(&ppack->mutex);
  if (action!=NULL) action(ppack);
  if (propositionVraie(ppack)) {
     pthread_cond_signal(&ppack->cond);
     signalerEffectue = Vrai;
     }
  pthread_mutex_unlock(&ppack->mutex);
  return signalerEffectue;
  }

    
//--------------- code manipulation de la proposition  

void initTermes(tpack1 *ppack)
  { 
  ppack->i=0;
  }

bool iEgalINT_QQUE(tpack1 *ppack)
  {
  return (ppack->i == INT_QQUE);
  }

void incrementeI(tpack1 *ppack)
  {
  ppack->i++;
  }

    
//---------------- code threads

void* codeThread2(void* p)
  {
  bool fin;
  do  
     fin=pack1_signalerUn(&pack,incrementeI,iEgalINT_QQUE);  
  while (!fin);
  return NULL;
  }

void codeThread1()
  {
  pack1_attendre(&pack,iEgalINT_QQUE,NULL);      
  printf("INT_QQUE=%d i=%d\n",INT_QQUE,pack.i);      // pas de pb SC
  }
  
int main(int argc,char* argv[],char* arge[])
  {
  pthread_t     thread2;
  pack1_init(&pack,initTermes);
  pthread_create(&thread2,NULL,codeThread2,NULL);
  codeThread1();
  }
  
