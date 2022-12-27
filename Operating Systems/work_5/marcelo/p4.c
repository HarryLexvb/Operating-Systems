#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdio.h>
#include <stdlib.h>
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


void main()
{
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
    
    exit(0);
}