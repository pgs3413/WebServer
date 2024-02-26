#ifndef LOG_H_
#define LOG_H_

#include "../queue/blockqueue.h"
#include<string>
#include<thread>
#include<mutex>
#include<time.h>
#include<assert.h>
#include<sstream>
#include<stdexcept>

namespace log {

using std::string;
using std::ostringstream;

enum Level{DEBUG,INFO,WARN,ERROR};

class Log{

private:

    Level level = INFO;
    bool toFile = false;
    bool toConsole = false;
    bool initialized = false;

    struct tm day;
    string filename;
    string path;
    string suffix;
    FILE* fp = nullptr;

    std::unique_ptr<BlockQueue<string>> queue = nullptr;
    std::unique_ptr<std::thread> consumer = nullptr;

    std::mutex mtx;

    Log();
    ~Log();

    void init_(Level level_, const string &path_, const string &suffix_, 
        int capacity_, bool toConsole_, bool toFile_);
    void makeFile();

    static Log * getInstance();    
    static void asynConsume();

    template <typename... Args>
    void produce(Level level_, const string &format, Args... args);
    template<typename T,typename... Args>
    void produce_(const string &format, int start, ostringstream &stream, T t, Args... args);
    void produce_(const string &format, int start, ostringstream &stream);

    void msgStart(ostringstream &stream, Level level_);
    string getLevelMsg(Level level_);

    void consume();

public:
    
    friend void init(Level level_, const string &path_, const string &suffix_, 
        int capacity_, bool toConsole_, bool toFile_);
    template<typename... Args>
    friend void debug(const string &format, Args... args);
    template<typename... Args>
    friend void info(const string &format, Args... args);
    template<typename... Args>
    friend void warn(const string &format, Args... args);
    template<typename... Args>
    friend void error(const string &format, Args... args);

};
 


template <typename... Args>
void Log::produce(Level level_, const string &format, Args... args){
    assert(initialized == true);
    if(level_ < level) return;
    ostringstream stream;
    msgStart(stream, level_);
    produce_(format, 0, stream, args...);
    stream << '\n';
    queue->push(stream.str());
}

template<typename T,typename... Args>
void Log::produce_(const string &format, int start, ostringstream &stream, T t, Args... args){
    if(start >= format.size()) return;
    while (start < format.size())
    {
        if(format[start] == '{' && start + 1 < format.size() && format[start + 1] == '}'){
            stream << t;
            start += 2;
            break;
        }else {
            stream << format[start];
            start += 1;
        }
    }
    produce_(format, start, stream, args...);
}

template<typename... Args>
void debug(const string &format, Args... args){
    Log::getInstance()->produce(DEBUG, format, args...);
}
template<typename... Args>
void info(const string &format, Args... args){
    Log::getInstance()->produce(INFO, format, args...);
}
template<typename... Args>
void warn(const string &format, Args... args){
    Log::getInstance()->produce(WARN, format, args...);
}
template<typename... Args>
void error(const string &format, Args... args){
    Log::getInstance()->produce(ERROR, format, args...);
}

void init(Level level_, const string &path_, const string &suffix_, 
        int capacity_, bool toConsole_, bool toFile_);

}


#endif