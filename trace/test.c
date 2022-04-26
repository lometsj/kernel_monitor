#include <stdio.h>

struct node{
	const char *ac;
};

int main(){
	char *argc[]={"ls", "-al", NULL};
	char *envp[]={0, NULL};
	execve("/bin/ls", argc, envp );
	return 0;
}
