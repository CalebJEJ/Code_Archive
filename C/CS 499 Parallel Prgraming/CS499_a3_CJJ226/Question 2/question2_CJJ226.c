#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

void *do_work(void *arg);
void *do_work2(void *arg);
pthread_mutex_t mutex;
pthread_mutex_t mutex2;
int buffer[3];
int buffer2[3];
int insertIndex = 0;
int insertIndex2 = 0;
int totalSeq = 0;
int totalSeq2 = 0;
int numOfCorrectSeq = 0;
int numOfCorrectSeq2 = 0;
int desiredCorrectSeq = 10;
int winningTeam = 0;
int value1[1] = {1};
int value2[1] = {2};
int value3[1] = {3};
int value4[1] = {4};
int value5[1] = {5};
int value6[1] = {6};

int main()
{
    pthread_mutex_init(&mutex, NULL);
    pthread_mutex_init(&mutex2, NULL);
    pthread_t value_thread1;
    pthread_t value_thread2;
    pthread_t value_thread3;

    pthread_create(&value_thread1, NULL, do_work, (void *) (int *) value1);
    pthread_create(&value_thread2, NULL, do_work, (void *) (int *) value2);
    pthread_create(&value_thread3, NULL, do_work, (void *) (int *) value3);

    pthread_t value_thread4;
    pthread_t value_thread5;
    pthread_t value_thread6;

    pthread_create(&value_thread4, NULL, do_work2, (void *) (int *) value4);
    pthread_create(&value_thread5, NULL, do_work2, (void *) (int *) value5);
    pthread_create(&value_thread6, NULL, do_work2, (void *) (int *) value6);

    pthread_join(value_thread4, NULL);
    pthread_join(value_thread5, NULL);
    pthread_join(value_thread6, NULL);

    pthread_join(value_thread1, NULL);
    pthread_join(value_thread2, NULL);
    pthread_join(value_thread3, NULL);


    printf("Total sequences generated team1: %d\n", totalSeq);
    printf("Number of correct sequences team1: %d\n", numOfCorrectSeq);

    printf("Total sequences generated team2: %d\n", totalSeq2);
    printf("Number of correct sequences team2: %d\n", numOfCorrectSeq2);

   return 0;
}


void *do_work(void *arg)
{
   int *valueList = (int *) arg;

   int value = valueList[0];

   while(numOfCorrectSeq < desiredCorrectSeq)
   {
       usleep(500000);

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
        if(winningTeam == 2)
        {
           break;
        }

    }//end of while loop

    if(winningTeam == 0)
    {
        winningTeam = 1;
        printf("Team 1 won!\n");
    }
    
}

void *do_work2(void *arg)
{
   int *valueList2 = (int *) arg;

   int value2 = valueList2[0];

   while(numOfCorrectSeq2 < desiredCorrectSeq)
   {

       usleep(500000);
       pthread_mutex_lock(&mutex2);

       int isCorrect2 = 1; //1 is our true value

       printf("My id: %d\n", value2);

       buffer2[insertIndex2] = value2;
       insertIndex2++;

       if(insertIndex2 > 2)
       {
          for(int bufInd2 = 0; bufInd2 < 2; bufInd2++)
          {
             if( buffer2[bufInd2] != (bufInd2+4) )
             {
                isCorrect2 = 0; //the buffer is bad
             }
          }

          if(isCorrect2 == 1)
          {
             printf("456\n");
             numOfCorrectSeq2++;
          }

          insertIndex2 = 0; //reset our index
          totalSeq2++;
        }
        
        pthread_mutex_unlock(&mutex2);
        if(winningTeam == 1)
        {
           break;
        }

    }//end of while loop

        if(winningTeam == 0)
    {
        winningTeam = 2;
        printf("Team 2 won!\n");
    }

}
