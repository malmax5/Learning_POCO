// Практический пример - оптимизация
#include "Poco/Logger.h"
#include "Poco/AsyncChannel.h"
#include "Poco/ConsoleChannel.h"
#include "Poco/AutoPtr.h"
#include <iostream>
#include <chrono>
#include <sstream>
#include <thread>

using Poco::Logger;
using Poco::AsyncChannel;
using Poco::ConsoleChannel;
using Poco::AutoPtr;
using Poco::Message;

/*
    Факторы влияния на производительность:
    - Создание Message: время, PID, TID, имя потока
    - Форматирование строк: конкатенация, преобразование типов
    - Поиск логгера: O(log n) для std::map
    - Доставка через каналы: синхронная vs асинхронная
*/

/*
    Оптимизационные стратегии:
    - Кэширование ссылок на логгеры
    - Проверка уровня перед созданием сообщения
    - Использование макросов для условной компиляции
    - Асинхронная доставка
*/

class OptimizedService
{
private:
    Logger& _logger;
    Logger& _perfLogger;
    Logger& _dataLogger;
    
public:
    OptimizedService() : 
        _logger(Logger::get("Service")),           // Кэшируем при создании
        _perfLogger(Logger::get("Service.Perf")),
        _dataLogger(Logger::get("Service.Data"))
    {

    }
    
    void processRequest(const std::string& request)
    {
        auto start = std::chrono::high_resolution_clock::now();
        
        // Проверяем уровень ДО дорогостоящих операций
        if (_logger.information())
        {
            _logger.information("Обработка запроса: " + request);
        }
        
        // Дорогостоящая операция - логируем только если нужно
        if (_dataLogger.debug())
        {
            std::string debugInfo = generateDebugInfo(request);
            _dataLogger.debug(debugInfo);
        }
        
        // Имитация обработки
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
        
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
        
        // Логирование производительности
        if (_perfLogger.information())
        {
            _perfLogger.information("Время обработки: " + 
                                   std::to_string(duration.count()) + " мкс");
        }
    }
    
private:
    std::string generateDebugInfo(const std::string& request)
    {
        // Дорогостоящая операция генерации отладочной информации
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
        
        std::ostringstream oss;
        oss << "Детальная информация для: " << request 
            << " (длина: " << request.length() << ")";
        return oss.str();
    }
};

void performance_comparison()
{
    std::cout << "=== Сравнение производительности ===" << std::endl;
    // Настройка асинхронного канала для производительности
    AutoPtr<ConsoleChannel> console(new ConsoleChannel);
    AutoPtr<AsyncChannel> asyncChannel(new AsyncChannel(console));
    Logger::root().setChannel(asyncChannel);
    
    OptimizedService service;
    
    // Тест производительности
    auto start = std::chrono::high_resolution_clock::now();
    
    for (int i = 0; i < 10; i++)
    {
        service.processRequest("request_" + std::to_string(i));
    }
    
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    
    std::cout << "Время выполнения 10 запросов: " 
              << duration.count() << " мс" << std::endl;
}

// Макросы для условного логирования
#define LOG_TRACE(logger, msg) \
    do { if (logger.trace()) logger.trace(msg); } while(0)

#define LOG_DEBUG(logger, msg) \
    do { if (logger.debug()) logger.debug(msg); } while(0)

#define LOG_INFO(logger, msg) \
    do { if (logger.information()) logger.information(msg); } while(0)

void macro_demo()
{
    Logger::root().setLevel(Message::PRIO_DEBUG);
    Logger& logger = Logger::get("MacroDemo");
    
    // Использование макросов
    LOG_TRACE(logger, "Трассировка выполнения");
    LOG_DEBUG(logger, "Отладочная информация");
    LOG_INFO(logger, "Информационное сообщение");
    
    // Сравнение с обычным подходом
    if (logger.debug())
    {
        logger.debug("Традиционная проверка уровня");
    }
}

int main()
{
    performance_comparison();
    macro_demo();
}

