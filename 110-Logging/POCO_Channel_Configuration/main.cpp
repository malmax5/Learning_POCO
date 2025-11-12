// Практический пример - конфигурация каналов
#include "Poco/Logger.h"
#include "Poco/FileChannel.h"
#include "Poco/AsyncChannel.h"
#include "Poco/SplitterChannel.h"
#include "Poco/ConsoleChannel.h"
#include "Poco/FormattingChannel.h"
#include "Poco/PatternFormatter.h"
#include "Poco/AutoPtr.h"
#include <iostream>

using Poco::Logger;
using Poco::FileChannel;
using Poco::AsyncChannel;
using Poco::SplitterChannel;
using Poco::ConsoleChannel;
using Poco::FormattingChannel;
using Poco::PatternFormatter;
using Poco::AutoPtr;
using Poco::Message;

/*
    Типы каналов:
    - ConsoleChannel - вывод в std::clog
    - FileChannel - продвинутое файловое логирование
    - SimpleFileChannel - простое файловое логирование
    - AsyncChannel - асинхронная доставка
    - SplitterChannel - мультиплексирование
    - FormattingChannel - форматирование сообщений
*/

/*
    Свойства конфигурации:
    - Пути к файлам
    - Политики ротации
    - Стратегии архивирования
    - Параметры сжатия
*/

void advanced_channels_demo()
{
    std::cout << "=== Продвинутая конфигурация каналов ===" << std::endl;
    
    // 1. FileChannel с ротацией и архивированием
    AutoPtr<FileChannel> fileChannel(new FileChannel);
    fileChannel->setProperty("path", "application.log");
    fileChannel->setProperty("rotation", "2M");           // Ротация при 2 МБ
    fileChannel->setProperty("archive", "timestamp");      // Архивировать с временной меткой
    fileChannel->setProperty("compress", "true");          // Сжимать архивы
    fileChannel->setProperty("purgeCount", "5");           // Хранить 5 архивов
    fileChannel->setProperty("times", "local");            // Локальное время
    
    // 2. Форматирование для консоли
    AutoPtr<PatternFormatter> consoleFormatter(new PatternFormatter);
    consoleFormatter->setProperty("pattern", "%Y-%m-%d %H:%M:%S [%p] %s: %t");
    AutoPtr<ConsoleChannel> consoleChannel(new ConsoleChannel);
    AutoPtr<FormattingChannel> consoleFormatting(
        new FormattingChannel(consoleFormatter, consoleChannel));
    
    // 3. SplitterChannel для дублирования логов
    AutoPtr<SplitterChannel> splitter(new SplitterChannel);
    splitter->addChannel(consoleFormatting);
    splitter->addChannel(fileChannel);
    
    // 4. AsyncChannel для асинхронной доставки
    AutoPtr<AsyncChannel> asyncChannel(new AsyncChannel(splitter));
    
    // 5. Установка канала для корневого логгера
    Logger::root().setChannel(asyncChannel);
    Logger::root().setLevel(Message::PRIO_DEBUG);
    
    // Тестирование
    Logger& logger = Logger::get("AdvancedChannelDemo");
    
    for (int i = 0; i < 100; i++)
    {
        logger.information("Тестовое сообщение #" + std::to_string(i));
        
        if (i % 10 == 0) {
            logger.warning("Каждое 10-е сообщение - предупреждение");
        }
        
        if (i % 25 == 0) {
            logger.error("Каждое 25-е сообщение - ошибка");
        }
    }
    
    logger.debug("Завершение демонстрации каналов");
}

// Специализированная конфигурация для разных окружений
class LoggingConfigurator {
public:
    static void setupDevelopment()
    {
        AutoPtr<ConsoleChannel> console(new ConsoleChannel);
        AutoPtr<PatternFormatter> formatter(new PatternFormatter);
        formatter->setProperty("pattern", "%H:%M:%S [%p] %t");
        AutoPtr<FormattingChannel> formatting(
            new FormattingChannel(formatter, console));
            
        Logger::root().setChannel(formatting);
        Logger::root().setLevel(Message::PRIO_DEBUG);
    }
    
    static void setupProduction()
    {
        AutoPtr<FileChannel> fileChannel(new FileChannel);
        fileChannel->setProperty("path", "application2.log");
        fileChannel->setProperty("rotation", "100M");
        fileChannel->setProperty("archive", "number");
        fileChannel->setProperty("compress", "true");
        fileChannel->setProperty("purgeCount", "10");
        
        AutoPtr<AsyncChannel> asyncChannel(new AsyncChannel(fileChannel));
        Logger::root().setChannel(asyncChannel);
        Logger::root().setLevel(Message::PRIO_WARNING);
    }
    
    static void setupStaging()
    {
        // Комбинированная конфигурация
        AutoPtr<FileChannel> fileChannel(new FileChannel);
        fileChannel->setProperty("path", "staging.log");
        fileChannel->setProperty("rotation", "10 M");
        
        AutoPtr<ConsoleChannel> console(new ConsoleChannel);
        
        AutoPtr<SplitterChannel> splitter(new SplitterChannel);
        splitter->addChannel(console);
        splitter->addChannel(fileChannel);
        
        Logger::root().setChannel(splitter);
        Logger::root().setLevel(Message::PRIO_INFORMATION);
    }
};

int main()
{
    advanced_channels_demo();
}

