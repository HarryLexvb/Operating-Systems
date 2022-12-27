#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#define KEY 0x1111

union semun
{
    int val;
    struct semid_ds *buf;
    unsigned short *array;
};

union semun u;

struct sembuf p = {0, -1, SEM_UNDO};
struct sembuf v = {0, +1, SEM_UNDO};

void semP(int id)
{
    if (semop(id, &p, 1) < 0)
    {
        perror("semop p");
        exit(13);
    }
}

void semV(int id)
{
    if (semop(id, &v, 1) < 0)
    {
        perror("semop p");
        exit(14);
    }
}

int getSem(int k)
{
    int id = semget(k, 1, 0666);
    if (id < 0)
    {
        perror("semget");
        exit(11);
    }
    return id;
}

int getSemCreate(int k)
{
    int id = semget(k, 1, 0666 | IPC_CREAT);
    if (id < 0)
    {
        perror("semget");
        exit(11);
    }
    return id;
}

void controlSem(int id, int numRecursos)
{
    //union semun u;
    u.val = numRecursos;
    if (semctl(id, 0, SETVAL, u) < 0)
    {
        perror("semctl");
        exit(12);
    }
}