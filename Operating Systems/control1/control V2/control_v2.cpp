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

    // Register the function foo to be executed when a SIGINT is received
    signal(SIGINT, foo);
    // Create a pipe
    int *fd = new int[2];
    // Create a pipe
    pipe(fd);
    // Create a pipe
    int *fd1 = new int[2];
    // Create a pipe
    pipe(fd1);
    int n_bytes; // Number of bytes read

    pid_t child_pid; // Child process id
    char *read_buffer = new char[100]; // Buffer to read from the pipe

    if((child_pid = fork()) == -1) {    // Create a child process
        perror("fork"); // Print the error
        exit(1); // Exit with error
    }

    else if(child_pid == 0) {
        // Child process
        std::cout << "Child program:\n" << std::endl;
        // Read from the pipe
        n_bytes = read(fd[0], read_buffer, sizeof(read_buffer));
        // Print the message
        std::cout << "Child recieved: " << read_buffer << std::endl;
        // Convert the message to an integer and multiply it by 11
        int temp = std::stoi(read_buffer) * 11;
        // Create a buffer to store the result
        char *temp_str = new char[100];
        // Create a stringstream to convert the integer to a string
        std::stringstream ss;
        // Write the result to the stringstream
        ss << temp;
        // Convert the stringstream to a string
        std::string str = ss.str();
        // Copy the string to the buffer
        strcpy(temp_str, str.c_str());
        // Write the result to the pipe
        write(fd1[1], temp_str, strlen(temp_str));
        // Print the result
        std::cout << "Child sending: " << temp_str << std::endl;
        exit(0); // Exit
    } else {

        // Parent process
        std::cout <<"Parent program:\n" << std::endl;
        // Lock the mutex
        mtx.lock();
        // Create a fifo
        char *fifo = "fifo";
        // Create a buffer to store the data
        char *data = new char[100];
        // Open the fifo
        int fifo_fd = open(fifo, O_RDONLY);
        // Read from the fifo
        read(fifo_fd, data, sizeof(data));
        // Write to the pipe
        n_bytes = write(fd[1], data, strlen(data));
        // Print the message
        printf("Parent has sent string: %s\n", data);
        // Create a buffer to store the result
        char *result = new char[100];
        //   Sleep for 4 seconds
        system("sleep 4");
        while (mtx.try_lock()) { // Wait for the child to write to the pipe
            // Read from the pipe
            n_bytes = read(fd1[0], result, 3);
            // Print the result
            std::cout << "Parent received: " << result << ", Nbytes: " << n_bytes << std::endl;
            system("sleep 1"); // Sleep for 1 second
        }
    }
}

