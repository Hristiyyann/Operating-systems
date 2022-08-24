#include <stdio.h>
#include <pthread.h>
#include <wait.h>
#include <stdlib.h>
#include <unistd.h>

//--------------------------------------------
// NAME: Hristiyan Radev
// CLASS: XI b
// NUMBER: 25
// PROBLEM: #miners
//---------------------------------------------

int total_gold = 0;
#define gold 10

pthread_mutex_t gold_mutex;


void* mining(void* miner_id)
{
	int id;
	id = (int)miner_id;
	
	for(int i = 0; i < 20; i++)
	{
		pthread_mutex_lock(&gold_mutex);
		
		total_gold += 10;
		
		pthread_mutex_unlock(&gold_mutex);
		
		printf("Miner %d gathered 10 gold\n", id);
		
		sleep(2);
	}
	
	return NULL;
}

void* trading(void *trader_id)
{
	int id;
	id = (int)trader_id;
	
	for(int i = 0; i < 20; i++)
	{
		pthread_mutex_lock(&gold_mutex);
		
		if(total_gold < 10)
		{
			printf("The warehouse is empty, cannot sell!\n");
		}
		
		else
		{
			total_gold -=10;
			printf("Trader %d sold 10 gold\n", id);
		}
		
		pthread_mutex_unlock(&gold_mutex);
		sleep(2);
	}
	
	return NULL;
}


int main(int argc, char** argv)
{
	int i;
	int miners, traders;

	
	pthread_mutex_init(&gold_mutex, NULL);
	
	if(argc == 1)
	{
		miners = 1;
		traders = 1;
	}
	
	else
	{
		miners = atoi(argv[1]);
		traders = atoi(argv[2]);
	}

	pthread_t total_miners[miners];
	pthread_t total_traders[traders];
	
	
	for(i = 0; i < miners; i++)
	{
		if(pthread_create(&total_miners[i], NULL, mining, (void*)i + 1));
		{
			perror("phtred creating");
		}
	}
	
	for(i = 0; i < traders; i++)
	{
		if(pthread_create(&total_traders[i], NULL, trading, (void*)i + 1) == -1)
		{
			perror("phtred creating");
		}
	}
	
	for(i = 0; i < miners; i++)
	{
		if(pthread_join(total_miners[i], NULL) == -1)
		{
			perror("pthread joing");
		}
	}
	
	for(i = 0; i < traders; i++)
	{
		if(pthread_join(total_traders[i], NULL) == -1)
		{
			perror("pthread joing");
		}
	}
	
	
	printf("Gold: %d\n", total_gold);
	pthread_mutex_destroy(&gold_mutex);
	
	
}
