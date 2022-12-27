// example for thread::join
// g++ thread.c -o thread.exe -std=c++11 -lpthread

#include <iostream>       // std::cout
#include <thread>         // std::thread, std::this_thread::sleep_for
#include <chrono>         // std::chrono::seconds


void pause_thread(int n)
{
  std::cout << "Thread: " << n << "\n";
}

int main()
{
  for (int i = 0; i < 100; ++i)
  {
    std::thread(pause_thread,i).detach();
  }
  
  
  std::cout << "(the main thread will now pause for 5 second)\n"; 
  //pause_thread(1000)
  std::this_thread::sleep_for(std::chrono::seconds(1000));

  return 0;
}
