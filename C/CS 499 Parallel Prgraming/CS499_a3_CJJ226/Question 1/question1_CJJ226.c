#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

void *do_work(void *arg);
pthread_mutex_t mutex;
int buffer[3];
int insertIndex = 0;
int totalSeq = 0;
int numOfCorrectSeq = 0;
int desiredCorrectSeq = 10;
int value1[1] = {1};
int value2[1] = {2};
int value3[1] = {3};


int main()
{
    pthread_mutex_init(&mutex, NULL);
    pthread_t value_thread1;
    pthread_t value_thread2;
    pthread_t value_thread3;

    pthread_create(&value_thread1, NULL, do_work, (void *) (int *) value1);
    pthread_create(&value_thread2, NULL, do_work, (void *) (int *) value2);
    pthread_create(&value_thread3, NULL, do_work, (void *) (int *) value3);

    pthread_join(value_thread1, NULL);
    pthread_join(value_thread2, NULL);
    pthread_join(value_thread3, NULL);


    printf("Total sequences generated: %d\n", totalSeq);
    printf("Number of correct sequences: %d\n", numOfCorrectSeq);

   return 0;
}


void *do_work(void *arg)
{
   int *valueList = (int *) arg;

   int value = valueList[0];

   while(numOfCorrectSeq < desiredCorrectSeq)
   {
       pthread_mutex_lock(&mutex);

       int isCorrect = 1; //1 is our true value

       printf("My id: %d\n", value);

       buffer[insertIndex] = value;
       insertIndex++;

       if(insertIndex > 2)
       {
          for(int bufInd = 0; bufInd < 2; bufInd++)
          {
             if( buffer[bufInd] != (bufInd+1) )
             {
                isCorrect = 0; //the buffer is bad
             }
          }

          if(isCorrect == 1)
          {
             printf("123\n");
             numOfCorrectSeq++;
          }

          insertIndex = 0; //reset our index
          totalSeq++;
        }
        
        pthread_mutex_unlock(&mutex);
        usleep(500000);

    }//end of while loop

}
