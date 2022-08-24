#include<unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include<errno.h>
#include <string.h>
#include "tail_functions.h"

//------------------------------------------------------------------------
// FUNCTION: is_file_directory(име на функцията)
// Връща 1 ако е директория и 0 ако не е.
// PARAMETERS:
// char* file - това е името на файла, с което да проверим дали е директория
//------------------------------------------------------------------------

int is_file_directory(char *file) 
{
   struct stat stats_file;
   stat(file, &stats_file);
   return S_ISDIR(stats_file.st_mode);
}

//------------------------------------------------------------------------
// FUNCTION: open_errors(име на функцията)
// Извежда грешка ако подадения аргумент е директория или ако няма такъв файл
// PARAMETERS:
// char* file - името на файла, от който е станал проблем при отваряне
//------------------------------------------------------------------------
void open_errors(char* file)
{
	 char error[256];
	 int res = is_file_directory(file);
   	 if(!res)
    	 {
		strcpy(error, "tail: cannot open '");
		strcat(error, file);
		strcat(error, "' for reading");
		perror(error); 
    	 }
    	 else
    	 {
    	 	strcpy(error, "tail: error reading '");
		strcat(error, file);
		strcat(error, "'");
		perror(error); 
    	 }
}
//------------------------------------------------------------------------
// FUNCTION: read_error(име на функцията)
// Извежда грешка ако е станал проблем при четене от файл.
// PARAMETERS:
// char* file - името на файл, от който е станал проблем при четене от него
//
//------------------------------------------------------------------------
void read_error(char* file)
{
	char error[256];
	strcpy(error, "tail: error reading '");
	strcat(error, file);
	strcat(error, "'");
	perror(error); 
}
//------------------------------------------------------------------------
// FUNCTION: write_error(име на функцията)
// Извежда грешка, ако е станал проблем при въвеждането от конзолата
// PARAMETERS: Няма
//
//------------------------------------------------------------------------
void write_error()
{
	char error[256];
	strcpy(error, "tail: error writing 'standard output':");
	perror(error); 
}


int successfull_files = 1; //Тази променлива ми е нужна, за да разбера колко файла са принтирани успешно и ако е повече от 1, да сложа нов ред пред всеки следващ.

//------------------------------------------------------------------------
// FUNCTION: print_file_names(име на функцията)
// Принтира името на файла
// PARAMETERS:
// char* file - името на файла, което да се изпринтира
// int success - променлива, която ни казва колко файла успешно са принтирани, за да знаем дали да сложим нов ред преди заглавието или не. 
//------------------------------------------------------------------------

void print_file_names(char* file, int success)
{
	if(success > 1)
	{
		write(STDOUT_FILENO, "\n==> ", strlen("\n==> "));
	}
	else
	{
		write(STDOUT_FILENO, "==> ", strlen("==> "));
	}
        write(STDOUT_FILENO, file, strlen(file));
        write(STDOUT_FILENO," <==\n", strlen(" <==\n"));
}

//------------------------------------------------------------------------
// FUNCTION: read_and_print_last_ten_lines(име на функцията)
// Целта на тази функция е да намери последните десет реда от един файл и да ги изпринтира.
// Ако файла има по-малко от 10 реда, се принтират всички.
// PARAMETERS:
// char* file_for_read - името на файла, от който да изчетем информацията.
// int argc - тази променлива ни трябва за да знаем дали да изпринтираме заглавие или не
//------------------------------------------------------------------------
void read_and_print_last_ten_lines(char* file_for_read, int argc)
{
	int filedescriptor = open(file_for_read, O_RDWR);
	int result_from_system_funcs;

	if(filedescriptor < 0)
	{
		open_errors(file_for_read);
		return;
	}
	
	int all_bytes = lseek(filedescriptor, -1, SEEK_END);
	char letter;
	int new_lines = 0;
	
	
	while(all_bytes >= 0)
	{
		result_from_system_funcs = read(filedescriptor, &letter, 1);
		
		if(result_from_system_funcs < 0)
		{
			read_error(file_for_read);
			return;
		}
		
		if(letter == '\n')
		{
			if(new_lines == 10)
			{
				break;
			}
			new_lines++;
		}
		all_bytes--;
		
		lseek(filedescriptor, all_bytes, SEEK_SET);
	}
	
	if(argc > 2)
	{
		print_file_names(file_for_read, successfull_files);
	}
	
	lseek(filedescriptor, all_bytes+1, SEEK_SET);
	
	while((result_from_system_funcs = read(filedescriptor, &letter, 1)) != 0)
	{
		write(STDOUT_FILENO, &letter, 1);
	}
	
	
	if(close(filedescriptor) < 0)
	{
		read_error(file_for_read);
		return;
	}	
	
	successfull_files++;
}

// FUNCTION:input_from_console (име на функцията)
// Целта на функцията е да прочете от конзолата вход и след това да изпринтира всички редове ако са по-малко от 10 или последните 10 ако са повече.
// 
// PARAMETERS:
// int argc - нужен ми е за да знам дали да принтирам заглавие или не. 
// 
//------------------------------------------------------------------------
void input_from_console(int argc)
{
	int filedescriptor = open("file_for_write.txt", O_RDWR | O_CREAT, S_IRWXU); // Създаваме файла като слагаме и S_IRWXU mode за да можем да го изтрием после.
	
	int result_from_system_funcs;
	char line;

	if(argc > 2)
	{
		print_file_names("standard input", successfull_files);
	}
	
	
	while((line = getchar()) != EOF)
	{
		result_from_system_funcs = write(filedescriptor, &line, 1);
		
		if(result_from_system_funcs < 0)
		{
			write_error();
			return;
		}
	}
	
	close(filedescriptor);
	
	successfull_files++;

	read_and_print_last_ten_lines("file_for_write.txt", 0);

	remove("file_for_write.txt"); // след като се принтират последните 10 реда, изтривам файла.
}

