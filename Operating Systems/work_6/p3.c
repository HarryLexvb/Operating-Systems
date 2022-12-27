#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <sys/msg.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include "semaphoras.h"

#define MAXSIZE     128
#define SHMSZ     27

struct My_msgbuf
{
    long    mtype;
    char    mtext[MAXSIZE];
};

void die(char *s)
{
    perror(s);
    exit(1);
}

int main()
{

    //semaphoras------------------------------------------------
    int id=getSemCreate(3);
    controlSem(id,1);

    // obtener semaphra
    int idP2=getSem(2);
    //while(1){

    //share memory ----------------------------------------------
    int shmid;
    key_t keysh;
    char *shm, *s;
    /*
     * We need to get the segment named
     * "5678", created by the server.
     */
    keysh = 5678;

    /*
     * Locate the segment.
     */
    if ((shmid = shmget(keysh, SHMSZ, 0666)) < 0) {
        perror("shmget");
        exit(1);
    }

    /*
     * Now we attach the segment to our data space.
     */
    if ((shm = shmat(shmid, NULL, 0)) == (char *) -1) {
        perror("shmat");
        exit(1);
    }

    /*
     * Now read what the server put in the memory.
     */
    semP(idP2);
    char temp[MAXSIZE];
    int i=0;
    for (s = shm; *s != NULL; s++,i++){
        //putchar(*s);
        temp[i]=*s;
    }
    //printf("%s\n",temp);
    putchar('\n');

    /*
     * Finally, change the first character of the 
     * segment to '*', indicating we have read 
     * the segment.
     */
    *shm = '*';


    // MQ --------------------------------------------

    int msqid;
    int msgflg = IPC_CREAT | 0666;
    key_t key;
    struct My_msgbuf sbuf;
    size_t buflen;

    key = 1234;

    if ((msqid = msgget(key, msgflg )) < 0)   //Get the message queue ID for the given key
        die("msgget");

    //Message Type
    sbuf.mtype = 1;
    strcpy(sbuf.mtext,temp);
    buflen = strlen(sbuf.mtext) + 1 ;
    //printf("%s\n", sbuf.mtext);

    int  n = msgsnd(msqid, &sbuf, buflen, IPC_NOWAIT);

    if ( n< 0)
    {
        printf ("%d, %ld, %s, %d \n", msqid, sbuf.mtype, sbuf.mtext, (int)buflen);
        die("msgsnd");
    }
    else{
        printf("Message Sent\n");
        semV(id);
    }



    //}
    exit(0);
}