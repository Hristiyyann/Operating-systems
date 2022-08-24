//------------------------------------------------------------------------
// NAME: Hristiyan Radev
// CLASS: XIb
// NUMBER: 25
// PROBLEM: #1
// FILE NAME: tail.c (unix file name)
// FILE PURPOSE:
// Да се имплементира функцията tail
//------------------------------------------------------------------------

#include<stdio.h>
#include "tail_functions.h"

int main(int argc, char* argv[])
{
	if(argc == 1)
    	{
    		input_from_console(argc);
    	}
    	else
    	{
		for(int i = 1; i < argc; i++)
		{
			if(argv[i][0] == '-')
			{
			     input_from_console(argc);
			}
			else
			{
			    read_and_print_last_ten_lines(argv[i],argc);
			}
			
	    	}
	 }
	return 0;
}
