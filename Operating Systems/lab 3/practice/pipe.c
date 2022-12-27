#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>


int c;


int main(void)
{
        int     myPipe[2], nbytes; // fd[2]
        pid_t   childpid;
        char    string[] = "Hello, dad!\n";
        char    readbuffer[80];
        int c;

        pipe(myPipe); /// pipe(fd)

        if((childpid = fork()) == -1)
        {
                perror("fork");
                exit(1);
        }

        if(childpid == 0)
        {
                int c;
                printf("Child program:"); 
                c=getchar();
                printf("C %c\n",c);
                /* Child process closes up input side of pipe */
                close(myPipe[0]);

                /* Send "string" through the output side of pipe */
                write(myPipe[1], string, (strlen(string)+1));
                exit(0);
        }
        else // papa 
        {
                int c;
                printf("Parent program:"); 
                c=getchar();
                printf("P %c\n",c);
                /* Parent process closes up output side of pipe */
                close(myPipe[1]);

                /* Read in a string from the pipe */
                nbytes = read(myPipe[0], readbuffer, sizeof(readbuffer));
                printf("Received from my son: %s", readbuffer);
        }

        return(0);
}
