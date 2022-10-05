#include <iostream>
#include <cstdio>
#include <fcntl.h>
#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include <sstream>
#include <csignal>
#include <mutex>

std::mutex mtx;

void foo(int sigint){ mtx.unlock(); } // This function is executed when a SIGINT is received

int main(){
    signal(SIGINT, foo); // Register the function foo to be executed when a SIGINT is received
    int *fd = new int[2];
    pipe(fd);
    int *fd1 = new int[2];
    pipe(fd1);
    int n_bytes; // Number of bytes read

    pid_t child_pid;
    char *read_buffer = new char[100];
    int c;

    if((child_pid = fork()) == -1){
        perror("fork");
        exit(1);
    }
    else if(child_pid == 0){
        std::cout << "Child process\n" << std::endl;

        //read from pipe
        n_bytes = read(fd[0], read_buffer, sizeof(read_buffer));
        std::cout << "Child received: " << read_buffer << std::endl;
        //convert string to int and multiply by 11
        std::stringstream ss(read_buffer);
        ss >> c;
        c *= 11;
        //convert int to string
        std::string s = std::to_string(c);
        //write to pipe
        write(fd1[1], s.c_str(), s.length());
        std::cout << "Child sent: " << s << std::endl;
        exit(0);
    }
    else{
        std::cout << "Parent program\n" << std::endl;
        mtx.lock();
        auto *file_fifo = new std::string("fifo");
        // buffer for writing to fifo
        char *write_buffer = new char[100];
        // open fifo
        int fifo = open(file_fifo->c_str(), O_WRONLY);
        // read from fifo
        n_bytes = read(fifo, write_buffer, sizeof(write_buffer));
        //write string to pipe
        n_bytes = write(fd[1], write_buffer, n_bytes);
        std::cout << "parent has sent string: " << write_buffer << std::endl;
        char *result = new char[100];
        //sleep for 3 seconds
        system("sleep 3");
        // loop until SIGINT is received
        while(mtx.try_lock()){
            n_bytes = read(fd1[0], result, sizeof(result));
            std::cout << "parent received: " << result << ", Nbytes: " << n_bytes << std::endl;
            sleep(1);
        }
    }
}

