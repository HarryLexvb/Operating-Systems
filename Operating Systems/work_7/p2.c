#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/shm.h>

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

//#include "semaphoras.h"
#define MAXSIZE     128
#define SHMSZ     27


union semun
{
    int val;
    struct semid_ds *buf;
    unsigned short *array;
};

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
    
    
    // obtener semaphra
    //int idP1=getSem(1);
    //semP(idP1);

    key_t semakeyp1=0x1111;
    int semid;
    struct sembuf sb = {0, -1, 0}; 
    if ((semid = semget(semakeyp1, 1, 0)) == -1) 
    {
        perror("semget");
        exit(1); 
    }

    //semaphoras---------------------------------
    //int id=getSemCreate(2);
    //controlSem(id,1);

    key_t semakey = 0x2222;
    int id;
    union semun arg;
    //struct sembuf sb = {0, -1, 0};
    if ((id = semget(semakey, 1, 0666 | IPC_CREAT)) == -1) 
    {
        perror("semget");
        exit(1); 
    }

    arg.val = 0;
    if (semctl(id, 0, SETVAL, arg) == -1) 
    {
        perror("semctl");
        exit(1); 
    }

     //P

    if (semop(semid, &sb, 1) == -1) // wait for a free resource
    { 
        perror("semop");
        exit(1); 
    }


    //while(1){
    int msqid;
    key_t key;
    struct msgbuf rcvbuffer;

    key = 1235;
   
    if ((msqid = msgget(key, 0666)) < 0)
      die("msgget()");
    int n;
    do{
     	//Receive an answer of message type 1.
       
     	n = msgrcv(msqid, &rcvbuffer, MAXSIZE, 1, IPC_NOWAIT);
     	//if ( n < 0)
        //	die("msgrcv");
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
    *s = NULL;

    // v
    sb.sem_op = 1; /* free resource */ 
    if (semop(id, &sb, 1) == -1) 
    {
        perror("semop");
        exit(1); 
        
    }

    while (*shm != '*')
        sleep(1);

    //semV(id);
    //}
    exit(0);
}

























