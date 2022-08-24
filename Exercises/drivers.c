#include <stdio.h>
#include <pthread.h>
#include <wait.h>
#include <stdlib.h>
#include <unistd.h>


pthread_mutex_t car;

void* take_car(void* driver_id)
{
	int id;
	id = (int)driver_id;
	
	while(1)
	{
		if(pthread_mutex_trylock(&car) == 0)
		{
			printf("Driver %d take the car\n", id);
			sleep(1);
			printf("Driver %d going out of the car\n", id);
			pthread_mutex_unlock(&car);
		}
		
		else
		{
			printf("Driver %d is waiting\n", id);
		}
		sleep(1);
		
	}
}

int main()
{
	
	int size = 5;
	pthread_t threads[size];
	pthread_mutex_init(&car, NULL);
	
	
	for(int i = 0; i < size; i++)
	{
		printf("Driver %d is starting\n", i+1);
		pthread_create(&threads[i], NULL, take_car, (void *)i + 1);
	}
	
	
	while(1);
	
	pthread_mutex_destroy(&car);
	
	return 0;
}
