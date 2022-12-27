#include<stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>

int main() {

   int pid;
   pid = fork();

   // Child process
   if (pid == 0) {
      system("echo 'hijo'");
      //system("echo $$");
      int pid = getpid();
      int ppid = getpid(); 
      printf("HIJO:PID:%d,PPID.%d\n", pid, ppid);
      sleep(100);  
   } else {
      system("echo 'padre'");
      //system("echo $$");
      int pid = getpid();
      int ppid = getpid(); 
      printf("padre:PID:%d,PPID.%d\n", pid, ppid);
      sleep(100);
   }
   return 0;
}