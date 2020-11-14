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


void* plus(void*);
char* get_buff_2();
char* do_plus(char*);
void* put_buff_3(char*);


char* get_buff_3();

char buffer[50000];
char buffer_1[1000];
char buffer_2[1000];
char buffer_3[1000];
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


//third thread
int count_3 = 0;
int prod_idx_3 = 0;
int con_ind_3 = 0;
pthread_mutex_t mutex_3 = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t full_3 = PTHREAD_COND_INITIALIZER;





int main(int argc, char* argv[])
{
  
	pthread_t input_t, linSep_t, plus_t, output_t;

	pthread_create(&input_t, NULL, get_input, NULL);
	pthread_create(&linSep_t, NULL, lineSeperator, NULL);
	pthread_create(&plus_t, NULL,plus, NULL);
	

	pthread_join(input_t,NULL);
	pthread_join(linSep_t,NULL);
	pthread_join(plus_t,NULL);


}





// sets buffer 3 value
void* put_buff_3(char* x)
{
	
	pthread_mutex_lock(&mutex_3);
   	strcpy(buffer_3,x);
  	prod_idx_3 = strlen(buffer_1 + 1);
	count_3++;
  	pthread_cond_signal(&full_3);
   	pthread_mutex_unlock(&mutex_3);

}






// Changes ++ to ^
char* do_plus(char* x)
{
	char temp_buffer[1000] = {0};
	char* substring = "++";
	char* replacement_string = "^";
	char* insert_point = &temp_buffer[0];
	const char* temp = x;
	size_t needle_len = strlen(substring);
	size_t repl_len = strlen(replacement_string);

	while(1)
	{
		const char* p = strstr(temp,substring);
		if(p == NULL)
		{
			strcpy(insert_point,temp);
			break;
		}


		memcpy(insert_point, temp, p-temp);
		insert_point += p- temp;
		
		memcpy(insert_point,replacement_string,repl_len);
		insert_point = insert_point + repl_len;

		temp = p + needle_len;

	}

	printf("%s",temp_buffer);

	strcpy(x,temp_buffer);
	return x;
	
}







//Third thread 
void* plus(void* x)
{

	put_buff_3(get_buff_2());
	return NULL;
}






//waits for line seperator 
char* get_buff_2()
{
	pthread_mutex_lock(&mutex_2);
	while(count_2 == 0)
	{
		 pthread_cond_wait(&full_2, &mutex_2);

	}

	count_2--;
	do_plus(buffer_2);
	pthread_mutex_unlock(&mutex_2);
	return buffer_2;

	
	

}




//adds string to second buffer
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
