#include <iostream>
#include <thread>

using namespace std;

void firstThread()
{
    int arr;
    for (arr = 0; arr < 100; arr++)
    {
        arr = arr * 1;
        cout << "first thread: " << arr << "\n";
    }
}

void secondThread()
{
    int i = 100;
    int arr2[i];
    for (arr2[i] = 0; arr2[i] < 100; arr2[i]++)
    {
        arr2[i] = arr2[i] * 2;
        ///cout << arr2[i] << "\n"; NOT ALLOWED
    }
}

int main()
{
    thread fill1(firstThread);
    thread fill2(secondThread);

    fill1.join();
    fill2.join();
}