#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#define MAXSIZE 34

void sending_primes(int left_pipe[]){
	//close(left_pipe[1]);
    	int right_pipe[2];
    	int actualNumber;
    	int number;
	int status=0;
	int pom;
	int wpid;
    	
    	int pid= fork();
    	while(1)
{
	if (pid==0){ //child cita
	
		pipe(right_pipe);
		
		
		 pom=read(left_pipe[0],&actualNumber,sizeof(actualNumber)); //prve cislo v pipe je prvocislo
			
		 printf("prime %d\n",actualNumber);
		
		if(pom<0)
		break;
		while (read(left_pipe[0],&number,sizeof(number) > 0)){ //pozeram vsetky dalsie cisla
			if(number%actualNumber!=0){
				write(right_pipe[1],&number,sizeof(number));
			}
		}
		
				
	}

	else if (pid>0){ //parent caka

	 	while((wpid=wait(&status))>0);	
	 pom=read(right_pipe[0],&actualNumber,sizeof(actualNumber));//prve cislo v pipe je prvocislo
	  printf("prime %d\n",actualNumber);
		
		if(pom<0)
		break;
		while (read(right_pipe[0],&number,sizeof(number) )){ //pozeram vsetky dalsie cisla
			if(number%actualNumber!=0){
				write(left_pipe[1],&number,sizeof(number));
			}
		}
	      
	}
	else {
		printf("Fork error");
	}
    	
	}
}

int main(int argc,char *argv[]){

	
	int fd[2];    
    	pipe(fd);	

    	for(int number=2; number<= 35;number++){
        
        	write(fd[1],&number,sizeof(number));       
    	}
    	sending_primes(fd);
    	exit(0);
		
}
