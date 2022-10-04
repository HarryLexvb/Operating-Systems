#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <wayland-client-protocol.h>

int main(void) 
{
    char *afifo = "afifo";
    char *readbuffer;
    mkfifo(afifo, 0666);
    int fifofd = open(afifo, O_WRONLY);
    printf("Send a message: ");
    scanf("%s", readbuffer);
    write(fifofd, readbuffer, strlen(readbuffer) + 1);
    close(fifofd);
}
