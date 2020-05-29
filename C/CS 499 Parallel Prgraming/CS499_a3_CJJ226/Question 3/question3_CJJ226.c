#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

void *do_work(void *arg);
int total = 0;
int insertQueue = 0;
int value0[1] = {0};
int value1[1] = {1};
int value2[1] = {2};
int value3[1] = {3};
int value4[1] = {4};
int value5[1] = {5};
int value6[1] = {6};
int value7[1] = {7};
int value8[1] = {8};
int value9[1] = {9};


int main()
{
    pthread_t value_thread0;
    pthread_t value_thread1;
    pthread_t value_thread2;
    pthread_t value_thread3;
    pthread_t value_thread4;
    pthread_t value_thread5;
    pthread_t value_thread6;
    pthread_t value_thread7;
    pthread_t value_thread8;
    pthread_t value_thread9;

    pthread_create(&value_thread0, NULL, do_work, (void *) (int *) value0);
    pthread_create(&value_thread1, NULL, do_work, (void *) (int *) value1);
    pthread_create(&value_thread2, NULL, do_work, (void *) (int *) value2);
    pthread_create(&value_thread3, NULL, do_work, (void *) (int *) value3);
    pthread_create(&value_thread4, NULL, do_work, (void *) (int *) value4);
    pthread_create(&value_thread5, NULL, do_work, (void *) (int *) value5);
    pthread_create(&value_thread6, NULL, do_work, (void *) (int *) value6);
    pthread_create(&value_thread7, NULL, do_work, (void *) (int *) value7);
    pthread_create(&value_thread8, NULL, do_work, (void *) (int *) value8);
    pthread_create(&value_thread9, NULL, do_work, (void *) (int *) value9);

    pthread_join(value_thread0, NULL);
    pthread_join(value_thread1, NULL);
    pthread_join(value_thread2, NULL);
    pthread_join(value_thread3, NULL);
    pthread_join(value_thread4, NULL);
    pthread_join(value_thread5, NULL);
    pthread_join(value_thread6, NULL);
    pthread_join(value_thread7, NULL);
    pthread_join(value_thread8, NULL);
    pthread_join(value_thread9, NULL);


    printf("Total: %d\n", total);

   return 0;
}


void *do_work(void *arg)
{
   int *numList = (int *) arg;

   int num = numList[0];

   while(total < 990)
   {
       if(num == insertQueue)
       {
          if(total == 990)
          {
            break;
          }

          total += num;
          printf("my num: %d", num);
          printf(", total: %d\n", total);
          insertQueue++;

          if(insertQueue > 9)
          {
             insertQueue = 0;
          }
       }

   }//end of while loop
}
