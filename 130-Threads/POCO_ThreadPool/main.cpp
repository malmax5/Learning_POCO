#include "Poco/Thread.h"
#include "Poco/ThreadPool.h"
#include "Poco/Runnable.h"
#include "Poco/Activity.h"
#include <iostream>
#include <atomic>
#include <sstream>

using Poco::ThreadPool;
using Poco::Thread;
using Poco::Runnable;
using namespace std;

class PoolWorker : public Runnable
{
private:
    int _taskId;
    static atomic<int> _completedTasks;
    
public:
    PoolWorker(int taskId)
        : _taskId(taskId)
    {

    }
    
    void run() override
    {
        stringstream ss;
        ss << "Task " << _taskId << " started in thread: " 
           << Thread::current()->name() << endl;
        cout << ss.str();
        
        // Имитация работы
        Thread::sleep(100 + (_taskId % 5) * 50);
        
        int completed = ++_completedTasks;
        cout << "Task " << _taskId << " completed. Total: " << completed << endl;
    }
};

atomic<int> PoolWorker::_completedTasks(0);

void thread_pool_demo()
{
    cout << "=== Thread Pool Demo ===" << endl;
    
    // 1. Использование пула по умолчанию
    cout << "\n--- Default Thread Pool ---" << endl;
    {
        ThreadPool& pool = ThreadPool::defaultPool();
        cout << "Default pool capacity: " << pool.capacity() << endl;
        cout << "Default pool allocated: " << pool.allocated() << endl;
        cout << "Default pool available: " << pool.available() << endl;
        
        // Запускаем задачи
        for (int i = 0; i < 10; i++)
        {
            PoolWorker* worker = new PoolWorker(i + 1);
            pool.start(*worker);
            // Пул берет на себя управление памятью для Runnable
        }
        
        pool.joinAll();
        cout << "All tasks completed in default pool" << endl;
    }
    
    // 2. Создание кастомного пула
    cout << "\n--- Custom Thread Pool ---" << endl;
    {
        ThreadPool customPool(2, 4, 60, 1); // minThreads, maxThreads, idleTime, stackSize
        cout << "Custom pool capacity: " << customPool.capacity() << endl;
        
        // Запускаем больше задач, чем потоков
        for (int i = 0; i < 8; ++i)
        {
            PoolWorker* worker = new PoolWorker(100 + i);
            try
            {
                customPool.start(*worker);
                cout << "Started task " << (100 + i) << endl;
            }
            catch (const Poco::NoThreadAvailableException& e)
            {
                cout << "No threads available for task " << (100 + i) 
                     << ": " << e.displayText() << endl;
                delete worker;
            }
        }
        
        customPool.joinAll();
    }
    
    // 3. Динамическое изменение пула
    cout << "\n--- Dynamic Pool Adjustment ---" << endl;
    {
        ThreadPool dynamicPool(1, 3); // От 1 до 3 потоков
        
        cout << "Initial capacity: " << dynamicPool.capacity() << endl;
        
        // Добавляем capacity
        dynamicPool.addCapacity(2);
        cout << "After adding capacity: " << dynamicPool.capacity() << endl;
        
        for (int i = 0; i < 5; ++i)
        {
            dynamicPool.start(*new PoolWorker(200 + i));
        }
        
        dynamicPool.joinAll();
    }
}

// 4. Мониторинг состояния пула
class PoolMonitor : public Runnable
{
private:
    ThreadPool& _pool;
    bool _running;
    
public:
    PoolMonitor(ThreadPool& pool)
        : _pool(pool), _running(true)
    {

    }
    
    void stop()
    {
        _running = false;
    }
    
    void run() override
    {
        while (_running)
        {
            cout << "Pool stats - Capacity: " << _pool.capacity()
                 << ", Allocated: " << _pool.allocated()
                 << ", Available: " << _pool.available() << endl;
            
            Thread::sleep(500);
        }
    }
};

void monitored_pool_demo()
{
    cout << "\n=== Monitored Thread Pool ===" << endl;
    
    ThreadPool pool(2, 4);
    PoolMonitor monitor(pool);
    
    // Запускаем монитор в отдельном потоке
    Thread monitorThread;
    monitorThread.start(monitor);
    
    // Запускаем рабочие задачи
    for (int i = 0; i < 10; i++)
    {
        pool.start(*new PoolWorker(300 + i));
        Thread::sleep(150);
    }
    
    pool.joinAll();
    
    // Останавливаем монитор
    monitor.stop();
    monitorThread.join();
}

int main()
{
    thread_pool_demo();
    monitored_pool_demo();
}
