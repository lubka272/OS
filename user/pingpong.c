#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
int main(int argc,char *argv[]){

int to_child[2];
int to_parent[2];

pipe(to_child);
pipe(to_parent);

int ret=fork();

if (ret==0) {
//child
	char received;
	int n=read(to_child[0],&received,1);
	if (n==1){
	printf("%d: received ping\n", getpid());
	}
	write(to_parent[1],"y",1);
}
else if (ret>0){
//parent
	write(to_child[1],"x",1);
	
	char received;
	int n=read(to_parent[0],&received,1);
	if (n==1){
	printf("%d: received pong\n", getpid());
	}
}
else{
fprintf(2,"fork() error\n");
exit(1);
}

exit(0);
}

