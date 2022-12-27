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

void sighandler(int);

int  lock = 1; 
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


void main(int argc, char **argv)
{
    //while(1){

   
    
    signal(SIGINT, sighandler);
    
    //semaphoras---------------------------------
    //int id=getSemCreate(1);
    //controlSem(id,1);  

    //crear semaphora
    key_t semakey = 0x1111;
    int id;
    union semun arg;
    struct sembuf sb = {0, -1, 0};
    if ((id = semget(semakey, 1, 0666 | IPC_CREAT)) == -1) 
    {
        perror("semget");
        exit(1); 
    }
    //control
    arg.val = 0;
    if (semctl(id, 0, SETVAL, arg) == -1) 
    {
        perror("semctl");
        exit(1); 
    }

    //MQ---------------------------------

    int msqid;
    int msgflg = IPC_CREAT | 0666;
    key_t key;
    struct msgbuf sbuf;
    size_t buflen;

    key = 1235;

    while(lock==1){};

    if ((msqid = msgget(key, msgflg )) < 0)   //Get the message queue ID for the given key
      die("msgget");

    //Message Type
    sbuf.mtype = 1;

    //mensaje
    strcpy(sbuf.mtext,"pepe");


    buflen = strlen(sbuf.mtext) + 1 ;

//envio mensaje
    int  n = msgsnd(msqid, &sbuf, buflen, IPC_NOWAIT);

    // v
    sb.sem_op = 1; /* free resource */ 
    if (semop(id, &sb, 1) == -1) 
    {
        perror("semop");
        exit(1); 
        
    }

    if ( n< 0)
    {
        printf ("%d, %ld, %s, %d \n", msqid, sbuf.mtype, sbuf.mtext, (int)buflen);
        die("msgsnd");
    }
    else{
        printf("Message Sent\n");
        //semV(id);
    }
      



    exit(0);
    //}
}



void sighandler(int signum) {
   lock=0;
}








