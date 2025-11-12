// Практический пример - управление уровнями
#include "Poco/Logger.h"
#include "Poco/ConsoleChannel.h"
#include "Poco/AutoPtr.h"
#include <iostream>

using Poco::Logger;
using Poco::ConsoleChannel;
using Poco::AutoPtr;
using Poco::Message;

/*
    Уровни приоритетов (по убыванию важности):
    - FATAL - критические ошибки, приводящие к остановке системы
    - CRITICAL - серьезные ошибки, нарушающие функциональность
    - ERROR - ошибки, которые можно обработать
    - WARNING - предупреждения о потенциальных проблемах
    - NOTICE - важные информационные сообщения
    - INFO - общая информация о работе приложения
    - DEBUG - отладочная информация
    - TRACE - детальная трассировка выполнения
*/

class Application {
private:
    Logger& _logger;
    
public:
    Application()
        : _logger(Logger::get("Application"))
    {

    }
    
    void initialize()
    {
        _logger.information("Инициализация приложения");
        
        // Проверка уровня перед сложными вычислениями
        if (_logger.debug())
        {
            std::string debugInfo = gatherDebugInfo();
            _logger.debug(debugInfo);
        }
    }
    
    void processRequest(const std::string& request)
    {
        _logger.information("Обработка запроса: " + request);
        
        try
        {
            // Имитация обработки
            if (request.empty())
            {
                _logger.warning("Пустой запрос");
                return;
            }
            
            // Сложная операция - логируем только на debug уровне
            if (_logger.trace())
            {
                _logger.trace("Начало сложной обработки");
                // Детальная трассировка...
                _logger.trace("Завершение сложной обработки");
            }
            
        }
        catch (const std::exception& e)
        {
            _logger.error("Ошибка обработки: " + std::string(e.what()));
        }
    }
    
    void shutdown()
    {
        _logger.information("Завершение работы приложения");
    }
    
private:
    std::string gatherDebugInfo()
    {
        // Дорогостоящая операция сбора отладочной информации
        return "Debug information...";
    }
};

void priority_demo()
{
    AutoPtr<ConsoleChannel> console(new ConsoleChannel);
    Logger::root().setChannel(console);
    
    // Настройка уровней для разных компонентов
    Logger::get("Application").setLevel(Message::PRIO_INFORMATION); // В зависимости какой уровень выставляем, то и будет выводиться
    Logger::get("Network").setLevel(Message::PRIO_WARNING);
    Logger::get("Database").setLevel(Message::PRIO_DEBUG);
    
    Application app;
    app.initialize();
    app.processRequest("test request");
    app.processRequest(""); // пустой запрос
    app.shutdown();
}

int main()
{
    priority_demo();
}

