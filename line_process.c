#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <math.h>
#include <assert.h>
#include <string.h>
#include <stdbool.h>






void* get_input(void*);
void* put_buff_1(char);
char* get_user_input();


void* lineSeperator(void*);
char* do_line(char*);
char get_buf_1();
void* put_buff_2(char);


void* plus(void*);
char get_buff_2();
char* do_plus(char*);
void* put_buff_3(char);


char get_buff_3();
void* store_output(void*);
char* check_stop(char*);


char buffer_1[50000];
char buffer_2[50000];
char buffer_3[50000];
int globalVar = 0;






//for first input thread
int count_1 = 0;
int prod_idx_1 = 0;
int con_idx_1 = 0;
pthread_mutex_t mutex_1 = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t full_1 = PTHREAD_COND_INITIALIZER;
pthread_cond_t empty = PTHREAD_COND_INITIALIZER;


//second thread producer
int count_2 = 0;
int prod_idx_2 = 0;
int con_idx_2 = 0;
pthread_mutex_t mutex_2 = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t full_2 = PTHREAD_COND_INITIALIZER;


//third thread
int count_3 = 0;
int prod_idx_3 = 0;
int con_idx_3 = 0;
pthread_mutex_t mutex_3 = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t full_3 = PTHREAD_COND_INITIALIZER;




int main(int argc, char* argv[])
{	
	

	pthread_t input_t, linSep_t, plus_t, output_t;

	pthread_create(&input_t, NULL, get_input, NULL);
	pthread_create(&linSep_t, NULL, lineSeperator, NULL);
	pthread_create(&plus_t, NULL,plus, NULL);
	pthread_create(&output_t, NULL,store_output, NULL);


	pthread_join(input_t,NULL);
	pthread_join(linSep_t,NULL);
	pthread_join(plus_t,NULL);
	pthread_join(output_t,NULL);


}




//changes string if there is a stop statement then raises a global var for terminaltion of threads
char* check_stop(char* x)
{

	char stop[7];
	char* ptr = &stop[0];
	sprintf(ptr,"STOP\n");

	char* stop_ptr = strstr(x,ptr);


	if(strcmp(ptr,stop_ptr) == 0)
	{
		globalVar = -1;
		char new_string[1000];
		strncpy(new_string,x,sizeof(x-ptr));
		printf("%s",new_string);
		return NULL;
	}

	else
	{
		return x;
	}

}





// 4th thread handles the output and keeping count of the number of lines to print

void* store_output(void* x)
{
	
	int i = 0;
	int j = 0;
	char buf [50000];
	char item = 0;
	
	while(1)
	{
		item = get_buff_3();
		if(item == 1)
		{
			break;	
		}
		buf[i] = item;
		i++;
		if(i == 81)
		{
			printf("%s\n", buf);
			i = 0;
		}
	}
	
	exit(1);

}









// waits for third buffer then checks for 80 chars in that buffer to be printed
char get_buff_3()
{

	
	pthread_mutex_lock(&mutex_3);
	while(count_3 == 0)
	{
		pthread_cond_wait(&full_3, &mutex_3);
	}
	char item = buffer_3[con_idx_3];
	con_idx_3++;
	count_3--;
	pthread_mutex_unlock(&mutex_3);

	return item;

}





// sets buffer 3 value
void* put_buff_3(char x)
{
	
	pthread_mutex_lock(&mutex_3);
	buffer_3[prod_idx_3] = x;
  	prod_idx_3++;
	count_3++;
  	pthread_cond_signal(&full_3);
	pthread_mutex_unlock(&mutex_3);



}









// Changes ++ to ^
char* do_plus(char* x)
{
	

	char temp_buffer[50000] = {0};
	char* substring = "++";
	char* replacement_string = "^";
	char* insert_point = &temp_buffer[0];
	const char* temp = x;
	size_t needle_len = strlen(substring);
	size_t repl_len = strlen(replacement_string);

	while(1)
	{
		 char* p = strstr(temp,substring);
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



	strcpy(x,temp_buffer);

	return x;
	
}







//Third thread grabs 2 char
void* plus(void* x)
{
	char newChar = '^';
	char pluss = '+';
	char item = 0;
	char item2 = 0;
	char item3 = 0;
	for(int i = 0; i < 50000; i++)
	{
		
		item = get_buff_2();
		
		if(item == 1)
		{
			put_buff_3(1);
			break;
		}
		item2 = get_buff_2();

		if(item3 == item)
		{
			item = newChar;
			put_buff_3(item3);
		}
		
		else if(item == pluss && item2 == pluss)
		{
			item = newChar;
			put_buff_3(item);
			item3 = item2;
		}
		
		
			
			put_buff_3(item);
			put_buff_3(item2);
			
		
	}	
	return NULL;
}






//waits for line seperator 
char get_buff_2()
{
	pthread_mutex_lock(&mutex_2);
	while(count_2 == 0)
	{
		 pthread_cond_wait(&full_2, &mutex_2);

	}

	char item = buffer_2[con_idx_2];
	con_idx_2++;
	count_2--;
	pthread_mutex_unlock(&mutex_2);
	return item;

	
	

}




//adds char to second buffer
void* put_buff_2(char x)
{
	
	pthread_mutex_lock(&mutex_2);
	buffer_2[prod_idx_2] = x;
     	prod_idx_2++;
	count_2++;
  	pthread_cond_signal(&full_2);
   	pthread_mutex_unlock(&mutex_2);

}





//waits for first thread then accesses memory to grab buffer char
char get_buff_1()
{
	pthread_mutex_lock(&mutex_1);
  	while (count_1 == 0)
	{

         pthread_cond_wait(&full_1, &mutex_1);

	 }


	char item = buffer_1[con_idx_1];
	con_idx_1++;
	count_1--;
	pthread_mutex_unlock(&mutex_1);
	
	return item;
      
}




//Second thread grabs from buffer 1 then checks for null or STOP call
void* lineSeperator(void*x)
{

	char item = 0;
	char newChar = ' ';
	
	for(int i = 0; i < 500000; i++)
	{
		 item = get_buff_1();
	
		if( item == 1)
		{
		  put_buff_2(1);
		  break;
		}
		
		if(item =='\n')
		{
			item = newChar;

		}

		put_buff_2(item);
	}		
	
	return NULL;
		
 }














// NOT USED
//reads in user input returns it to input thread function
char* get_user_input()
{

    char* user_input = calloc(1000,sizeof(char));
	fgets(user_input,1000,stdin);
	fflush(stdin);

    return user_input;
}










//Takes in user input and puts it in buffer for first thread
void *get_input(void* x)
{
 
	   char* buffer = calloc(500000,sizeof(char));
	while(fgets(buffer,1000,stdin) != NULL && strcmp(buffer,"STOP\n"))
	{

		for(int i = 0 ; i < strlen(buffer); i++)
		{
				put_buff_1(buffer[i]);
		}
	
	}
	 
	put_buff_1(1);

		
      return NULL;

}






// reads in user input and adds it to input buffer
void* put_buff_1(char x)
{
// //  Lock the mutex before putting the item in the buffer
   pthread_mutex_lock(&mutex_1);
//   // Put the item in the buffer
  
//   char* temp = &buffer_1[prod_idx_1];

   buffer_1[prod_idx_1] = x;

//	check_stop(temp);	
//   // Increment the index here the next item will be put.
  // prod_idx_1 = strlen(buffer_1+1);
  	prod_idx_1++;
	count_1++;
//   // Signal to the consumer that the buffer is no longer empty
   pthread_cond_signal(&full_1);
//   // Unlock the mutex
   pthread_mutex_unlock(&mutex_1);

}
