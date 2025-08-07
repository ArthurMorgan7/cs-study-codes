// 交替打印 a b c

#include <iostream>
#include <thread>

using namespace std;

void printChar(char c){
    while(1){
        cout << c << endl;
    }
}

int main(){
    thread t1(printChar, 'a');
    thread t2(printChar, 'b');
    thread t3(printChar, 'c');

    t1.join();
    t2.join();
    t3.join();
    
    return 0;
}