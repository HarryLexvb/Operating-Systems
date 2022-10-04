#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>


int c;


int main(void)
{
        int     fd[2], nbytes;
        pid_t   childpid;
        char string[] = "hola juan"; 
        char    readbuffer[80];
        int c;

        pipe(fd);

        if((childpid = fork()) == -1)
        {
                perror("fork");
                exit(1);
        }
 

        c=getchar();


        if(childpid == 0)
        {
                int c;
                char cad[10];

                c=getchar();

                /* Child process closes up input side of pipe */
                close(fd[1]);

                /* Read in a string from the pipe */
                nbytes = read(fd[0], readbuffer, sizeof(readbuffer));
                sprintf(cad, "%d", atoi(readbuffer)+7);
                printf("Valor recibido por HIJO: %s\n", cad);  

                //NIETOOOOOOOOOOOOOOOOOOOO

                int     fdnieto[2], nbytes2;
                pid_t   nietopid;
                char    readbuffernieto[80];

                pipe(fdnieto);

                if((nietopid = fork()) == -1)
                {
                        perror("fork");
                        exit(1);
                }
        

                c=getchar();


                if(nietopid == 0)
                {
                        int c;
                        char cad[10];

                        c=getchar();

                        /* Child process closes up input side of pipe */
                        close(fdnieto[1]);

                        /* Read in a string from the pipe */
                        nbytes = read(fdnieto[0], readbuffernieto, sizeof(readbuffernieto));
                        sprintf(cad, "%d", atoi(readbuffernieto)/3);
                        printf("Valor recibido por NIETO: %s\n", cad);  

                        //BISNIETOOOOOOOOOOOOOOOOOO

                        int     fdbisnieto[2], nbytes3;
                        pid_t   bisnietopid;
                        char    readbufferbisnieto[80];

                        pipe(fdbisnieto);

                        if((bisnietopid = fork()) == -1)
                        {
                                perror("fork");
                                exit(1);
                        }
                

                        c=getchar();


                        if(bisnietopid == 0)
                        {
                                int c;
                                char cad[10];

                                c=getchar();

                                /* Child process closes up input side of pipe */
                                close(fdbisnieto[1]);

                                /* Read in a string from the pipe */
                                nbytes = read(fdbisnieto[0], readbufferbisnieto, sizeof(readbufferbisnieto));
                                sprintf(cad, "%d", atoi(readbufferbisnieto)*atoi(readbufferbisnieto));
                                printf("Valor recibido por BISNIETO: %s\n", cad);  

                                /* Send "string" through the output side of pipe */
                                printf("Valor enviado por BISNIETO: %s\n", cad); 
                                write(fd[1], cad, (strlen(cad)+1));
                                
                                

                                exit(0);
                                
                        }
                        else
                        {
                                int c;
                                char string[10];
                                sprintf(string, "%d", atoi(cad));

                                c=getchar();

                                /* Parent process closes up output side of pipe */
                                close(fdbisnieto[0]);

                                
                                /* Send "string" through the output side of pipe */
                                printf("Valor enviado por NIETO: %s\n", string); 
                                write(fdbisnieto[1], string, (strlen(string)+1));
                                exit(0);
                        }

                        //FIN BISNIETOOOOOOOOOOOOOOO

                        exit(0);
                        
                }
                else
                {
                        int c;
                        char string[10];
                        sprintf(string, "%d", atoi(cad));

                        c=getchar();

                        /* Parent process closes up output side of pipe */
                        close(fdnieto[0]);

                        
                        /* Send "string" through the output side of pipe */
                        printf("Valor enviado por HIJO: %s\n", string); 
                        write(fdnieto[1], string, (strlen(string)+1));
                        exit(0);
                }


                //FIN NIETOOOOO

                exit(0);
                
        }
        else
        {
                int c;
                char string[] = "5";

                c=getchar();

                /* Parent process closes up output side of pipe */
                close(fd[0]);

                
                /* Send "string" through the output side of pipe */
                printf("Valor enviado por PADRE: %s\n", string); 
                write(fd[1], string, (strlen(string)+1));

                close(fd[1]);
                nbytes = read(fd[0], readbuffer, sizeof(readbuffer));
                printf("Valor recibido por PADRE: %s\n", readbuffer);

                exit(0);

                
        }        


        return(0);
}
