#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>

void random_sleep(int a, int b);
void *do_work(void* args);

//define structure
struct arguments 
{
    int id;
    pthread_mutex_t *mutex;
    pthread_mutex_t *mutex2;
    char baker_type;
    pthread_mutex_t *right_mittenL;
    pthread_mutex_t *left_mittenL;
    int *right_mittens;
    int *left_mittens;
};

#define NUM_ITERATIONS 10
#define NUM_LEFT_OVEN_MITTS 3
#define NUM_RIGHT_OVEN_MITTS 3

/* Helper function to sleep a random number of microseconds
 * picked between two bounds (provided in seconds)
 */
void random_sleep(int lbound_sec, int ubound_sec) {
   int num_usec;
   num_usec = lbound_sec*100000 + 
	      (int)((ubound_sec - lbound_sec)*100000 * ((double)(rand()) / RAND_MAX)); 
   usleep(num_usec);
   return; 
}

/*
 * Main function
 */
int main(int argc, char **argv) {

  int num_left_handed_bakers;
  int num_right_handed_bakers;
  int num_cautious_bakers;
  int seed;

  /* Process command-line arguments */
  if (argc != 5) {
    fprintf(stderr,"Usage: %s <# left-handed bakers> <# right-handed bakers> <# cautious bakers> <seed>\n",argv[0]);
    exit(1);
  }

  if ((sscanf(argv[1],"%d",&num_left_handed_bakers) != 1) || 
      (sscanf(argv[2],"%d",&num_right_handed_bakers) != 1) || 
      (sscanf(argv[3],"%d",&num_cautious_bakers) != 1) || 
      (sscanf(argv[4],"%d",&seed) != 1) ||
      (num_left_handed_bakers < 1) ||
      (num_right_handed_bakers < 1) ||
      (num_cautious_bakers < 1) ||
      (seed < 0)) {
    fprintf(stderr,"Invalid command-line arguments... Aborting\n");
    exit(1);
  }
 
  /* Seed the RNG */
  srand(seed);

  // IMPLEMENTED CODE //// By Caleb Johnson
    pthread_mutex_t lock;
    pthread_mutex_t lock2;
    pthread_mutex_t right;
    pthread_mutex_t left;
    
    int left_count = NUM_LEFT_OVEN_MITTS;
    int right_count = NUM_RIGHT_OVEN_MITTS;
    
    pthread_t left_bakers[num_left_handed_bakers];
    pthread_t right_bakers[num_right_handed_bakers];
    pthread_t cautious_bakers[num_cautious_bakers];
 
    int total_size = num_left_handed_bakers +
                     num_right_handed_bakers + 
                     num_cautious_bakers;
         
    struct arguments* total_args[total_size];
    int args_counter = 0;

    pthread_mutex_init(&lock, NULL);
    pthread_mutex_init(&lock2, NULL);
    pthread_mutex_init(&right, NULL);
    pthread_mutex_init(&left, NULL);

    //create all left hand bakers
    for(int i = 0; i < num_left_handed_bakers; i++)
    {
        struct arguments* args;
        args = (struct arguments *)calloc(1, sizeof(struct arguments));

        args->mutex = &lock;
        args->mutex2 = &lock2;
        args->right_mittenL = &right;
        args->left_mittenL = &left;
        args->left_mittens = &left_count;
        args->right_mittens = &right_count;
        args->id = i;
        args->baker_type = 'L';
        total_args[args_counter] = args;
        if(pthread_create( &left_bakers[i], NULL, do_work,
                                 (void *) total_args[args_counter] ))
        {
            fprintf(stderr, "Error while creating left baker thread \n");
            exit(1);
        }
        args_counter++;
    }

    //create all right hand bakers
    for(int i = 0; i < num_right_handed_bakers; i++)
    {
        struct arguments* args;
        args = (struct arguments *)calloc(1, sizeof(struct arguments));

        args->mutex = &lock;
        args->mutex2 = &lock2;
        args->right_mittenL = &right;
        args->left_mittenL = &left;
        args->left_mittens = &left_count;
        args->right_mittens = &right_count;
        args->id = i;
        args->baker_type = 'R';
        total_args[args_counter] = args;
        if(pthread_create( &right_bakers[i], NULL, do_work, 
                                  (void *) total_args[args_counter] ))
        {
            fprintf(stderr, "Error while creating right baker thread \n");
            exit(1);
        }
        args_counter++;
    }

    //create all cautious bakers
    for(int i = 0; i < num_cautious_bakers; i++)
    {
        struct arguments* args;
        args = (struct arguments *)calloc(1, sizeof(struct arguments));

        args->mutex = &lock;
        args->mutex2 = &lock2;
        args->right_mittenL = &right;
        args->left_mittenL = &left;
        args->left_mittens = &left_count;
        args->right_mittens = &right_count;
        args->id = i;
        args->baker_type = 'C';
        total_args[args_counter] = args;
        if(pthread_create( &cautious_bakers[i], NULL, do_work, 
                                  (void *) total_args[args_counter] ))
        {
            fprintf(stderr, "Error while creating cautious baker thread \n");
            exit(1);
        }
        args_counter++;
    }
    

    //join all right hand bakers
    for(int i = 0; i < num_right_handed_bakers; i++)
    {
        if(pthread_join(right_bakers[i], NULL))
        {
            fprintf(stderr, "Error while joining right hand baker thread \n");
            exit(1);
        }
    }
    
    //join all cautious bakers
    for(int i = 0; i < num_cautious_bakers; i++)
    {
        if(pthread_join(cautious_bakers[i], NULL))
        {
            fprintf(stderr, "Error while joining cautious baker thread \n");
            exit(1);
        }
    }
    
    //join all left hand bakers
    for(int i = 0; i < num_left_handed_bakers; i++)
    {
        if(pthread_join(left_bakers[i], NULL))
        {
            fprintf(stderr, "Error while joining left hand baker thread \n");
            exit(1);
        }
    }


  exit(0);
}

void *do_work(void* args)
{
    //devoid the typing of the bakers arguments
    struct arguments* baker_arg = (struct arguments *) args;
    
    //each baker work for the set amount of numerations
    for(int i = 0; i < NUM_ITERATIONS; i++)
    {
        //switch case to state type, id, and start of work
        switch(baker_arg->baker_type)
        {
            case 'L':
            printf("[Left-handed baker %d] is working...\n", baker_arg->id);
            break;

            case 'R':
            printf("[Right-handed baker %d] is working...\n", baker_arg->id);
            break;

            case 'C':
            printf("[Cautious baker %d] is working...\n", baker_arg->id);
            break;
        }

        //Try to get oven mitts based on baker type
        //lock the getting of mittens to ensure mutal exclusion
        pthread_mutex_lock(baker_arg->mutex);
        switch(baker_arg->baker_type)
        {
            //left baker gets mitts
            case 'L':
            printf("[Left-handed baker %d] wants a left-handed mitt...\n",
                                                           baker_arg->id);
            
            pthread_mutex_lock(baker_arg->left_mittenL);
            *baker_arg->left_mittens = *baker_arg->left_mittens - 1;
            if(*baker_arg->left_mittens > 0)
            {
                pthread_mutex_unlock(baker_arg->left_mittenL);
            }
            
            printf("[Left-handed baker %d] has got a left-handed mitt...\n",
                                                           baker_arg->id);
            break;

            //right hand baker gets mitts
            case 'R':
            printf("[Right-handed baker %d] wants a right-handed mitt...\n",
                                                           baker_arg->id);
                                                           
            pthread_mutex_lock(baker_arg->right_mittenL);
            *baker_arg->right_mittens = *baker_arg->right_mittens - 1;
            if(*baker_arg->right_mittens > 0)
            {
                pthread_mutex_unlock(baker_arg->right_mittenL);
            }
            printf("[Right-handed baker %d] has got a right-handed mitt...\n",
                                                           baker_arg->id);
            break;

            //cautious baker gets mitts
            case 'C':
            printf("[Cautious baker %d] wants a left-handed mitt...\n",
                                                           baker_arg->id);
            pthread_mutex_lock(baker_arg->left_mittenL);
            *baker_arg->left_mittens = *baker_arg->left_mittens - 1;
            if(*baker_arg->left_mittens > 0)
            {
                pthread_mutex_unlock(baker_arg->left_mittenL);
            }
            printf("[Cautious baker %d] has got a left-handed mitt...\n",
                                                           baker_arg->id);
            
            //right mitt after left mitt
            printf("[Cautious baker %d] wants a right-handed mitt...\n",
                                                           baker_arg->id);
            pthread_mutex_lock(baker_arg->right_mittenL);
            *baker_arg->right_mittens = *baker_arg->right_mittens - 1;
            if(*baker_arg->right_mittens > 0)
            {
                pthread_mutex_unlock(baker_arg->right_mittenL);
            }
            printf("[Cautious baker %d] has got a right-handed mitt...\n",
                                                           baker_arg->id);
            break;
        }
        pthread_mutex_unlock(baker_arg->mutex);
        
        //place cookies in oven then wait
        switch(baker_arg->baker_type)
        {
            //left baker gets mitts
            case 'L':
            printf("[Left-handed baker %d] has put the cookies in the oven and is waiting...\n",
                                                           baker_arg->id);
            break;

            //right hand baker gets mitts
            case 'R':
            printf("[Right-handed baker %d] has put the cookies in the oven and is waiting...\n",
                                                           baker_arg->id);
            break;

            //cautious baker gets mitts
            case 'C':
            printf("[Cautious baker %d] has put the cookies in the oven and is waiting...\n",
                                                           baker_arg->id);
            break;
        }
        //wait for some time to "bake"
        random_sleep(2, 5);

        //done waiting take cookies out of oven
        switch(baker_arg->baker_type)
        {
            //left baker gets mitts
            case 'L':
            printf("[Left-handed baker %d] has taken cookies out of the oven...\n",
                                                           baker_arg->id);
            break;

            //right hand baker gets mitts
            case 'R':
            printf("[Right-handed baker %d] has taken cookies out of the oven...\n",
                                                           baker_arg->id);
            break;

            //cautious baker gets mitts
            case 'C':
            printf("[Cautious baker %d] has taken cookies out of the oven...\n",
                                                           baker_arg->id);
            break;
        }
        
        //return oven mitts based on baker type
        pthread_mutex_lock(baker_arg->mutex2);
        switch(baker_arg->baker_type)
        {
            //left baker gets mitts
            case 'L':
            printf("[Left-handed baker %d] has put back a left-handed mitt...\n",
                                                           baker_arg->id);
            if(*baker_arg->left_mittens == 0)
            {
                *baker_arg->left_mittens = *baker_arg->left_mittens + 1;
                pthread_mutex_unlock(baker_arg->left_mittenL);
            }
            else
            {
                *baker_arg->left_mittens = *baker_arg->left_mittens + 1;
            }

            break;

            //right hand baker gets mitts
            case 'R':
            printf("[Right-handed baker %d] has put back a right-handed mitt...\n",
                                                           baker_arg->id);
            if(*baker_arg->right_mittens == 0)
            {
                *baker_arg->right_mittens = *baker_arg->right_mittens + 1;
                pthread_mutex_unlock(baker_arg->right_mittenL);
            }
            else
            {
                *baker_arg->right_mittens = *baker_arg->right_mittens + 1;
            }

            break;

            //cautious baker gets mitts
            case 'C':
            printf("[Cautious baker %d] has put back a left-handed mitt...\n",
                                                           baker_arg->id);
            if(*baker_arg->left_mittens == 0)
            {
                *baker_arg->left_mittens = *baker_arg->left_mittens + 1;
                pthread_mutex_unlock(baker_arg->left_mittenL);
            }
            else
            {
                *baker_arg->left_mittens = *baker_arg->left_mittens + 1;
            }
            
            //right mitt after left mitt
            printf("[Cautious baker %d] has put back a right-handed mitt...\n",
                                                           baker_arg->id);
            if(*baker_arg->right_mittens == 0)
            {
                *baker_arg->right_mittens = *baker_arg->right_mittens + 1;
                pthread_mutex_unlock(baker_arg->right_mittenL);
            }
            else
            {
                *baker_arg->right_mittens = *baker_arg->right_mittens + 1;
            }

            break;
        }
        pthread_mutex_unlock(baker_arg->mutex2);

    }//end of for loop for baker working iterations
}


