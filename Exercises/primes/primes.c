#include <stdio.h>
#include <pthread.h>
#include <string.h>

void supported_commands()
{
	printf("Supported commands:\n");
	printf("p N - Starts a new calculation for the number of primes from 1 to N\n");
	printf("e - Waits for all calculations to finish and exits\n");
}	

int is_for_exit(char* command)
{
	return((command[0] == 'e') && (strlen(command) == 2));
}

int is_for_prime(char* command)
{
	return(command[0] == 'p' && command[1] == ' ');
}

int make_number_from_array(char* array, int size)
{
	int number_for_return = 0;
	
	for(int i = 0; i < size; i++)
	{
		number_for_return += array[i] - '0';
		number_for_return *= 10;
	}
	
	number_for_return /= 10;
	
	return number_for_return;
}

void* finding_primes(void* up_to_number)
{
	int actual_number, count_of_primes = 0;
	int flag = 0;
	actual_number = (int)up_to_number;
	
	for(int i = 2; i < actual_number; i++)
	{
		flag = 0;
		
		for(int j = 2; j < i; j++)
		{
			if(i % j == 0)
			{
				flag = 1;
				break;
			}
			
		}
		if(!flag)
		{
			count_of_primes++;
		}
		
	}
	
	printf("Number of primes for N=%d is %d\n",actual_number ,count_of_primes);
	return 0;
}

int main()
{
	char for_command[200];

	while(1)
	{
		fgets(for_command, 200, stdin);
		
		if(is_for_exit(for_command))
		{
			break;
		}
		
		else if(is_for_prime(for_command))
		{
			char array[strlen(for_command) - 3];
			int size = sizeof(array)/sizeof(char);
			int j = 0;
			
			for(int i = 2; i < strlen(for_command) - 1; i++)
			{
				if(for_command[i] >= '0' && for_command[i] <= '9')
				{
					strcpy(&array[j], &for_command[i]);
					j++;
				}
				
				else
				{
					supported_commands();
					break;
				}
			}
			
			
			int number = make_number_from_array(array, size);
			printf("Prime calculation started for N=%d\n", number);
			
			pthread_t thread;
			pthread_create(&thread, NULL, finding_primes, (void*)number);
		}
		
		else
		{
			supported_commands();
		}
	
	}
	
	pthread_exit(NULL);
	return 0;
}
