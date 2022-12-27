#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include "OStools.h"

using namespace std; 

void OStools::MyKill(int P, int S){
	kill(P,S); 
}
