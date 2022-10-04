#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <iostream>

using namespace std; 

void sighandler(int signum){
	//printf("Caught signal $d, coming out...\n", signum); 
	cout << "Caught signal " << signum << ", coming out...\n"; 
}


int main(){

	signal(SIGINT, sighandler);

	while(1){
		//ntf("going to sleep for a second...\n");
		cout << "going to sleep for a second...\n";
		sleep(1); 
	}
	return(0);
}
