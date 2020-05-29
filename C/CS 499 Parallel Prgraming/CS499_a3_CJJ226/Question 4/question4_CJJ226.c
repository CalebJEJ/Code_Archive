#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>

void *increment_work(void *arg);
void *decrement_work(void *arg);
sem_t up;
sem_t down;
int remainingInc = 30;
int remainingDec = 20;
int count = 0;
int switchCount = 1;


int main()
{
    pthread_t value_threadI;
    pthread_t value_threadD;

    sem_init(&up, 0, 1);
    sem_init(&down, 1, 1);

    pthread_create(&value_threadI, NULL, increment_work, NULL);
    pthread_create(&value_threadD, NULL, decrement_work, NULL);

    pthread_join(value_threadI, NULL);
    pthread_join(value_threadD, NULL);


   return 0;
}


void *increment_work(void *arg)
{
   while(remainingInc > 0) 
   {
      sem_wait(&up);

      while(count < 10)
      {
         count++;
         remainingInc--;
         printf("Count is now (inc fn): %d\n", count);
      }

      sem_post(&down);
   }
}

void *decrement_work(void *arg)
{
   while(remainingDec > 0) 
   {
      sem_wait(&down);

      while(count > 0)
      {
         count--;
         remainingDec--;
         printf("Count is now (dec fn): %d\n", count);
      }

      sem_post(&up);
   }
}
