// Комплексный пример реального приложения
#include "Poco/Logger.h"
#include "Poco/FormattingChannel.h"
#include "Poco/PatternFormatter.h"
#include "Poco/FileChannel.h"
#include "Poco/AsyncChannel.h"
#include "Poco/SplitterChannel.h"
#include "Poco/ConsoleChannel.h"
#include "Poco/AutoPtr.h"
#include <iostream>
#include <thread>
#include <vector>

using Poco::Logger;
using Poco::FormattingChannel;
using Poco::PatternFormatter;
using Poco::FileChannel;
using Poco::AsyncChannel;
using Poco::SplitterChannel;
using Poco::ConsoleChannel;
using Poco::AutoPtr;
using Poco::Message;

class WebApplication {
private:
    Logger& _appLogger;
    Logger& _accessLogger;
    Logger& _securityLogger;
    Logger& _databaseLogger;
    
public:
    WebApplication() : 
        _appLogger(Logger::get("WebApp")),
        _accessLogger(Logger::get("WebApp.Access")),
        _securityLogger(Logger::get("WebApp.Security")),
        _databaseLogger(Logger::get("WebApp.Database"))
    {

    }
    
    void start()
    {
        _appLogger.information("Запуск веб-приложения");
        
        // Имитация работы приложения
        simulateUserSessions();
        
        _appLogger.information("Остановка веб-приложения");
    }
    
private:
    void simulateUserSessions()
    {
        std::vector<std::thread> threads;
        
        for (int i = 0; i < 5; i++)
        {
            threads.emplace_back([this, i]()
            {
                handleUserSession("user_" + std::to_string(i));
            });
        }
        
        for (auto& thread : threads)
        {
            thread.join();
        }
    }
    
    void handleUserSession(const std::string& username)
    {
        _accessLogger.information("Начало сессии для: " + username);
        
        // Аутентификация
        if (!authenticateUser(username))
        {
            _securityLogger.warning("Неудачная аутентификация: " + username);
            return;
        }
        
        _securityLogger.information("Успешная аутентификация: " + username);
        
        // Обработка запросов
        for (int i = 0; i < 3; i++)
        {
            handleUserRequest(username, "/api/data/" + std::to_string(i));
        }
        
        _accessLogger.information("Завершение сессии для: " + username);
    }
    
    bool authenticateUser(const std::string& username)
    {
        _databaseLogger.debug("Проверка пользователя в БД: " + username);
        
        // Имитация проверки в БД
        std::this_thread::sleep_for(std::chrono::milliseconds(5));
        
        return username != "user_0"; // user_0 всегда неудачная аутентификация
    }
    
    void handleUserRequest(const std::string& username, const std::string& endpoint)
    {
        _accessLogger.information("Запрос от " + username + " к " + endpoint);
        
        try
        {
            _databaseLogger.debug("Запрос к базе данных для: " + username);
            
            // Имитация работы с БД
            std::this_thread::sleep_for(std::chrono::milliseconds(2));
            
            if (endpoint.find("admin") != std::string::npos)
            {
                _securityLogger.warning("Попытка доступа к админке: " + username);
            }
            
            _accessLogger.information("Успешный ответ для: " + username);
            
        }
        catch (const std::exception& e)
        {
            _appLogger.error("Ошибка обработки запроса: " + std::string(e.what()));
        }
    }
};

void setupProductionLogging()
{
    // Форматтер для детальных логов
    AutoPtr<PatternFormatter> detailedFormatter(new PatternFormatter);
    detailedFormatter->setProperty("pattern",
        "%Y-%m-%d %H:%M:%S.%f [%p] %s (%T): %t");
    
    // FileChannel для основных логов
    AutoPtr<FileChannel> mainFileChannel(new FileChannel);
    mainFileChannel->setProperty("path", "webapp.log");
    mainFileChannel->setProperty("rotation", "50M");
    mainFileChannel->setProperty("archive", "timestamp");
    mainFileChannel->setProperty("compress", "true");
    
    AutoPtr<FormattingChannel> mainFormatting(
        new FormattingChannel(detailedFormatter, mainFileChannel));
    
    // FileChannel для логов доступа
    AutoPtr<FileChannel> accessFileChannel(new FileChannel);
    accessFileChannel->setProperty("path", "access.log");
    accessFileChannel->setProperty("rotation", "100M");
    
    AutoPtr<PatternFormatter> accessFormatter(new PatternFormatter);
    accessFormatter->setProperty("pattern", "%Y-%m-%d %H:%M:%S %s: %t");
    
    AutoPtr<FormattingChannel> accessFormatting(
        new FormattingChannel(accessFormatter, accessFileChannel));

    // FileChannel для логов базы данных
    AutoPtr<FileChannel> databaseFileChannel(new FileChannel);
    databaseFileChannel->setProperty("path", "database.log");
    databaseFileChannel->setProperty("rotation", "100M");

    AutoPtr<PatternFormatter> databaseFormatter(new PatternFormatter);
    databaseFormatter->setProperty("pattern", "%Y-%m-%d %H:%M:%S %s: %t");

    AutoPtr<FormattingChannel> databaseFormatting(
        new FormattingChannel(databaseFormatter, databaseFileChannel));
    
    // Splitter для корневого логгера
    AutoPtr<SplitterChannel> splitter(new SplitterChannel);
    splitter->addChannel(mainFormatting);
    
    // AsyncChannel для производительности
    AutoPtr<AsyncChannel> asyncChannel(new AsyncChannel(splitter));
    
    Logger::root().setChannel(asyncChannel);
    Logger::root().setLevel(Message::PRIO_INFORMATION);
    
    // Отдельный канал для логов доступа
    Logger::get("WebApp.Access").setChannel(accessFormatting);
    Logger::get("WebApp.Access").setLevel(Message::PRIO_INFORMATION);
    
    // Более строгий уровень для безопасности
    Logger::get("WebApp.Security").setLevel(Message::PRIO_WARNING);

    // Уровень Debug для базы данных
    Logger::get("WebApp.Database").setChannel(databaseFormatting);
    Logger::get("WebApp.Database").setLevel(Message::PRIO_DEBUG);
}

int main()
{
    // Настройка логирования для production
    setupProductionLogging();
    
    // Запуск приложения
    WebApplication app;
    app.start();
    
    return 0;
}
