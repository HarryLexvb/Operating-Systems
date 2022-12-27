#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include "OStools.h"

using namespace std; 

int main(int argc, char **argv){

	OStools k; 

	k.MyKill(atoi(argv[1]), atoi(argv[2]));
}


