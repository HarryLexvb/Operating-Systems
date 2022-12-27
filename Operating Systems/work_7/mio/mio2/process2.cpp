#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <sys/shm.h>
#include <sys/sem.h>
#include <unistd.h>

#define MAXSIZE 128
#define SHMSZ 27


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

struct MY_msg_buf
{
    long m_type;
    char m_text[MAXSIZE];
};

int main(){
    
    key_t sem_key_process1=0x1111; //key for semaphore process1 
    int sem_id; //semaphore id for process1 
    struct sembuf sb = {0, -1, 0}; //semaphore buffer for process1 
    if ((sem_id = semget(sem_key_process1, 1, 0)) == -1) { //get semaphore id for process1
        perror("semget"); //error handling for semaphore id
        exit(1); //exit if error
    }

    key_t sem_key = 0x2222; //key for semaphore process2
    int id; //semaphore id for process2
    union semun arg{}; //semaphore union for process2
    //struct sembuf sb = {0, -1, 0};
    if ((id = semget(sem_key, 1, 0666 | IPC_CREAT)) == -1){ //get semaphore id for process2
        perror("semget"); // error handling for semaphore id    
        exit(1); //exit if error
    }

    arg.val = 0; //set semaphore value to 0
    if (semctl(id, 0, SETVAL, arg) == -1){ //set semaphore value to 0
        perror("semctl"); //error handling for semaphore value
        exit(1); //exit if error
    }

    //P operation for process1 
    if (semop(sem_id, &sb, 1) == -1) // wait for a free resource
    {
        perror("semop"); //error handling for semaphore operation
        exit(1); //exit if error
    }
    
    int msqid; //message queue id
    key_t key; //message queue key
    struct MY_msg_buf rcvbuffer{}; //message queue buffer

    key = 1235; //set message queue key

    if ((msqid = msgget(key, 0666)) < 0) //get message queue id
        die("msgget()"); //error handling for message queue id
    int n; //number of bytes read
    do{ 
        //Receive an answer of message type 1.
        n = msgrcv(msqid, &rcvbuffer, MAXSIZE, 1, IPC_NOWAIT); //receive message from message queue
    }while(n<1);
    
    char c; // character to be read
    int shmid; //shared memory id
    key_t keysh; //shared memory key
    char *shm, *s; //shared memory buffer
    int buflen; //buffer length

    keysh = 5678; //set shared memory key
    if ((shmid = shmget(keysh, SHMSZ, IPC_CREAT | 0666)) < 0) { //get shared memory id
        perror("shmget"); //error handling for shared memory id 
        exit(1); //exit if error
    }
    //    if ((shm = shmat(shmid, NULL, 0)) == (char *) -1) {
    if ((shm = static_cast<char *>(shmat(shmid, NULL, 0))) == (char *) -1) { //attach shared memory to process
        perror("shmat"); //error handling for shared memory attachment
        exit(1); //exit if error
    }
    
    char temp[MAXSIZE];  //temporary buffer
    strcpy(temp,rcvbuffer.m_text); //copy message queue buffer to temporary buffer
    buflen = strlen(rcvbuffer.m_text) + 1; //set buffer length to message queue buffer length 
    s = shm; //set shared memory buffer to s 

    for (int i=0; i <= buflen; i++){ //loop through message queue buffer
        *s++ = temp[i]; //copy message queue buffer to shared memory buffer
    }
    *s = '\0'; //set shared memory buffer to null

    // v operation for process2 
    sb.sem_op = 1; /* free resource */
    if (semop(id, &sb, 1) == -1) { //signal that a resource is free
        perror("semop"); //error handling for semaphore operation
        exit(1); //exit if error
    }
    
    while (*shm != '*') //loop until shared memory buffer is not equal to *
        sleep(1); //sleep for 1 second

    exit(0);
}
