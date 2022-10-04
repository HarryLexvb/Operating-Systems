#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>

int main(int argc, char **argv){

	int s = atoi(argv[1]); 
	int p = atoi(argv[2]); 

	kill(p,s);

	return 0; 
}
