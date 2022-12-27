#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <stdio.h>
#include <wayland-client-protocol.h>

int main(void)
{
    int     fd[2], fd2[2], nbytes;
    pid_t   childpid;
    char    string[] = "Hello, world!\n";
    char    readbuffer[80];
    int c;

    pipe(fd);

    if((childpid = fork()) == -1) {
        perror("fork");
        exit(1);
    }

    if(childpid == 0) {
        pid_t cchildpid;
        pipe(fd2);
        if ((cchildpid = fork()) == -1) {
            perror("fork");
            exit(1);
        }

        if (cchildpid == 0) {
            //printf("Grandchild program.\nWrite a message for the parent: ");
            printf("Grandchild program.\n");
            char *afifo = "afifo";
            mkfifo(afifo, 0666);
            int fifofd = open(afifo, O_RDONLY);
            read(fifofd, readbuffer, sizeof(readbuffer));
            close(fifofd);

            //scanf("%s", readbuffer);
            close(fd2[0]);
            write(fd2[1], readbuffer, strlen(readbuffer));
            exit(0);
        } else {
            printf("Child program.\n"); 
            nbytes = read(fd2[0], readbuffer, sizeof(readbuffer));
            write(fd[1], readbuffer, strlen(readbuffer));
            exit(0);
        }
    } else {
        printf("Parent program.\n"); 
        close(fd[1]);
        nbytes = read(fd[0], readbuffer, sizeof(readbuffer));
        printf("Parent has received string: %s\n", readbuffer);
    }

    return(0);
}
