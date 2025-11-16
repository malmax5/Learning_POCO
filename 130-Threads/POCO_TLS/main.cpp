#include "Poco/ThreadLocal.h"
#include "Poco/Thread.h"
#include "Poco/Runnable.h"
#include <iostream>
#include <map>
#include <sstream>

using Poco::Thread;
using Poco::Runnable;
using Poco::ThreadLocal;
using namespace std;

class TLSUser : public Runnable
{
private:
    // Статическая TLS переменная - у каждого потока своя копия
    static ThreadLocal<int> _threadCounter;
    static ThreadLocal<string> _threadName;
    static ThreadLocal<map<string, int>> _threadStats;
    
    int _userId;
    
public:
    TLSUser(int id)
        : _userId(id)
    {
        
    }
    
    void run() override
    {
        // Инициализация TLS для этого потока
        *_threadCounter = 0;
        *_threadName = "Thread-" + to_string(_userId);
        
        // Работа с TLS
        for (int i = 0; i < 5; ++i)
        {
            (*_threadCounter)++;
            
            // Использование сложных объектов в TLS
            (*_threadStats)["iterations"] = *_threadCounter;
            (*_threadStats)["userId"] = _userId;
            
            printStatus();
            Thread::sleep(100);
        }
        
        printFinalStats();
    }
    
private:
    void printStatus()
    {
        stringstream ss;
        ss << *_threadName << " - Counter: " << *_threadCounter 
           << ", User: " << _userId << endl;
        cout << ss.str();
    }
    
    void printFinalStats()
    {
        cout << "Final stats for " << *_threadName << ":" << endl;
        for (const auto& pair : *_threadStats) {
            cout << "  " << pair.first << ": " << pair.second << endl;
        }
    }
};

// Статические члены должны быть определены
ThreadLocal<int> TLSUser::_threadCounter;
ThreadLocal<string> TLSUser::_threadName;
ThreadLocal<map<string, int>> TLSUser::_threadStats;

// Более сложный пример с пользовательскими типами
class ThreadContext
{
public:
    string name;
    int counter;
    vector<string> history;
    
    ThreadContext()
        : counter(0)
    {
        history.reserve(10);
    }
    
    void addEvent(const string& event)
    {
        history.push_back(event);
        counter++;
    }
    
    void printHistory() const
    {
        cout << "History for " << name << ":" << endl;
        for (const auto& event : history)
        {
            cout << "  - " << event << endl;
        }
    }
};

class ContextAwareWorker : public Runnable
{
private:
    static ThreadLocal<ThreadContext> _context;
    int _workerId;
    
public:
    ContextAwareWorker(int id)
        : _workerId(id)
    {

    }
    
    void run() override
    {
        // Инициализация контекста
        _context->name = "Worker-" + to_string(_workerId);
        _context->addEvent("Started");
        
        for (int i = 0; i < 3; ++i)
        {
            string event = "Iteration " + to_string(i + 1);
            _context->addEvent(event);
            
            cout << _context->name << " completed " << event << endl;
            Thread::sleep(150);
        }
        
        _context->addEvent("Finished");
        _context->printHistory();
    }
};

ThreadLocal<ThreadContext> ContextAwareWorker::_context;

void tls_demo()
{
    cout << "=== Thread Local Storage Demo ===" << endl;
    
    // Демонстрация базового TLS
    cout << "\n--- Basic TLS Demo ---" << endl;
    {
        TLSUser user1(1);
        TLSUser user2(2);
        TLSUser user3(3);
        
        Thread t1, t2, t3;
        t1.start(user1);
        t2.start(user2);
        t3.start(user3);
        
        t1.join();
        t2.join();
        t3.join();
    }
    
    // Демонстрация TLS с пользовательскими типами
    cout << "\n--- Advanced TLS with Custom Types ---" << endl;
    {
        ContextAwareWorker worker1(1);
        ContextAwareWorker worker2(2);
        
        Thread t1, t2;
        t1.start(worker1);
        t2.start(worker2);
        
        t1.join();
        t2.join();
    }
}

int main()
{
    tls_demo();
}
