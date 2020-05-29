#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h> 

#define SIZE 50

double do_crazy_computation(int i,int j);

int main(int argc, char **argv) {
  double mat[SIZE][SIZE];
  int i,j;



  //modified code here
  omp_set_num_threads(2);
  double tstart=omp_get_wtime();
  double thread1Time;
  double thread2Time;
  int threadFin = 0;
  double loadImb = 0;
  
  #pragma omp parallel private(i, j) shared(mat, loadImb)
  {
	  #pragma omp for schedule(static) nowait
	  for (i=0;i<SIZE;i++) { /* loop over the rows */
		for (j=0;j<SIZE;j++) {  /* loop over the columns */
			mat[i][j] = do_crazy_computation(i,j);
			  fprintf(stderr,".");
	    }
	  }
	  
	  #pragma omp critical
	  if(threadFin == 0)
	  {
	    double tend = omp_get_wtime();
	    thread1Time = tend - tstart;
	    threadFin++;
	  }
	  else
	  {
	    double tend = omp_get_wtime();
	    thread2Time = tend - tstart;
	  }
  }
 
  loadImb = thread2Time - thread1Time;
  
  if(loadImb < 0)
	    {
	        loadImb = loadImb * -1;
	    }
  double tend=omp_get_wtime();
  double elapsed=tend - tstart;
  printf("\nTotal time (Sanity): %f seconds\n",elapsed);
  printf("Thread 1 time: %f seconds\n",thread1Time);
  printf("Thread 2 time: %f seconds\n",thread2Time);
  printf("Load imbalence: %f seconds\n", loadImb);
  

  //For testing, set to 0 if not testing
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

