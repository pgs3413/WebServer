#include "log.h"
#include<assert.h>
#include<dirent.h>
#include<iostream>
#include<iomanip>
#include<sys/stat.h>

namespace log {

Log::Log(){}

Log::~Log(){
    queue->close();
    consumer->join();
    fclose(fp);
}

void Log::init_(Level level_, const string &path_, const string &suffix_, 
        int capacity_, bool toConsole_, bool toFile_){

    std::lock_guard<std::mutex> locker(mtx);
    assert(initialized == false);
    assert(path_.length() > 0);
    assert(capacity_ > 0);

    initialized = true;
    level = level_;
    toFile = toFile_;
    toConsole = toConsole_;
    path = path_;
    suffix = suffix_;

    time_t now = time(nullptr);
    localtime_r(&now, &day);

    if(path.length() > 1 && path.back() == '/'){
        path.pop_back();
    }

    makeFile();

    std::unique_ptr<StrBlockQueue> queue_temp(new StrBlockQueue(capacity_));
    queue = std::move(queue_temp);

    std::unique_ptr<std::thread> thread_temp(new std::thread(asynConsume));
    consumer = std::move(thread_temp);

}

void Log::makeFile(){

    filename.clear();
    ostringstream filenameStream;
    filenameStream << path << '/' << day.tm_year + 1900  << '_'
    << std::setw(2) << std::setfill('0') << day.tm_mon + 1 << '_'
    << std::setw(2) << std::setfill('0') << day.tm_mday << suffix;
    filename = filenameStream.str();

    if(fp){ fclose(fp); }

    DIR *dir = opendir(path.c_str());
    if(dir == nullptr && errno == ENOENT){
        if(mkdir(path.c_str(), 0777)){
            std::cerr << "can`t mkdir path: " << path << std::endl;
            exit(1);
        }        
    }
    closedir(dir);

    fp = fopen(filename.c_str(), "a");
    if(fp == nullptr){
        std::cerr << "can`t access log file: " << filename << std::endl;
        exit(1);
    }

    string startLine;
    ostringstream startLineStream;
    startLineStream << "------ log start at " << day.tm_year + 1900 << '-'
    << std::setw(2) << std::setfill('0') << day.tm_mon + 1 << '-'
    << std::setw(2) << std::setfill('0') << day.tm_mday << ' '
    << std::setw(2) << std::setfill('0') << day.tm_hour << ':'
    << std::setw(2) << std::setfill('0') << day.tm_min << ':'
    << std::setw(2) << std::setfill('0') << day.tm_sec
    << " ------\n";
    startLine = startLineStream.str();
    fputs(startLine.c_str(), fp);
    fflush(fp);

}

void Log::asynConsume(){
     getInstance() -> consume();
}

Log * Log::getInstance(){
    static Log log;
    return &log;
}

void Log::msgStart(ostringstream &stream, Level level_){
    struct tm now;
    time_t t = time(nullptr);
    localtime_r(&t, &now);
    stream << '[' << day.tm_year + 1900  << '-'
    << std::setw(2) << std::setfill('0') << day.tm_mon + 1 << '-'
    << std::setw(2) << std::setfill('0') << day.tm_mday << ' '
    << std::setw(2) << std::setfill('0') << day.tm_hour << ':'
    << std::setw(2) << std::setfill('0') << day.tm_min << ':'
    << std::setw(2) << std::setfill('0') << day.tm_sec << ']'
    << '[' << getLevelMsg(level_) << "]"
    << '[' << std::this_thread::get_id() << ']' << ' ';
}

string Log::getLevelMsg(Level level_){
    switch (level_)
    {
    case DEBUG : return "debug";
    case INFO : return "info";
    case WARN : return "warn";
    case ERROR : return "error";
    default: return "??";
    }
}

void Log::produce_(const string &format, int start, ostringstream &stream){
    while (start < format.size())
    {
        if(format[start] == '{' && start + 1 < format.size() && format[start + 1] == '}'){
            stream << "??";
            start += 2;
        }else {
            stream << format[start];
            start += 1;
        }
    }    
}

void Log::consume(){
    assert(initialized == true);
    string msg;
    while (queue -> pop(msg))
    {
        // todo 日切时需要生成新文件
        std::lock_guard<std::mutex> locker(mtx);
        if(toConsole) std::cout << msg;
        if(toFile) {
            fputs(msg.c_str(), fp);
            fflush(fp);
        }
    }    
}

void init(Level level_, const string &path_, const string &suffix_, 
    int capacity_, bool toConsole_, bool toFile_){
    Log::getInstance()->init_(level_, path_, suffix_, capacity_, toConsole_, toFile_);
}


}