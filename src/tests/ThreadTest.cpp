#include <iostream>
#include <thread>
#include <ctime>   
#include <unistd.h>
using namespace std;


void runThreadForOneSec(int value){
    time_t now = time(0); // get current dat/time with respect to system  

    char* dt = ctime(&now); // convert it into string  

    cout << "On thread number " << value<<". The local date and time is: " << dt << endl; // print local date and time  
    sleep(10);
    now = time(0); // get current dat/time with respect to system  

    dt = ctime(&now); // convert it into string  
  
    cout << "On thread number " << value<<". The local date and time is: " << dt << endl; // print local date and time  
}


int main()
{
    cout<<"started"<<endl;
    std::thread thread1(runThreadForOneSec,1);
    sleep(3);
    std::thread thread2(runThreadForOneSec,2);

    thread1.join();
    thread2.join();
    return 0;
}
