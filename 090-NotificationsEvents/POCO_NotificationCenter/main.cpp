// Базовые нотификации: система оповещений о состоянии сервера
#include "Poco/NotificationCenter.h"
#include "Poco/Notification.h"
#include "Poco/Observer.h"
#include "Poco/AutoPtr.h"
#include <iostream>

// Базовый класс состояния сервера
class ServerNotification : public Poco::Notification
{
public:
    ServerNotification(const std::string& message)
        : _message(message)
    {

    }

    std::string getMessage() const
    {
        return _message;
    }

    virtual std::string getType() const = 0;

private:
    std::string _message;
};

// Класс старта сервера
class ServerStartNotification : public ServerNotification
{
public:
    ServerStartNotification(const std::string& message)
        : ServerNotification(message)
    {

    }

    std::string getType() const override
    {
        return "START";
    }
};

// Класс ошибки сервера
class ServerErrorNotification : public ServerNotification
{
public:
    ServerErrorNotification(const std::string& message, int code)
        : ServerNotification(message),
          _errorCode(code)
    {

    }

    std::string getType() const override
    {
        return "ERROR";
    }

    int getErrorCode() const { return _errorCode; }
    
private:
    int _errorCode;
};

/*
    Класс-наблюдатель -> Logger
    Имеет ручки для разных server events, чтобы их логрировать
*/
class ServerLogger
{
public:
    // Обычные сырые указатели
    void handleStart(ServerStartNotification* pNf)
    {
        std::cout << "[LOG] Server started: "
                  << pNf->getMessage() << "\n";
        pNf->release(); // ! Освобождаем владение
    }

    void handleError(ServerErrorNotification* pNf)
    {
        std::cout << "[LOG] Server error " 
                  << pNf->getErrorCode() << ": "
                  << pNf->getMessage() << "\n";
        pNf->release(); // ! Освобождаем владение
    }

    // Перегрузка с AutoPtr
    void handleStart(const Poco::AutoPtr<ServerStartNotification>& pNf)
    {
        std::cout << "[LOG] Server started: "
                  << pNf->getMessage() << "\n";
        // AutoPtr автоматически освободит владение
    }

    void handleError(const Poco::AutoPtr<ServerErrorNotification> pNf)
    {
        std::cout << "[LOG] Server error " 
                  << pNf->getErrorCode() << ": "
                  << pNf->getMessage() << "\n";
        // AutoPtr автоматически освободит владение
    }
};

/*
    Класс-наблюдатель -> Monitor
    Подписывается на получение ServerNotification
    => абсолютно на все оповещения сервера, которые inherits by ServerNotification
    Имеет одну ручку на все ивенты
*/
class ServerMonitor
{
public:
    // Обычный сырой указатель
    void handleAnyServerEvent(ServerNotification* pNf)
    {
        std::cout << "[MONITOR] Server event " << pNf->getType()
                  << ": " << pNf->getMessage() << "\n";
        pNf->release(); // ! Освобождаем владение
    }

    // Перегрузка с AutoPtr
    void handleAnyServerEvent(const Poco::AutoPtr<ServerNotification> pNf)
    {
        std::cout << "[MONITOR] Server event " << pNf->getType()
                  << ": " << pNf->getMessage() << "\n";
        // AutoPtr автоматически освободит владение
    }
};

int main()
{
    // Центр оповещений
    Poco::NotificationCenter nc;

    // Наши Observers
    ServerLogger logger;
    ServerMonitor monitor;

    // Регестрируем Observers в центре оповещений
    nc.addObserver(Poco::Observer<ServerLogger, ServerStartNotification>(
        logger, &ServerLogger::handleStart
    ));
    nc.addObserver(Poco::Observer<ServerLogger, ServerErrorNotification>(
        logger, &ServerLogger::handleError
    ));

    nc.addObserver(Poco::Observer<ServerMonitor, ServerNotification>(
        monitor, &ServerMonitor::handleAnyServerEvent
    ));

    // Отправляем уведомления
    std::cout << "=== Sending notifications ===" << std::endl;
    nc.postNotification(new ServerStartNotification("Web server initialized"));
    nc.postNotification(new ServerErrorNotification("Database connection failed", 500));

    // Отписываемся
    nc.removeObserver(Poco::Observer<ServerLogger, ServerStartNotification>(
        logger, &ServerLogger::handleStart
    ));
    
    // Отправляем уведомления после отписки
    std::cout << "\n=== After unsubscribing from START ===" << std::endl;
    nc.postNotification(new ServerStartNotification("This won't be logged"));
    
    // Отписываемся перед завершением
    nc.removeObserver(Poco::Observer<ServerLogger, ServerErrorNotification>(
        logger, &ServerLogger::handleError
    ));
    nc.removeObserver(Poco::Observer<ServerMonitor, ServerNotification>(
        monitor, &ServerMonitor::handleAnyServerEvent
    ));
    
    return 0;
}
