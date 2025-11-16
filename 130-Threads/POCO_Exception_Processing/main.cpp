#include "Poco/Runnable.h"
#include "Poco/Thread.h"
#include "Poco/ErrorHandler.h"
#include "Poco/Exception.h"
#include <iostream>
#include <stdexcept>

using Poco::Runnable;
using Poco::Thread;
using Poco::ErrorHandler;
using Poco::Exception;
using namespace std;

class CustomErrorHandler : public ErrorHandler
{
public:
    void exception(const Exception& exc) override
    {
        cerr << "POCO Exception in thread: " << exc.displayText() << endl;
    }
    
    void exception(const std::exception& exc) override
    {
        cerr << "std::exception in thread: " << exc.what() << endl;
    }
    
    void exception() override
    {
        cerr << "Unknown exception in thread" << endl;
    }
};

class FaultyWorker : public Runnable
{
private:
    int _workerType;
    
public:
    FaultyWorker(int type)
        : _workerType(type)
    {

    }
    
    void run() override
    {
        string threadName = Thread::current() ? 
                            Thread::current()->name() : "unknown";
        
        cout << "FaultyWorker " << _workerType << " running in " << threadName << endl;
        
        switch (_workerType)
        {
            case 1:
                throw Poco::RuntimeException("POCO runtime error occurred");
            case 2:
                throw std::runtime_error("std::runtime_error occurred");
            case 3:
                throw "C-string exception"; // Нестандартное исключение
            case 4: // Аварийное завершение программы все равно будет, т.к. вызывается сигнал SIGSEGV, а не ошибка
                int* ptr = nullptr;
                *ptr = 42; // Segmentation fault
                break;
        }
        
        cout << "FaultyWorker " << _workerType << " completed normally" << endl;
    }
};

void error_handling_demo()
{
    cout << "=== Error Handling Demo ===" << endl;
    
    // Устанавливаем кастомный обработчик ошибок
    CustomErrorHandler customHandler;
    ErrorHandler* oldHandler = ErrorHandler::set(&customHandler);
    
    cout << "Testing different exception types in threads..." << endl;
    
    // Тестируем различные типы исключений
    vector<Thread*> threads;
    for (int i = 1; i < 4; ++i)
    {
        FaultyWorker* worker = new FaultyWorker(i);
        Thread* thread = new Thread("FaultyThread-" + to_string(i));
        thread->start(*worker);
        threads.push_back(thread);
    }
    
    // Ожидаем завершения (или аварийного завершения) потоков
    for (Thread* thread : threads)
    {
        try
        {
            thread->join();
            cout << "Thread " << thread->name() << " joined successfully" << endl;
        }
        catch (const Exception& e)
        {
            cerr << "Exception while joining thread: " << e.displayText() << endl;
        }

        delete thread;
    }
    
    // Восстанавливаем старый обработчик
    ErrorHandler::set(oldHandler);
    
    cout << "Error handling demo completed" << endl;
}

int main()
{
    error_handling_demo();
}
