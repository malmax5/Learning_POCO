// Практический пример - иерархия логгеров
#include "Poco/Logger.h"
#include "Poco/ConsoleChannel.h"
#include "Poco/SimpleFileChannel.h"
#include "Poco/AutoPtr.h"
#include <iostream>

using Poco::Logger;
using Poco::ConsoleChannel;
using Poco::SimpleFileChannel;
using Poco::AutoPtr;
using Poco::Message;

/*
    Древовидная структура имен:
    - Корневой логгер: "" (пустая строка)
    - Дочерние логгеры: "parent.child.grandchild"
    - Разделитель: точка (.)

    Наследование свойств:
    - Новый логгер наследует уровень и канал от ближайшего существующего предка
    - После создания связь разрывается - изменения предков не влияют на потомков
*/

void logger_hierarchy_demo()
{
    std::cout << "=== Демонстрация иерархии логгеров ===" << std::endl;
    
    // 1. Настройка корневого логгера
    AutoPtr<ConsoleChannel> rootChannel(new ConsoleChannel);
    Logger::root().setChannel(rootChannel);
    Logger::root().setLevel(Message::PRIO_WARNING);
    
    // 2. Создание иерархии
    Logger& serverLogger = Logger::get("Server");
    Logger& networkLogger = Logger::get("Server.Network");
    Logger& httpLogger = Logger::get("Server.Network.HTTP");
    Logger& databaseLogger = Logger::get("Server.Database");
    
    // 3. Демонстрация наследования
    std::cout << "\n--- Наследование от корневого логгера ---" << std::endl;
    serverLogger.information("Это сообщение НЕ появится (уровень WARNING)"); // Не появится
    serverLogger.warning("Предупреждение сервера"); // Появится
    
    // 4. Изменение уровня для поддерева
    std::cout << "\n--- Изменение уровня для Server ---" << std::endl;
    serverLogger.setLevel(Message::PRIO_INFORMATION);
    
    // Создаем новых потомков после изменения уровня
    Logger& newHttpLogger = Logger::get("Server.Network.NewHTTP");
    Logger& cacheLogger = Logger::get("Server.Cache");
    
    serverLogger.information("Теперь информационные сообщения видны");
    newHttpLogger.information("Новый HTTP логгер наследует уровень Server");
    cacheLogger.information("Кэш логгер также наследует уровень");
    
    // 5. Разные каналы для разных компонентов
    std::cout << "\n--- Разные каналы для компонентов ---" << std::endl;
    AutoPtr<SimpleFileChannel> fileChannel(new SimpleFileChannel);
    fileChannel->setProperty("path", "database.log");
    databaseLogger.setChannel(fileChannel);
    databaseLogger.setLevel(Message::PRIO_DEBUG);
    
    databaseLogger.debug("Отладочное сообщение в файл");
    databaseLogger.information("Информация о запросе к БД");
}

// Пример реального веб-сервера
class WebServer {
private:
    Logger& _logger;
    Logger& _requestLogger;
    Logger& _authLogger;
    
public:
    WebServer() : 
        _logger(Logger::get("WebServer")),
        _requestLogger(Logger::get("WebServer.RequestHandler")),
        _authLogger(Logger::get("WebServer.Authentication"))
    {

    }
    
    void start()
    {
        _logger.information("Запуск веб-сервера");
        
        // Имитация обработки запросов
        handleRequest("/index.html", "192.168.1.1");
        handleRequest("/admin", "192.168.1.2");
        handleRequest("/api/data", "192.168.1.3");
    }
    
private:
    void handleRequest(const std::string& path, const std::string& clientIP)
    {
        _requestLogger.information("Запрос: " + path + " от " + clientIP);
        
        if (path == "/admin") {
            authenticateUser("admin", clientIP);
        }
        
        _requestLogger.information("Ответ для: " + path);
    }
    
    void authenticateUser(const std::string& user, const std::string& ip)
    {
        _authLogger.debug("Аутентификация пользователя: " + user);
        // Логика аутентификации...
        _authLogger.information("Успешная аутентификация: " + user + " с IP " + ip);
    }
};

int main()
{
    logger_hierarchy_demo();

    std::cout << "=== Демонстрация работы WebServer ===\n";

    Logger::get("WebServer").setLevel(Message::PRIO_INFORMATION);
    WebServer server;
    server.start();
}

