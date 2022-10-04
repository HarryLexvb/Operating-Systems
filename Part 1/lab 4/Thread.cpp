#include <iostream>       // std::cout
#include <thread>         // std::thread, std::this_thread::sleep_for
#include <chrono>         // std::chrono::seconds
#include <string>

int  VG; 

void pause_thread(int n)
{
  while(VG!=n){}; // pseudo lock 
  VG++; // estamos generando una secuencia 
  std::cout << "Thread with ID: " << n << "\n";  // Sec Cri (1 th debe de exe esta linea )

}

int main()
{
   std::string n;

  std::cout << "Spawning and detaching 3 threads...\n";
  for(int x=1; x<10 ; x++){
    std::thread (pause_thread,x).detach();
  }
  std::cout << "press enter \n";
  std::cin >> n;
  VG=1;
  for(;;){} // busy wait 
  return 0;
}

/*
Spawning and detaching 3 threads...
End of threads
Thread with ID: 7
Thread with ID: 8
Thread with ID: 9
Thread with ID: 6
Thread with ID: 5
Thread with ID: 4
Thread with ID: 3
Thread with ID: 2
Thread with ID: 1
*/