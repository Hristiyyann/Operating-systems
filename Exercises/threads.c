#include <stdio.h>
#include <pthread.h>
#include <wait.h>
#include <stdlib.h>
#include <unistd.h>

int total;
pthread_mutex_t numbers;

void* print_id(void* thread_id)
{
	int id;
	int rand_num;
	id = (int)thread_id;
	
	printf("ID OF THREAD %d\n", id);
	
	
	rand_num = (rand() % 1) * 0xFFFFFFF;
	
	for(int i = 0; i < rand_num; i++);
	
	pthread_mutex_lock(&numbers);
	
	total += id;
	
	pthread_mutex_unlock(&numbers);
	
	printf("THREAD %d is going to return\n", id); 
}

int main()
{
	int size = 170;
	pthread_t threads[size];
	pthread_mutex_init(&numbers, NULL);
	
	for(int i = 0; i < size; i++)
	{
		printf("THREAD %d is starting\n", i+1);
		pthread_create(&threads[i], NULL, print_id, (void *)i + 1);
	}
	
	for(int j = 0; j < size; j++)
	{
		pthread_join(threads[j], NULL);
	}
	
	pthread_mutex_destroy(&numbers);
	
	printf("Total - %d\n", total);
	
	return 0;
}
