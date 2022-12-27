#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <cstdio>
#include <sys/msg.h>
#include <cstring>
#include <cstdlib>
#include <sys/sem.h>
#include <iostream>

#define MAXSIZE 128 // max size of shared memory segment
#define SHMSZ 27 // shared memory size

struct MY_msg_buf{ // message buffer
    long    m_type; // message type
    char    m_text[MAXSIZE]; // message text
};

void die(char *s){ // error handling function
    perror(s); // print error message
    exit(1); // exit with error
}

union semun{ // semaphore union
    int val; // semaphore value
    struct semid_ds *buf; // buffer for IPC_STAT, IPC_SET
    unsigned short *array; // array for GETALL, SETALL
};

int main(){
    
    key_t sem_key_process2=0x2222;
    int sem_id;
    struct sembuf sb = {0, -1, 0}; /* set to allocate resource */
    if ((sem_id = semget(sem_key_process2, 1, 0)) == -1){ // get semaphore
        perror("semget"); // print error message
        exit(1); // exit with error
    }

    key_t sem_key = 0x3333; // semaphore key
    int id; // semaphore id
    union semun arg{}; // semaphore union
    
    if ((id = semget(sem_key, 1, 0666 | IPC_CREAT)) == -1){ // get semaphore
        perror("semget"); // print error message
        exit(1); // exit with error
    }

    arg.val = 0; // set semaphore value
    if (semctl(id, 0, SETVAL, arg) == -1){ // set semaphore value
        perror("semctl"); // print error message
        exit(1); // exit with error
    }

    // P operation on semaphore id with 0 index and 0 flag (wait) 
    if (semop(sem_id, &sb, 1) == -1) // wait for a free resource
    {
        perror("semop"); // print error message
        exit(1); // exit with error
    }

    // shared memory key and id variables 
    int shm_id; // shared memory id
    key_t key_sh; // shared memory key
    char *shm, *s; // shared memory pointer
    /*
     * We need to get the segment named
     * "5678", created by the server.
     */
    key_sh = 5678; // shared memory key

    /*
     * Locate the segment.
     */
    if ((shm_id = shmget(key_sh, SHMSZ, 0666)) < 0) { // get shared memory 
        perror("shmget"); // print error message
        exit(1); // exit with error
    }

    /*
     * Now we attach the segment to our data space.
     */
    if ((shm = static_cast<char *>(shmat(shm_id, NULL, 0))) == (char *) -1) { // attach shared memory
        perror("shmat"); // print error message
        exit(1); // exit with error
    }

    /*
     * Now read what the server put in the memory.
     */
    char temp[MAXSIZE]; // temporary string
    int i=0; // counter
    for (s = shm; *s != NULL; s++,i++){ // copy shared memory to temp
        temp[i]=*s; // copy shared memory to temp
    }
    
    putchar('\n');  

    /*
     * Finally, change the first character of the 
     * segment to '*', indicating we have read 
     * the segment.
     */
    *shm = '*'; // change first character of shared memory to '*'


    // MQ  key and id variables 

    int msq_id; // message queue id
    int msg_flg = IPC_CREAT | 0666; // message queue flag
    key_t key; // message queue key
    struct MY_msg_buf sbuf{}; // message buffer
    size_t buf_len; // message buffer length

    key = 1234; // message queue key

    if ((msq_id = msgget(key, msg_flg )) < 0)   //Get the message queue ID for the given key
        die("msgget"); // error handling

    //Message Type
    sbuf.m_type = 1; // message type
    strcpy(sbuf.m_text, temp); // copy temp to message buffer
    buf_len = strlen(sbuf.m_text) + 1 ; // message buffer length

    // Send a message to process 4
    int  n = msgsnd(msq_id, &sbuf, buf_len, IPC_NOWAIT); 
    // V operation on semaphore id with 0 index and 0 flag (signal)
    sb.sem_op = 1; /* free resource */
    if (semop(id, &sb, 1) == -1) { // signal that a resource is free 
        perror("semop"); // print error message
        exit(1); // exit with error

    }

    if ( n< 0){ // error handling 
        //printf ("%d, %ld, %s, %d \n", msq_id, sbuf.m_type, sbuf.m_text, (int)buf_len);
        std::cout << msq_id << " " << sbuf.m_type << " " << sbuf.m_text << " " << buf_len << std::endl; // print error message
        die("msgsnd"); // error handling
    }
    else
        std::cout << "Message Sent" << std::endl; // print message
    exit(0);
}
