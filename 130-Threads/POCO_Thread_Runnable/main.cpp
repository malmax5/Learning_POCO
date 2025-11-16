#include "Poco/Thread.h"
#include "Poco/Runnable.h"
#include "Poco/RunnableAdapter.h"
#include <iostream>
#include <vector>
#include <chrono>

using Poco::Thread;
using Poco::Runnable;
using Poco::RunnableAdapter;
using namespace std;

// 1. Классический подход с наследованием от Runnable
class SimpleWorker : public Runnable
{
private:
    int _id;
    int _iterations;
    
public:
    SimpleWorker(int id, int iterations)
        : _id(id), _iterations(iterations)
    {

    }
    
    void run() override
    {
        Thread* currentThread = Thread::current();
        cout << "Worker " << _id << " started in thread: " 
             << (currentThread ? currentThread->name() : "main") << endl;
        
        for (int i = 0; i < _iterations; ++i) {
            cout << "Worker " << _id << " - iteration " << i << endl;
            Thread::sleep(100); // Миллисекунды
        }
        
        cout << "Worker " << _id << " finished" << endl;

        // std::cout << "--- Thread Properties (from inside thread) ---" << std::endl;
        // std::cout << "ID (Poco):        " << pThread->id() << std::endl;
        // std::cout << "TID (native):     " << pThread->tid() << std::endl;
        // std::cout << "Name:             " << pThread->name() << std::endl;
        // std::cout << "Is running:       " << (pThread->isRunning() ? "true" : "false") << std::endl;
        // std::cout << "Priority:         " << pThread->getPriority() << std::endl;
        // std::cout << "OS Priority:      " << pThread->getOSPriority() << std::endl;
        // std::cout << "Stack size:       " << pThread->getStackSize() << " bytes" << std::endl;

        // Дополнительно: мин/макс приоритеты (для справки)
        // std::cout << "Min OS Priority:  " << Thread::getMinOSPriority() << std::endl;
        // std::cout << "Max OS Priority:  " << Thread::getMaxOSPriority() << std::endl;

        // Просто для демонстрации — немного поспим
        Poco::Thread::sleep(100);
    }
};

// 2. Использование функторов с RunnableAdapter
class FunctorWorker
{
private:
    int _id;
    
public:
    FunctorWorker(int id)
        : _id(id)
    {

    }
    
    void operator()()
    {
        cout << "Functor worker " << _id << " executing" << endl;
        for (int i = 0; i < 3; ++i)
        {
            cout << "Functor " << _id << " - step " << i << endl;
            Thread::sleep(200);
        }
    }
};

void basic_thread_demo()
{
    cout << "=== Basic Thread Demo ===" << endl;
    
    // Демонстрация приоритетов потоков
    cout << "\n--- Thread Priorities ---" << endl;
    cout << "PRIO_LOWEST: " << Thread::PRIO_LOWEST << endl;
    cout << "PRIO_LOW: " << Thread::PRIO_LOW << endl;
    cout << "PRIO_NORMAL: " << Thread::PRIO_NORMAL << endl;
    cout << "PRIO_HIGH: " << Thread::PRIO_HIGH << endl;
    cout << "PRIO_HIGHEST: " << Thread::PRIO_HIGHEST << endl;
    
    // Создание потоков с Runnable
    cout << "\n--- Using Runnable ---" << endl;
    SimpleWorker worker1(1, 5);
    SimpleWorker worker2(2, 3);
    
    Thread thread1("WorkerThread-1");
    Thread thread2("WorkerThread-2");
    
    // Установка приоритетов
    thread1.setPriority(Thread::PRIO_HIGH);
    thread2.setPriority(Thread::PRIO_LOW);
    
    thread1.start(worker1);
    thread2.start(worker2);
    
    // Ожидание завершения
    thread1.join();
    thread2.join();
    
    // Использование RunnableAdapter
    cout << "\n--- Using RunnableAdapter ---" << endl;
    FunctorWorker functor1(1);
    FunctorWorker functor2(2);
    
    RunnableAdapter<FunctorWorker> adapter1(functor1, &FunctorWorker::operator());
    RunnableAdapter<FunctorWorker> adapter2(functor2, &FunctorWorker::operator());
    
    Thread thread3, thread4;
    thread3.start(adapter1);
    thread4.start(adapter2);
    
    thread3.join();
    thread4.join();
}

// 3. Продвинутый пример с управлением группой потоков
class ThreadGroupManager
{
private:
    vector<Thread*> _threads;
    vector<SimpleWorker*> _workers;
    
public:
    ~ThreadGroupManager()
    {
        cleanup();
    }
    
    void startWorkers(int count, int iterations)
    {
        for (int i = 0; i < count; ++i)
        {
            SimpleWorker* worker = new SimpleWorker(i + 1, iterations);
            Thread* thread = new Thread("Worker-" + to_string(i + 1));
            
            _workers.push_back(worker);
            _threads.push_back(thread);
            
            thread->start(*worker);
        }
    }
    
    void waitAll()
    {
        for (Thread* thread : _threads)
        {
            thread->join();
        }
    }
    
    void stopAll()
    {
        // Для корректной остановки нужно реализовать механизм прерывания
        waitAll();
    }
    
private:
    void cleanup() {
        for (Thread* thread : _threads)
        {
            delete thread;
        }

        for (SimpleWorker* worker : _workers)
        {
            delete worker;
        }

        _threads.clear();
        _workers.clear();
    }
};

void thread_group_demo()
{
    cout << "\n=== Thread Group Demo ===" << endl;
    
    ThreadGroupManager manager;
    manager.startWorkers(4, 3);
    manager.waitAll();
}

int main()
{
    basic_thread_demo();
    Thread::sleep(10);
    thread_group_demo();
}
