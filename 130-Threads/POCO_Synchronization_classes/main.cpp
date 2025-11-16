#include "Poco/Runnable.h"
#include "Poco/Thread.h"
#include "Poco/Mutex.h"
#include "Poco/ScopedLock.h"
#include "Poco/Event.h"
#include "Poco/Condition.h"
#include "Poco/Semaphore.h"
#include "Poco/RWLock.h"
#include <iostream>
#include <vector>
#include <queue>
#include <atomic>
#include <map>

using Poco::Runnable;
using Poco::Thread;
using Poco::Mutex;
using Poco::FastMutex;
using Poco::ScopedLock;
using Poco::Event;
using Poco::Condition;
using Poco::Semaphore;
using Poco::RWLock;
using namespace std;

// 1. Mutex и ScopedLock
class ThreadSafeCounter
{
private:
    mutable Mutex _mutex;
    int _value;
    
public:
    ThreadSafeCounter()
        : _value(0)
    {

    }
    
    void increment()
    {
        ScopedLock<Mutex> lock(_mutex);
        _value++;
    }
    
    void decrement()
    {
        ScopedLock<Mutex> lock(_mutex);
        _value--;
    }
    
    int getValue() const
    {
        ScopedLock<Mutex> lock(_mutex);
        return _value;
    }
    
    // Использование tryLock
    bool safeIncrement(int timeoutMs = 100)
    {
        if (_mutex.tryLock(timeoutMs))
        {
            _value++;
            _mutex.unlock();
            return true;
        }

        return false;
    }
};

class MutexDemoWorker : public Runnable
{
private:
    ThreadSafeCounter& _counter;
    int _iterations;
    
public:
    MutexDemoWorker(ThreadSafeCounter& counter, int iterations) 
        : _counter(counter), _iterations(iterations)
    {

    }
    
    void run() override
    {
        for (int i = 0; i < _iterations; ++i)
        {
            _counter.increment();
            Thread::sleep(10);
        }
    }
};

void mutex_demo()
{
    cout << "=== Mutex and Synchronization Demo ===" << endl;
    
    ThreadSafeCounter counter;
    vector<Thread*> threads;
    
    // Создаем несколько потоков, работающих со счетчиком
    for (int i = 0; i < 5; i++)
    {
        MutexDemoWorker* worker = new MutexDemoWorker(counter, 20);
        Thread* thread = new Thread();
        thread->start(*worker);
        threads.push_back(thread);
    }
    
    // Ожидаем завершения
    for (Thread* thread : threads)
    {
        thread->join();
        delete thread;
    }
    
    cout << "Final counter value: " << counter.getValue() << endl;
    cout << "Expected value: 100" << endl;
}

// 2. Event - механизм уведомлений
class EventProducer : public Runnable
{
private:
    Event& _event;
    int _messages;
    
public:
    EventProducer(Event& event, int messages) 
        : _event(event), _messages(messages)
    {

    }
    
    void run() override
    {
        for (int i = 0; i < _messages; ++i)
        {
            cout << "Producer sending message " << (i + 1) << endl;
            _event.set(); // Сигнализируем событие
            Thread::sleep(200);
        }

        cout << "Producer finished" << endl;
    }
};

class EventConsumer : public Runnable
{
private:
    Event& _event;
    atomic<bool> _running;
    
public:
    EventConsumer(Event& event)
        : _event(event), _running(true)
    {

    }
    
    void stop() { _running = false; }
    
    void run() override
    {
        int messageCount = 0;
        while (_running)
        {
            if (_event.tryWait(100))
            { // Ждем событие с таймаутом
                messageCount++;
                cout << "Consumer received message " << messageCount << endl;
                _event.reset();
            }
        }

        cout << "Consumer processed " << messageCount << " messages" << endl;
    }
};

void event_demo()
{
    cout << "\n=== Event Demo ===" << endl;
    
    Event event(false); // Manual reset event
    
    EventProducer producer(event, 5);
    EventConsumer consumer(event);
    
    Thread producerThread, consumerThread;
    consumerThread.start(consumer);
    producerThread.start(producer);
    
    producerThread.join();
    
    // Даем потребителю время обработать оставшиеся события
    Thread::sleep(300);
    consumer.stop();
    consumerThread.join();
}

// 3. Condition - условные переменные с очередью
template<typename T>
class ThreadSafeQueue
{
private:
    mutable Mutex _mutex;
    Condition _condition;
    queue<T> _queue;
    bool _shutdown;
    
public:
    ThreadSafeQueue()
        : _shutdown(false)
    {
        
    }
    
    void push(const T& item)
    {
        ScopedLock<Mutex> lock(_mutex);
        _queue.push(item);
        _condition.signal(); // Уведомляем ждущие потоки
    }
    
    bool pop(T& item, int timeoutMs = 0)
    {
        ScopedLock<Mutex> lock(_mutex);
        
        while (_queue.empty() && !_shutdown)
        {
            if (timeoutMs > 0)
            {
                if (!_condition.tryWait(_mutex, timeoutMs))
                {
                    return false; // Таймаут
                }
            }
            else
            {
                _condition.wait(_mutex);
            }
        }
        
        if (_shutdown && _queue.empty())
        {
            return false;
        }
        
        item = _queue.front();
        _queue.pop();

        return true;
    }
    
    void shutdown()
    {
        ScopedLock<Mutex> lock(_mutex);
        _shutdown = true;
        _condition.broadcast(); // Будим все ждущие потоки
    }
    
    size_t size() const
    {
        ScopedLock<Mutex> lock(_mutex);
        return _queue.size();
    }
};

class QueueProducer : public Runnable
{
private:
    ThreadSafeQueue<int>& _queue;
    int _items;
    
public:
    QueueProducer(ThreadSafeQueue<int>& queue, int items) 
        : _queue(queue), _items(items)
    {

    }
    
    void run() override
    {
        for (int i = 0; i < _items; ++i)
        {
            _queue.push(i);
            cout << "Produced: " << i << endl;
            Thread::sleep(50);
        }

        cout << "Producer finished" << endl;
    }
};

class QueueConsumer : public Runnable
{
private:
    ThreadSafeQueue<int>& _queue;
    atomic<bool> _running;
    int _id;
    
public:
    QueueConsumer(ThreadSafeQueue<int>& queue, int id) 
        : _queue(queue), _running(true), _id(id)
    {

    }
    
    void stop() { _running = false; }
    
    void run() override
    {
        int item;
        int consumed = 0;
        
        while (_running)
        {
            if (_queue.pop(item, 100))
            {
                consumed++;
                cout << "Consumer " << _id << " consumed: " << item << endl;
            }
        }
        cout << "Consumer " << _id << " finished, consumed: " << consumed << endl;
    }
};

void condition_demo()
{
    cout << "\n=== Condition Variable Demo ===" << endl;
    
    ThreadSafeQueue<int> queue;
    
    QueueProducer producer(queue, 10);
    QueueConsumer consumer1(queue, 1);
    QueueConsumer consumer2(queue, 2);
    
    Thread producerThread, consumerThread1, consumerThread2;
    
    consumerThread1.start(consumer1);
    consumerThread2.start(consumer2);
    producerThread.start(producer);
    
    producerThread.join();
    
    // Даем потребителям время обработать оставшиеся элементы
    Thread::sleep(300);
    
    consumer1.stop();
    consumer2.stop();
    queue.shutdown();
    
    consumerThread1.join();
    consumerThread2.join();
}

// 4. Semaphore - ограничение доступа к ресурсам
class ResourcePool
{
private:
    Semaphore _semaphore;
    vector<string> _resources;
    mutable Mutex _resourceMutex;
    
public:
    ResourcePool(int poolSize)
        : _semaphore(poolSize, poolSize)
    {
        for (int i = 0; i < poolSize; ++i)
        {
            _resources.push_back("Resource-" + to_string(i + 1));
        }
    }
    
    string acquireResource(int timeoutMs = 0)
    {
        if (timeoutMs > 0)
        {
            if (!_semaphore.tryWait(timeoutMs))
            {
                throw runtime_error("Timeout acquiring resource");
            }
        }
        else
        {
            _semaphore.wait();
        }
        
        ScopedLock<Mutex> lock(_resourceMutex);
        if (_resources.empty())
        {
            _semaphore.set();
            throw runtime_error("No resources available");
        }
        
        string resource = _resources.back();
        _resources.pop_back();
        return resource;
    }
    
    void releaseResource(const string& resource)
    {
        ScopedLock<Mutex> lock(_resourceMutex);
        _resources.push_back(resource);
        _semaphore.set();
    }
    
    int availableResources() const
    {
        ScopedLock<Mutex> lock(_resourceMutex);
        return _resources.size();
    }
};

class ResourceUser : public Runnable
{
private:
    ResourcePool& _pool;
    int _id;
    int _iterations;
    
public:
    ResourceUser(ResourcePool& pool, int id, int iterations) 
        : _pool(pool), _id(id), _iterations(iterations)
    {

    }
    
    void run() override
    {
        for (int i = 0; i < _iterations; i++)
        {
            try
            {
                string resource = _pool.acquireResource(1000);
                cout << "User " << _id << " acquired: " << resource << endl;
                
                // Использование ресурса
                Thread::sleep(100 + (_id * 20));
                
                _pool.releaseResource(resource);
                cout << "User " << _id << " released: " << resource << endl;
                
            }
            catch (const exception& e) 
            {
                cout << "User " << _id << " failed: " << e.what() << endl;
            }
        }
    }
};

void semaphore_demo()
{
    cout << "\n=== Semaphore Demo ===" << endl;
    
    ResourcePool pool(3); // Пул из 3 ресурсов
    
    vector<Thread*> threads;
    for (int i = 0; i < 5; ++i)
    {
        ResourceUser* user = new ResourceUser(pool, i + 1, 3);
        Thread* thread = new Thread();
        thread->start(*user);
        threads.push_back(thread);
    }
    
    for (Thread* thread : threads)
    {
        thread->join();
        delete thread;
    }
}

// 5. RWLock - читатели-писатели
class ThreadSafeConfig {
private:
    mutable RWLock _rwLock;
    map<string, string> _config;
    
public:
    void setValue(const string& key, const string& value)
    {
        RWLock::ScopedWriteLock lock(_rwLock);
        _config[key] = value;
        cout << "Config updated: " << key << " = " << value << endl;
    }
    
    string getValue(const string& key) const
    {
        RWLock::ScopedReadLock lock(_rwLock);
        auto it = _config.find(key);
        if (it != _config.end())
        {
            return it->second;
        }

        return "";
    }
    
    void printAll() const
    {
        RWLock::ScopedReadLock lock(_rwLock);
        cout << "Current configuration:" << endl;
        for (const auto& pair : _config)
        {
            cout << "  " << pair.first << " = " << pair.second << endl;
        }
    }
};

class ConfigReader : public Runnable
{
private:
    ThreadSafeConfig& _config;
    string _key;
    int _reads;
    
public:
    ConfigReader(ThreadSafeConfig& config, const string& key, int reads) 
        : _config(config), _key(key), _reads(reads)
    {

    }
    
    void run() override
    {
        for (int i = 0; i < _reads; i++)
        {
            string value = _config.getValue(_key);
            cout << "Reader got: " << _key << " = " << value << endl;
            Thread::sleep(50);
        }
    }
};

class ConfigWriter : public Runnable
{
private:
    ThreadSafeConfig& _config;
    string _key;
    int _writes;
    
public:
    ConfigWriter(ThreadSafeConfig& config, const string& key, int writes) 
        : _config(config), _key(key), _writes(writes)
    {

    }
    
    void run() override
    {
        for (int i = 0; i < _writes; ++i)
        {
            string value = "Value-" + to_string(i + 1);
            _config.setValue(_key, value);
            Thread::sleep(150);
        }
    }
};

void rwlock_demo()
{
    cout << "\n=== Read-Write Lock Demo ===" << endl;
    
    ThreadSafeConfig config;
    
    // Создаем писателей и читателей
    ConfigWriter writer1(config, "setting1", 3);
    ConfigWriter writer2(config, "setting2", 2);
    
    ConfigReader reader1(config, "setting1", 5);
    ConfigReader reader2(config, "setting2", 5);
    ConfigReader reader3(config, "setting1", 5);
    
    Thread w1, w2, r1, r2, r3;
    
    // Запускаем читателей и писателей
    r1.start(reader1);
    r2.start(reader2);
    r3.start(reader3);
    
    w1.start(writer1);
    w2.start(writer2);
    
    w1.join();
    w2.join();
    r1.join();
    r2.join();
    r3.join();
    
    config.printAll();
}

int main()
{
    mutex_demo();
    event_demo();
    condition_demo();
    semaphore_demo();
    rwlock_demo();
}
