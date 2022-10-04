
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>      

int fd[4][2]; 
float num;
//char string[] = "hola juan"

int main()
{
    
    for(int j = 0; j < 3 ; j++) //se crean los pipes
        pipe(fd[j]);

    for(float i=1; i<=3; i++)
    {
        if(fork()==0) 
        {
            if(fork()==0)
            {
                if(fork()==0) // Bisnieto
                {
                    close(fd[2][1]); //w
                    close(fd[3][0]); //r

                    read(fd[2][0],&num, sizeof(num));
                    printf("Bisnieto recibe: %f y ",num);
                    num = 777; // num*num
                    printf("envia: %f\n",num);
                    write(fd[3][1],&num, sizeof(num));

                    close(fd[2][0]); 
                    close(fd[3][1]);
                    return 0;
                }
                else //nieto
                {
                    close(fd[1][1]); //w
                    close(fd[2][0]); //r

                    read(fd[1][0],&num, sizeof(num));
                    printf("Nieto recibe: %f y ",num);
                    num = num/3;
                    printf("envia: %f\n",num);
                    write(fd[2][1],&num, sizeof(num));
                   
                    close(fd[1][0]);
                    close(fd[2][1]);
                    return 0;
                }
            }
            else // hijo
            {
                close(fd[0][1]); //w
                close(fd[1][0]); //r

                //printf("Hijo recibe: %f y ",num);
                read(fd[0][0],&num, sizeof(num));
                printf("Hijo recibe: %f y ",num);
                num+=7;
                write(fd[1][1],&num, sizeof(num));
                printf("envia: %f\n",num);
                
                close(fd[0][0]);
                close(fd[1][1]);
                return 0;
            }
        }
        else //Padre
        {   
            num=i;
            close(fd[0][0]); //cierra input
            close(fd[3][1]);

            write(fd[0][1],&num,sizeof(num)); //escribe en output
            printf("El número es :%f \n",num);

            sleep(1);
            
            read(fd[3][0],&num, sizeof(num));
            printf("Padre Recibe(777): %f \n",num);
            close(fd[0][1]);
            close(fd[3][0]);
        }
    }
    return 0;
}
 ////   [4][2]  [0][0] input [0][1] output
 /////////////  [1][0]
 /////////////  [2][0]