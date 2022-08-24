#include <stdio.h>

#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main(int argc, char *argv[]){
    
    	while(1)
    	{
    		int pid  = fork();
    		if(pid == 0)
    		{
    			if(execvp(argv[1], argv + 1) != 0)
    			{
    				perror(argv[1]);
    				return -1;
    			}
    			
    		}
    		
    		else if(pid < 0)
    		{
    			perror("fork");
    		}
    		else
    		{
    			wait(NULL);
    		}
  
    		sleep(2);
    	}
    	
    return 0;
}
