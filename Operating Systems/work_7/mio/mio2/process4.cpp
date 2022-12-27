#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <cstdio>
#include <cstdlib>
#include <sys/sem.h>
#include <iostream>

#define MAXSIZE 128 /* maximum size of message */

void die(char *s){ // error handling
    perror(s); // print error message
    exit(1); // exit with error
}

struct MY_msg_buf{ // message buffer
    long    m_type; // message type
    char    m_text[MAXSIZE]; // message text
};

union sem_union{ // semaphore union
    int val; // value for SETVAL
    struct semid_ds *buf; // buffer for IPC_STAT, IPC_SET
    unsigned short *array; // array for GETALL, SETALL
};

int main(){

    key_t sem_key_process4 = 0x3333; // semaphore key for process 4
    int sem_id; // semaphore id
    struct sembuf sb = {0, -1, 0}; /* set to allocate resource */
    if ((sem_id = semget(sem_key_process4, 1, 0)) == -1) { // get semaphore id
        perror("semget"); // print error message
        exit(1); // exit with error
    }
    //P operation for semaphore 0

    if (semop(sem_id, &sb, 1) == -1) // wait for a free resource
    {
        perror("semop"); // print error message
        exit(1); // exit with error
    }
    
    int msq_id; // message queue id
    key_t key; // key for message queue
    struct MY_msg_buf rcv_buffer{}; // receive buffer

    key = 1234; // key for message queue
    
    if ((msq_id = msgget(key, 0666)) < 0) // get message queue id
        die("msgget()"); // error handling
    int n; // number of bytes read
    
    do{
        //Receive an answer of message type 1.
        n = msgrcv(msq_id, &rcv_buffer, MAXSIZE, 1, IPC_NOWAIT); // receive message
     
    }while(n<1); // if no message received, try again

    std::cout << rcv_buffer.m_text << std::endl; // print message

    exit(0);
}
