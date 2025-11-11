// Асинхронные уведомления: система обработки задач с рабочими потоками
#include "Poco/NotificationQueue.h"
#include "Poco/Notification.h"
#include "Poco/Thread.h"
#include "Poco/Runnable.h"
#include "Poco/AutoPtr.h"
#include <iostream>
#include <atomic>

// Задача для обработки
class TaskNotification : public Poco::Notification
{
public:
    TaskNotification(int id, const std::string& data)
        : _taskId(id),
          _taskData(data)
    {

    }

    int getTaskId() const 
    {
        return _taskId;
    }

    std::string getTaskData() const
    {
        return _taskData;
    }

private:
    int _taskId;
    std::string _taskData;
};

// Специальное уведомление для остановки
class QuitNotification : public Poco::Notification
{

};

// Рабочий поток
class Worker : public Poco::Runnable
{
public:
    Worker(const std::string& name, Poco::NotificationQueue& queue)
        : _name(name),
          _queue(queue)
    {

    }

    void run() override
    {
        std::cout << _name << " started" << "\n";

        Poco::AutoPtr<Poco::Notification> pNf(_queue.waitDequeueNotification());

        while (pNf && !dynamic_cast<QuitNotification*>(pNf.get()))
        {
            TaskNotification* pTask = dynamic_cast<TaskNotification*>(pNf.get());
            if (pTask)
            {
                Poco::Thread::sleep(pTask->getTaskId());
                std::cout << _name << " processing task " << pTask->getTaskId()
                          << ": " << pTask->getTaskData() << "\n";
                
                Poco::Thread::sleep(1000 + pTask->getTaskId());
            }

            pNf = _queue.waitDequeueNotification();
        }

        std::cout << _name << " stopped" << "\n";
    }

private:
    std::string _name;
    Poco::NotificationQueue& _queue;
};

int main()
{
    Poco::NotificationQueue queue;

    Worker worker1("Worker-1", queue);
    Worker worker2("Worker-2", queue);
    Worker worker3("Worker-3", queue);

    std::cout << "=== Creating Threads ===" << "\n";
    Poco::Thread t1, t2, t3;
    t1.start(worker1);
    t2.start(worker2);
    t3.start(worker3);

    Poco::Thread::sleep(1000); // Ждем инициализации потоков

    std::cout << "\n=== Sending tasks ===" << "\n";
    for (int i = 0; i <= 6; i++)
    {
        std::string data = "Task data " + std::to_string(i);
        if (i % 2 == 0)
        {
            queue.enqueueUrgentNotification(new TaskNotification(i, "[URGENT] " + data));
        }
        else
        {
            queue.enqueueNotification(new TaskNotification(i, data));
        }
    }

    Poco::Thread::sleep(1000); // Обрабатываем

    std::cout << "\n=== Stopping workers ===" << "\n";
    queue.enqueueNotification(new QuitNotification());
    queue.enqueueNotification(new QuitNotification());
    queue.enqueueNotification(new QuitNotification());

    t1.join();
    t2.join();
    t3.join();
}

