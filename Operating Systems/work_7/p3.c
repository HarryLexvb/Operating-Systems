#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <sys/msg.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
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
#define SHMSZ     27

struct msgbuf
{
    long    mtype;
    char    mtext[MAXSIZE];
};

void die(char *s)
{
  perror(s);
  exit(1);
}
union semun
{
    int val;
    struct semid_ds *buf;
    unsigned short *array;
};



void main()
{   
    // obtener semaphra
    //int idP2=getSem(2);
    //semP(idP2);

    key_t semakeyp2=0x2222;
    int semid;
    struct sembuf sb = {0, -1, 0}; /* set to allocate resource */
    if ((semid = semget(semakeyp2, 1, 0)) == -1) 
    {
        perror("semget");
        exit(1); 
    }

    //semaphoras------------------------------------------------
    //int id=getSemCreate(3);
    //controlSem(id,1);

    key_t semakey = 0x3333;
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


    
    //while(1){

    //share memory ----------------------------------------------
    // P
       if (semop(semid, &sb, 1) == -1) // wait for a free resource
    { 
        perror("semop");
        exit(1); 
    }

    //sharememory
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
    struct msgbuf sbuf;
    size_t buflen;

    key = 1234;

    if ((msqid = msgget(key, msgflg )) < 0)   //Get the message queue ID for the given key
      die("msgget");

    //Message Type
    sbuf.mtype = 1;
    strcpy(sbuf.mtext,temp);
    buflen = strlen(sbuf.mtext) + 1 ;
    //printf("%s\n", sbuf.mtext);

    //enviar mensaje a p4
    int  n = msgsnd(msqid, &sbuf, buflen, IPC_NOWAIT);
    // V
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
        
    }
    //semV(id);


    //}
    exit(0);
}