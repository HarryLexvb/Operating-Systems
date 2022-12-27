#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>

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

main(int argc, char **argv)
{

    signal(SIGINT, sighandler);
    
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

    
    strcpy(sbuf.mtext,"pepe");


    buflen = strlen(sbuf.mtext) + 1 ;

    int  n = msgsnd(msqid, &sbuf, buflen, IPC_NOWAIT);
   
    if ( n< 0)
    {
        printf ("%d, %ld, %s, %d \n", msqid, sbuf.mtype, sbuf.mtext, (int)buflen);
        die("msgsnd");
    }
    else
        printf("Message Sent\n");

    exit(0);
}



void sighandler(int signum) {
   lock=0;
}








