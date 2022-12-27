#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/shm.h>

#include "semaphoras.h"
#define MAXSIZE     128
#define SHMSZ     27




void die(char *s)
{
    perror(s);
    exit(1);
}

struct My_msgbuf
{
    long    mtype;
    char    mtext[MAXSIZE];
};


int main(int argc, char **argv)
{

    //semaphoras---------------------------------
    int id=getSemCreate(2);
    controlSem(id,1);
    // obtener semaphra
    int idP1=getSem(1);

    //while(1){
    int msqid;
    key_t key;
    struct My_msgbuf rcvbuffer;

    key = 1235;

    if ((msqid = msgget(key, 0666)) < 0)
        die("msgget()");
    int n;
    do{
        //Receive an answer of message type 1.
        semP(idP1);
        n = msgrcv(msqid, &rcvbuffer, MAXSIZE, 1, IPC_NOWAIT);
        //if ( n < 0)
        //  die("msgrcv");
    }while(n<1);
    //printf("%s\n", rcvbuffer.mtext);

    //-------------------------------------------------------------
    //  ShareMem 


    char c;
    int shmid;
    key_t keysh;
    char *shm, *s;
    int buflen;

    keysh = 5678;
    if ((shmid = shmget(keysh, SHMSZ, IPC_CREAT | 0666)) < 0) {
        perror("shmget");
        exit(1);
    }

    if ((shm = shmat(shmid, NULL, 0)) == (char *) -1) {
        perror("shmat");
        exit(1);
    }


    char temp[MAXSIZE];
    strcpy(temp,rcvbuffer.mtext);
    buflen = strlen(rcvbuffer.mtext)+1;
    s = shm;
    for (int i=0; i <= buflen; i++){
        //c=temp[i];
        *s++ = temp[i];
    }
    *s = '\0';

    while (*shm != '*')
        sleep(1);

    semV(id);
    //}
    exit(0);
}
























