#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdio.h>
#include <stdlib.h>
//#include "semaphoras.h"

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/sem.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
//#include "semaphoras.h"
#include <stdio.h> 
#include <errno.h> 
#include <sys/types.h> 
#include <sys/ipc.h> 
#define MAXSIZE     128

void die(char *s)
{
  perror(s);
  exit(1);
}

struct msgbuf
{
    long    mtype;
    char    mtext[MAXSIZE];
};


union semun
{
    int val;
    struct semid_ds *buf;
    unsigned short *array;
};
void main()
{   

   //getsemaphoras------------------------------------------------
    //int idP3=getSem(3);


    key_t semakeyp2=0x3333;
    int semid;
    struct sembuf sb = {0, -1, 0}; /* set to allocate resource */
    if ((semid = semget(semakeyp2, 1, 0)) == -1) 
    {
        perror("semget");
        exit(1); 
    }

    //controlSem(1,0);
    //controlSem(2,0);
    

    
//P

  if (semop(semid, &sb, 1) == -1) // wait for a free resource
  { 
      perror("semop");
      exit(1); 
  }
   // while(1) {
    int msqid;
    key_t key;
    struct msgbuf rcvbuffer;

    key = 1234;
    


    if ((msqid = msgget(key, 0666)) < 0)
      die("msgget()");
    int n;
    do{
     	//Receive an answer of message type 1.
      
     	n = msgrcv(msqid, &rcvbuffer, MAXSIZE, 1, IPC_NOWAIT);
     	//if ( n < 0)
        //	die("msgrcv");
    }while(n<1);
    printf("%s\n", rcvbuffer.mtext);
    //  ShareMem 
    //}   
    exit(0);
}