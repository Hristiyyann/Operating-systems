//--------------------------------------------
// NAME: Hristiyan Radev
// CLASS: Xib
// NUMBER: 25
// PROBLEM: Warcraft
//---------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>

int mine_gold = 2000;
int collected_gold = 0;
int current_gold = 0;
int farms = 0;
int farms_builder_called = 0;

pthread_mutex_t mine_mutex;
pthread_mutex_t base_mutex;
pthread_mutex_t farms_mutex;

//------------------------------------------------------------------------
// FUNCTION: build_farm
// Тази функция ще се използва за построяването на ферми.
// PARAMETERS: Няма параметри.
//------------------------------------------------------------------------
void *build_farm()
{
	int status; 
	sleep(2);
	
	if(status = pthread_mutex_lock(&farms_mutex) != 0)
	{
		fprintf(stderr, "Mutex lock: %s\n", strerror(status));
	}
	
	++farms;
	printf("Farm %d created\n", farms);
	
	
	if(status = pthread_mutex_unlock(&farms_mutex) != 0)
	{
		fprintf(stderr, "Mutex unlock: %s\n", strerror(status));
	}
}

//------------------------------------------------------------------------
// FUNCTION: mining
// Функцията ще се използва от работниците, за да копаят злато.
// PARAMETERS: void* id - това е поредния номер на работника.
//------------------------------------------------------------------------
void *mining(void* id)
{
	int worker_gold = 0;
	int worker_id, status;
	worker_id = (int)id;
	
	while(1)
	{
		if(status = pthread_mutex_lock(&mine_mutex) != 0)
		{
			fprintf(stderr, "Mutex lock: %s\n", strerror(status));
		}
		
		if(mine_gold == 0)
		{
			printf("Worker %d exiting\n", worker_id);
			if(status = pthread_mutex_unlock(&mine_mutex) != 0)
			{
				fprintf(stderr, "Mutex unlock: %s\n", strerror(status));
			}
			break;
		}
		
		printf("Worker %d entered in mine 1\n", worker_id);
		
		if(mine_gold  < 10)
		{
			worker_gold += mine_gold;
			mine_gold = 0;
		}
		
		else
		{
			worker_gold += 10;
			mine_gold -=10;
		}
		
		collected_gold += worker_gold;
		
		if(status = pthread_mutex_unlock(&mine_mutex) != 0)
		{
			fprintf(stderr, "Mutex unlock: %s\n", strerror(status));
		}
		
		sleep(1);
		
		if(status = pthread_mutex_lock(&base_mutex))
		{
			fprintf(stderr, "Mutex lock: %s\n", strerror(status));
		}
		
		printf("Worker %d depositing\n", worker_id);
		current_gold += worker_gold;
		worker_gold = 0;
		
		if(current_gold >= 50)
		{
			if(farms_builder_called < 20)
			{
				pthread_t builder;
				if(status = pthread_create(&builder, NULL, build_farm, NULL) != 0)
				{
					fprintf(stderr, "Pthread create: %s\n", strerror(status));
				}
				current_gold -=50;
				farms_builder_called++;
			}
		}
		
		if(status = pthread_mutex_unlock(&base_mutex) != 0)
		{
			fprintf(stderr, "Mutex unlock: %s\n", strerror(status));
		}	
	}
}


int main(int argc, char **argv)
{
	int workers = 2;
	int status;
	
	if(argc > 1)
	{
		workers = atoi(argv[1]);
	}
	
	pthread_t all_workers[workers];
	
	if(status = pthread_mutex_init(&mine_mutex, NULL) != 0)
	{
		fprintf(stderr, "Mutex init: %s\n", strerror(status));
	}
	
	if(status = pthread_mutex_init(&farms_mutex, NULL) != 0)
	{
		fprintf(stderr, "Mutex init: %s\n", strerror(status));
	}
	
	if(status = pthread_mutex_init(&base_mutex, NULL) != 0)
	{
		fprintf(stderr, "Mutex init: %s\n", strerror(status));
	}
	
	for(int i = 0; i < workers; i++)
	{
		if(status = pthread_create(&all_workers[i], NULL, mining, (void*)i) != 0)
		{
			fprintf(stderr, "Pthread create: %s\n", strerror(status));
		}
	}
	
	for(int i = 0; i < workers; i++)
	{
		if(status = pthread_join(all_workers[i], NULL) != 0)
		{
			fprintf(stderr, "Pthread join: %s\n", strerror(status));
		}
	}
	
	
	while(1)
	{
		if(status = pthread_mutex_lock(&farms_mutex) != 0)
		{
			fprintf(stderr, "Mutex lock: %s\n", strerror(status));
		}
	
		if(farms == 20)
		{
			if(status = pthread_mutex_unlock(&farms_mutex) != 0)
			{
				fprintf(stderr, "Mutex unlock: %s\n", strerror(status));
			}
			break;
		}
			
		if(status = pthread_mutex_unlock(&farms_mutex) != 0)
		{
			fprintf(stderr, "Mutex unlock: %s\n", strerror(status));
		}
	}	

	if(status = pthread_mutex_destroy(&mine_mutex) != 0)
	{
		fprintf(stderr, "Mutex destroy: %s\n", strerror(status));
	}
	
	if(status = pthread_mutex_destroy(&base_mutex) != 0)
	{
		fprintf(stderr, "Mutex destroy: %s\n", strerror(status));
	}
	
	if(status = pthread_mutex_destroy(&farms_mutex) != 0)
	{
		fprintf(stderr, "Mutex destroy: %s\n", strerror(status));
	}
	
		
	printf("Gold in mine: %d Collected gold: %d Player gold: %d Farms: %d\n", mine_gold, collected_gold, current_gold, farms);
	
	return 1;
}
