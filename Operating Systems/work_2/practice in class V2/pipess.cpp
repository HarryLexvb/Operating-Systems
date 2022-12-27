/*****************************************************************************
 Excerpt from "Linux Programmer's Guide - Chapter 6"
 (C)opyright 1994-1995, Scott Burkett
 ***************************************************************************** 
 MODULE: pipe.c
 *****************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>


int c;


int main(void)
{
        int     fd[2], fd2[2],nbytes,nbytes2;
        pid_t   childpid;
        char    string[] = "Hola Juan!\n";
        char    readbuffer[80];
        char    readbuffer2[80];
        int c;

        pipe(fd);

        if((childpid = fork()) == -1)
        {
                perror("fork");
                exit(1);
        }
 
        printf("Main program:"); 
        //c=getchar();
        //printf("M %c\n",c);

        if(childpid == 0)
        {
                pipe(fd2);
                if((childpid = fork()) == -1)
                {
                        perror("fork");
                        exit(1);
                }
                if(childpid == 0)
                {
                        int c;
                        printf("Grandchild program:"); 
                        //c=getchar();
                        //printf("G %c\n",c);
                        /* Child process closes up input side of pipe */
                        close(fd2[0]);

                        /* Send "string" through the output side of pipe */
                        write(fd2[1], string, (strlen(string)+1));
                        exit(0);    
                }
                else
                {
                        int c;
                        printf("Child program:"); 
                        //c=getchar();
                        //printf("C %c\n",c);
                        /* Child process closes up input side of pipe */
                        close(fd2[1]);
                        nbytes = read(fd2[0], readbuffer, sizeof(readbuffer));
                        close(fd[0]);
                        /* Send "string" through the output side of pipe */
                        write(fd[1], readbuffer, (strlen(readbuffer)+1));
                        exit(0);    
                }
        }
        else
        {
                int c;
                printf("Parent program:"); 
                //c=getchar();
                //printf("P %c\n",c);
                /* Parent process closes up output side of pipe */
                close(fd[1]);

                /* Read in a string from the pipe */
                nbytes2 = read(fd[0], readbuffer2, sizeof(readbuffer2));
                printf("Received string: %s", readbuffer2);
        }

        return(0);
}
