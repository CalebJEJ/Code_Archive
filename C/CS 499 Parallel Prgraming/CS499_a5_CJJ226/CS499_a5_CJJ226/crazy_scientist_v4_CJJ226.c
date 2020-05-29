#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h> 
#include <pthread.h>

#define SIZE 50

double do_crazy_computation(int i,int j);
void * do_work(void * args);

struct arguments 
{
	pthread_mutex_t *mutex;
    int executeNum;
	double * endTime;
	double * startTime;
	double (* matPointer)[SIZE][SIZE];
	int row_start_num;//inclusive
	int row_end_num;//exclusive
};

int main(int argc, char **argv) {

  double mat[SIZE][SIZE];
  //modified code here

  omp_set_num_threads(2);
  double tstart=omp_get_wtime();
  double t1EndTime = 0;
  double t2EndTime = 0;
  pthread_mutex_t lock;
  
  pthread_mutex_init(&lock, NULL);
  
  
  
  struct arguments *args1;
  struct arguments *args2;
  args1 = (struct arguments *)calloc(1, sizeof(struct arguments) );
  args2 = (struct arguments *)calloc(1, sizeof(struct arguments) );
  
  args1->mutex = &lock;
  args1->executeNum = 0;
  args1->endTime = &t1EndTime;
  args1->startTime = &tstart;
  args1->matPointer = &mat;
  args1->row_start_num = 0;
  args1->row_end_num = 25;
  
  args2->mutex = &lock;
  args2->executeNum = 0;
  args2->endTime = &t2EndTime;
  args2->startTime = &tstart;
  args2->matPointer = &mat;
  args2->row_start_num = 25;
  args2->row_end_num = SIZE;
  
  pthread_t thread1;
  pthread_t thread2;

  pthread_create(&thread1, NULL, do_work, args1);
  
  pthread_create(&thread2, NULL, do_work, args2);
  
  pthread_join(thread1, NULL);
  pthread_join(thread2, NULL);
  
  printf("\n");

  printf("Thread 1 Time: %f\n", *args1->endTime);
  printf("Thread 2 Time: %f\n", *args2->endTime);
  
  printf("Thread 1 # Executions: %d\n", args2->executeNum);
  printf("Thread 2 # Executions: %d\n", args1->executeNum);
  
  double tend=omp_get_wtime();
  double elapsed=tend - tstart;
  printf("Total time (Sanity): %f seconds\n",elapsed);
  
  //subtract and get absolute value
  double loadImb = t1EndTime - t2EndTime;
  
  if(loadImb < 0)
  {
	  loadImb = loadImb*-1;
  }
  
  printf("Load imbalence: %f seconds\n", loadImb);
  
  //For testing only, set to 0 if not testing
  if(0)
  {
    int i, j;
    for (i=0;i<SIZE;i++) { /* loop over the rows */

		for (j=0;j<SIZE;j++) {  /* loop over the columns */
			 double memValue = mat[i][j];
			 printf("\nMemory Data[%d][%d]: %f\n", i, j, memValue );
		}
			
	}
  }
  

  exit(0);
}

//Crazy computation
double do_crazy_computation(int x,int y) {
   int iter;
   double value=0.0;

   for (iter = 0; iter < 5*x*x*x+1 + y*y*y+1; iter++) {
     value +=  (cos(x*value) + sin(y*value));
   }
  return value;
}

void *do_work(void * args)
{
	struct arguments *argsData = (struct arguments *) args;

  	int i,j;
	int row_start_position = argsData->row_start_num;
	int row_end_postion = argsData->row_end_num;	

	

	for (i=row_start_position;i<row_end_postion;i++) { /* loop over the rows */
			
			argsData->executeNum++;
			
			for (j=0;j<SIZE;j++) {  /* loop over the columns */
			  
			  //CRITICAL
			  pthread_mutex_lock(argsData->mutex);
			  
			  (*argsData->matPointer)[i][j] = do_crazy_computation(i,j);
			  
			  pthread_mutex_unlock(argsData->mutex);
			  //CRITICAL END
			  fprintf(stderr,".");
			}
			
	}
	
	*argsData->endTime = omp_get_wtime() - *argsData->startTime;
	
	
}
