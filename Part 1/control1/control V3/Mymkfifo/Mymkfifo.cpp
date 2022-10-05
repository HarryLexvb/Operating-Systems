#include <iostream>
#include <cstdio>
#include <fcntl.h>
#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include <sstream>
#include <csignal>
#include <mutex>
#include <fstream>

void mkfifo(const char *string, int i);

int main(){
    //create a fifo 
    mkfifo("fifo", 0666);
    //enter a value and echo to fifo
    std::string input;
    std::cout << "Enter a value: ";
    std::cin >> input;
    std::cout << "Echoing to fifo" << std::endl;
    std::ofstream fifo;
    fifo.open("fifo");
    fifo << input;
    fifo.close();
    return 0;    
}

void mkfifo(const char *string, int i) {
    int fd = open(string, O_CREAT | O_EXCL | O_RDWR, i);
    if (fd == -1) {
        std::cout << "Error: " << strerror(errno) << std::endl;
        exit(1);
    }
    close(fd);
}
