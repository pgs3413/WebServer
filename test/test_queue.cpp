#include "../src/queue/strblockqueue.h"
#include<thread>
#include<iostream>
#include<fstream>
#include<signal.h>

using std::cout;
using std::cin;

class test_queue{
private:
    StrBlockQueue queue;
    std::unique_ptr<std::thread> consumer ;
    std::ofstream fout;
    int sleepSecond;
public:
    test_queue (int capacity, const string &filename, int sleepSecond = 10);
    ~test_queue();
    void start();
    void consume();
    void push(const string &s);
};

test_queue::test_queue(int capacity, const string &filename, int sleepSecond) 
: queue(capacity) , sleepSecond(sleepSecond) {
    fout.open(filename);
}

test_queue::~test_queue(){
    cout << "test_queue destroyed in thread: " << std::this_thread::get_id() << std::endl;
    queue.close();
    consumer->join();
    fout.close();
}

void test_queue::start(){
    std::unique_ptr<std::thread> t(new std::thread([this]{this->consume();}));
    consumer = std::move(t);
}

void test_queue::consume(){
    fout << "current thread: " << std::this_thread::get_id() << std::endl;
    string s;
    while (queue.pop(s))
    {
        fout << "consume message: " << s << std::endl;
        if(s == "sleep") std::this_thread::sleep_for(std::chrono::seconds(sleepSecond));
    }
}

void test_queue::push(const string &s){
    queue.push(s);
}

void hander(int sig){
    exit(0);
}

int main(){

    signal(SIGINT, hander);
    cout << "current thread: " << std::this_thread::get_id() << std::endl;
    static test_queue tq(100,"test_queue.txt");
    tq.start();

    while (1)
    {
        string s;
        cout << "please input a message: ";
        getline(cin,s);
        if(s == "exit") break;
        tq.push(s);
    }
    

    exit(0);
}