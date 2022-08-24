//------------------------------------------------------------------------
// NAME: Hristiyan Radev
// CLASS: XIb
// NUMBER: 25
// PROBLEM: #1
// FILE NAME: shell.c (unix file name)
// FILE PURPOSE:
// Да се направи прост шел
// 
// ...
//------------------------------------------------------------------------

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>

//------------------------------------------------------------------------
// FUNCTION: free_pointers(име на функцията)
// Функцията има за цел да освободи паметта, която е заделена за аргументите, които се 
// подават на execv 
// PARAMETERS:
// char** arguments - това е пойнтъра, от който ще освободим паметта, заделена за 
// аргументите, а после ще осводободим и самия пойнтър
//
//------------------------------------------------------------------------
void free_pointers(char** arguments)
{
	for(int i = 0; arguments[i] != NULL; i++)
	{
		free(arguments[i]);
	}
	
	free(arguments);
}

//------------------------------------------------------------------------
// FUNCTION: parse_cmdline (име на функцията)
// Функцията трябва да раздели стринг в масив от стрингове. 
// PARAMETERS:
// const char* cmdline - от този стринг ще направим масива от стрингове
// 
//------------------------------------------------------------------------
char** parse_cmdline(const char* cmdline)
{
	int count_delimiters = 1;
	char* new_line = malloc(strlen(cmdline)); // Правя копие на cmdline, защото е константен и не мога да го променя. 

	strcpy(new_line, cmdline);
	
	for(int i = 0; i < strlen(new_line); i++)
	{
		if(new_line[i] == ' ')
		{
			count_delimiters++;
		}
	}
	
	int size = count_delimiters + 1; // Тук е +1, за да може да има място и за NULL
	char** command_line_arguments = malloc(sizeof(char*) * size);
	
	char* word_from_command_line = strtok(new_line, " ");
	
	int counter = 0;
	
	while(word_from_command_line)
	{
	
		command_line_arguments[counter] = (char*) malloc(sizeof(char) * (strlen(word_from_command_line) +1 ));
		strcpy(command_line_arguments[counter], word_from_command_line);
		word_from_command_line = strtok(NULL, " ");
		counter++;
	}
	
	command_line_arguments[counter] = NULL;
	
	free(new_line);
	
	return command_line_arguments;
}


int main()
{
	char word[256];
	
	while(1)
	{
		printf("$ ");
		
		if(fgets(word, 50, stdin) == NULL)
		{
			exit(1);
		}
		
		word[strcspn(word, "\n")] = '\0';
		char** arguments = parse_cmdline(word);
	
		pid_t pid = fork();
		
		if(pid == -1)
		{
			free_pointers(arguments);
			perror("fork");
		}
		
		else if(pid == 0)
		{
			if(execv(arguments[0], arguments))
			{
				perror(arguments[0]);
				free_pointers(arguments);
				return 0;
			}
		}
		
		else
		{
			wait(NULL);
		}
	
		free_pointers(arguments);
	}
	return 0;
}
