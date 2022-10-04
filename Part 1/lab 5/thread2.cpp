#include <iostream>       // std::cout
#include <thread>         // std::thread, std::this_thread::sleep_for
#include <chrono>         // std::chrono::seconds
#include <string>
#include <mutex>



std::mutex mtx; 
int  VG; 

void pause_thread(int n)
{
  //while(VG!=n){}; // pseudo lock 
  mtx.lock();   
  //VG=VG+1; // estamos generando una secuencia 
  VG=VG*1; // estamos generando una secuencia 
  std::cout << "Thread with ID: " << n << " vg: " << VG <<"\n";  // Sec Cri (1 th debe de exe esta linea )
  mtx.unlock(); 
}

int main()
{
  std::string n;
  VG=0;
  std::cout << "Spawning and detaching 3 threads...\n";
  for(int x=1; x<100 ; x++){
    //std::thread(pause_thread,x).detach();
    std::fill1(pause_thread(i))
  }
  std::this_thread::sleep_for(std::chrono::seconds(1000));
  //std::cout << "press enter \n";
  //std::cin >> n;
  //VG=1;
  //for(;;){} // busy wait 
  fill1.join(); 
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
