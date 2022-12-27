#include <iostream>       // std::cout
#include <thread>         // std::thread, std::this_thread::sleep_for
#include <chrono>         // std::chrono::seconds
#include <string>
#include <mutex>

void test_thread(int n) {
    for (std::size_t i(0); i < 10; ++i)
        std::cout << "thread " << n << " --> " << i << std::endl;
    std::cout << "end thread " << n << std::endl;
}

/*
void main_thread(int n){
    for (std::size_t i(0); i < 10; ++i)
        std::cout << "main " << n << " --> " << i  << std::endl;
    std::cout << "end main " << n << std::endl;
}*/

int main(){
    for(int i(0); i < 10; ++i){
        std::thread t1(test_thread, i);
        //std::thread t2(main_thread, i);
        t1.join();
        //t2.join();
    }
}
//g++ -o thread.exe -pthread thread.cpp
// ./thread.exe
