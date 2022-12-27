#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <sys/sem.h>
#include <csignal>
#include <iostream>

#define MAXSIZE 128 // max size of message

void sighandler(int); // signal handler

int  lock = 1;
void die(char *s) { // error handling
    perror(s);
    exit(1);
}

struct MY_msg_buf{ // message buffer
    long m_type; // message type
    char m_text[MAXSIZE];  // message text
};

union sem_union{ // semaphore union
    int val; // value for SETVAL
    struct semid_ds *buf; // buffer for IPC_STAT, IPC_SET
    unsigned short *array; // array for GETALL, SETALL
};


int main(){

    signal(SIGINT, sighandler); // signal handler for SIGINT

    // create message queue and semaphore
    key_t sem_key = 0x1111; // semaphore key
    int id; // semaphore id
    union sem_union arg{}; // semaphore union
    struct sembuf sb = {0, -1, 0}; // semaphore buffer

    /*
    create semaphore with key 0x1111 and 1 semaphore in the set with read/write permissions for all users and create if it doesn't exist already  
    */
    if ((id = semget(sem_key, 1, 0666 | IPC_CREAT)) == -1){
        perror("semget"); // error handling
        exit(1); // exit
    }
    // set value of semaphore to 1 (unlocked) 
    arg.val = 0;
    if (semctl(id, 0, SETVAL, arg) == -1) { // error handling 
        perror("semctl"); // error handling
        exit(1); // exit
    }

    int msqid; // message queue id
    int msgflg = IPC_CREAT | 0666; // message queue flags
    key_t key; // message queue key
    struct MY_msg_buf sbuf{}; // message buffer
    size_t buflen; // buffer length

    key = 1235; // message queue key

    while(lock==1){}; // wait for lock to be unlocked by signal handler for SIGINT 

    if ((msqid = msgget(key, msgflg )) < 0)   //Get the message queue ID for the given key
        die("msgget"); // error handling

    //Message Type
    sbuf.m_type = 1;

    // get message from user and store in message buffer
    std::string message;
    std::cout << "Enter a message to send :D : ";
    std::getline(std::cin, message);
    strcpy(sbuf.m_text, message.c_str());


    buflen = strlen(sbuf.m_text) + 1 ; // get length of message

    // send message to message queue
    int  n = msgsnd(msqid, &sbuf, buflen, IPC_NOWAIT); // send message to message queue

    // v operation on semaphore 
    sb.sem_op = 1; /* free resource */
    if (semop(id, &sb, 1) == -1){ // error handling
        perror("semop"); // error handling
        exit(1); // exit

    }

    if ( n< 0){ // error handling
        //printf ("%d, %ld, %s, %d \n", msqid, sbuf.m_type, sbuf.m_text, (int)buflen); // print error message
        std::cout << msqid << ", " << sbuf.m_type << ", " << sbuf.m_text << ", " << (int)buflen << std::endl;
        die("msgsnd"); // error handling
    }
    else
        std::cout << "Message Sent" << std::endl; // print message sent

    exit(0); // exit
}

void sighandler(int signum) { // signal handler for SIGINT
    lock=0; // unlock lock
}








