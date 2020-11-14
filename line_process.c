#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <math.h>
#include <assert.h>
#include <string.h>



//#define CH 1000;
//#define LI 50;
#define END_MARKER -1;


void* get_input(void*);
void* put_buff_1(char*);
char* get_user_input();
void* lineSeperator(void*);
char* do_line(char*);
char* get_buf_1();
void* put_buff_2(char*);


char buffer[50000];
char buffer_1[1000];
char buffer_2[1000];
int globalVar = 0;



//for first input thread
int count_1 = 0;
int prod_idx_1 = 0;
int con_ind_1 = 0;
pthread_mutex_t mutex_1 = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t full_1 = PTHREAD_COND_INITIALIZER;
pthread_cond_t empty = PTHREAD_COND_INITIALIZER;


//second thread producer
int count_2 = 0;
int prod_idx_2 = 0;
int con_ind_2 = 0;
pthread_mutex_t mutex_2 = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t full_2 = PTHREAD_COND_INITIALIZER;






int main(int argc, char* argv[])
{
  
	pthread_t input_t, linSep_t, plus_t, output_t;

	pthread_create(&input_t, NULL, get_input, NULL);
	pthread_create(&linSep_t, NULL, lineSeperator, NULL);

	

	pthread_join(input_t,NULL);
	pthread_join(linSep_t,NULL);


}




void* put_buff_2(char* x)
{
	
	pthread_mutex_lock(&mutex_2);
   	strcpy(buffer_2,x);
  	prod_idx_1 = strlen(buffer_1 + 1);
	count_2++;
  	pthread_cond_signal(&full_2);
   	pthread_mutex_unlock(&mutex_2);

}





//waits for first thread to finish then calls function to swap \n then returns line
char* get_buff_1()
{
	pthread_mutex_lock(&mutex_1);
  	while (count_1 == 0)
	{

   	 // Buffer is empty. Wait for the producer to signal that the buffer has data
         pthread_cond_wait(&full_1, &mutex_1);

	 }
	
	count_1--;
	
	do_line(buffer_1);
	pthread_mutex_unlock(&mutex_1);
	
	return buffer_1;
      
}







//gets ride of \n
char* do_line(char* x)
{

	x[prod_idx_1] = ' ';
	return x;

}






//Second thread
void* lineSeperator(void*x)
{

	put_buff_2(get_buff_1());
	return NULL;
 }








//reads in user input returns it to input thread function
char* get_user_input()
{

    char* user_input = malloc(sizeof(char)*1000);
    fgets(user_input,1000,stdin);
	
    return user_input;
}




//Takes in user input and puts it in buffer for first thread
void *get_input(void* x)
{
   
      put_buff_1(get_user_input());
   
      return NULL;

}






// reads in user input and adds it to input buffer
void* put_buff_1(char* x)
{
// //  Lock the mutex before putting the item in the buffer
   pthread_mutex_lock(&mutex_1);
//   // Put the item in the buffer
   strcpy(buffer_1,x);
//   // Increment the index here the next item will be put.
   prod_idx_1 = strlen(buffer_1 + 1);
	count_1++;
//   // Signal to the consumer that the buffer is no longer empty
   pthread_cond_signal(&full_1);
//   // Unlock the mutex
   pthread_mutex_unlock(&mutex_1);
}
