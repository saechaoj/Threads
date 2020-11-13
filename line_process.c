#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <math.h>
#include <assert.h>

//#define CH 1000;
//#define LI 50;
#define END_MARKER -1;


void* get_input(void*);
void put_buff_1(char*);
char* get_user_input();

//handles input thread include producer from stdin and consumer line seperator
char buffer_1[1000];
int count_1 = 0;
int prod_idx_1 = 0;
int con_ind_1 = 0;
pthread_mutex_t mutex_1 = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t full_1 = PTHREAD_COND_INITIALIZER;
pthread_cond_t empty = PTHREAD_COND_INITIALIZER;



int main(int argc, char* argv[])
{
  pthread_t input_t, linSep_t, plus_t, output_t;


}



char* get_user_input()
{
    char user_input[1000];
    fgets(user_input,1000,stdin);
    return user_input;
}


//reads in user input then adds it to buffer1 this is
//the first thread
void *get_input(void* x)
{
    for(int i = 0; i <= 5 ; i++)
    {
      char x[1000];
      x[i] = get_user_input();
      if(x[i] == NULL)
      {
        break;
      }
      else
      {
        put_buff_1(x[i]);
      }
    }
      return NULL;

}


// reads in user input and adds it to input buffer
void put_buff_1(char* x)
{
// //  Lock the mutex before putting the item in the buffer
//   pthread_mutex_lock(&mutex_1);
//   // Put the item in the buffer
//   buffer_1[prod_idx_1] = x;
//   // Increment the index where the next item will be put.
//   prod_idx_1 = prod_idx_1 + 1;
//   count_1++;
//   // Signal to the consumer that the buffer is no longer empty
//   pthread_cond_signal(&full_1);
//   // Unlock the mutex
//   pthread_mutex_unlock(&mutex_1);
}
