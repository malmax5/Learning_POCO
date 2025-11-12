// Практический пример - форматирование
#include "Poco/Logger.h"
#include "Poco/FormattingChannel.h"
#include "Poco/PatternFormatter.h"
#include "Poco/ConsoleChannel.h"
#include "Poco/FileChannel.h"
#include "Poco/AutoPtr.h"
#include <iostream>
#include <sstream>

using Poco::Logger;
using Poco::FormattingChannel;
using Poco::PatternFormatter;
using Poco::ConsoleChannel;
using Poco::FileChannel;
using Poco::AutoPtr;
using Poco::Message;

/*
    PatternFormatter использует printf-подобный синтаксис:
    - %s - источник (имя логгера)
    - %t - текст сообщения
    - %p - приоритет
    - %q - сокращенный приоритет
    - %P - ID процесса
    - %T - ID потока
    - %I - имя потока
    - %Y, %m, %d - год, месяц, день
    - %H, %M, %S - час, минута, секунда
    - %f - микросекунды
*/

void formatting_demo()
{
    std::cout << "=== Демонстрация форматирования ===" << std::endl;
    
    // 1. Простое форматирование для консоли
    AutoPtr<PatternFormatter> simpleFormatter(new PatternFormatter);
    simpleFormatter->setProperty("pattern", "[%p] %t");
    
    AutoPtr<ConsoleChannel> console(new ConsoleChannel);
    AutoPtr<FormattingChannel> simpleFormatting(
        new FormattingChannel(simpleFormatter, console));
    
    Logger::get("SimpleFormat").setChannel(simpleFormatting);
    Logger::get("SimpleFormat").information("Простое сообщение");
    
    // 2. Детальное форматирование для файла
    AutoPtr<PatternFormatter> detailedFormatter(new PatternFormatter);
    detailedFormatter->setProperty("pattern", 
        "%Y-%m-%d %H:%M:%S.%f [%q] %s (PID:%P TID:%T): %t");
    
    AutoPtr<FileChannel> fileChannel(new FileChannel);
    fileChannel->setProperty("path", "detailed.log");
    
    AutoPtr<FormattingChannel> detailedFormatting(
        new FormattingChannel(detailedFormatter, fileChannel));
    
    Logger::get("DetailedFormat").setChannel(detailedFormatting);
    Logger::get("DetailedFormat").setLevel(Message::PRIO_DEBUG);
    
    Logger::get("DetailedFormat").debug("Детальное отладочное сообщение");
    Logger::get("DetailedFormat").information("Информация с полным контекстом");
    
    // 3. JSON-подобное форматирование
    AutoPtr<PatternFormatter> jsonFormatter(new PatternFormatter);
    jsonFormatter->setProperty("pattern",
        "{\"timestamp\":\"%Y-%m-%dT%H:%M:%S.%fZ\","
        "\"level\":\"%p\","
        "\"logger\":\"%s\","
        "\"thread\":\"%T\","
        "\"message\":\"%t\"}");
    
    AutoPtr<FormattingChannel> jsonFormatting(
        new FormattingChannel(jsonFormatter, console));
    
    Logger::get("JSONLogger").setChannel(jsonFormatting);
    Logger::get("JSONLogger").information("Сообщение в JSON-формате");
}

class CustomFormatter : public Poco::Formatter
{
public:
    void format(const Message& msg, std::string& text) override
    {
        std::ostringstream oss;
        
        // Пользовательский формат
        oss << "🚀 [" << priorityToString(msg.getPriority()) << "] "
            << msg.getSource() << " | "
            << "Thread:" << msg.getTid() << " | "
            << msg.getText();
            
        text = oss.str();
    }
    
private:
    std::string priorityToString(int priority)
    {
        switch (priority)
        {
            case Message::PRIO_FATAL: return "💀 FATAL";
            case Message::PRIO_CRITICAL: return "🔥 CRITICAL";
            case Message::PRIO_ERROR: return "❌ ERROR";
            case Message::PRIO_WARNING: return "⚠️ WARN";
            case Message::PRIO_NOTICE: return "📢 NOTICE";
            case Message::PRIO_INFORMATION: return "ℹ️ INFO";
            case Message::PRIO_DEBUG: return "🐛 DEBUG";
            case Message::PRIO_TRACE: return "🔍 TRACE";
            default: return "❓ UNKNOWN";
        }
    }
};

void custom_formatter_demo()
{
    AutoPtr<CustomFormatter> customFormatter(new CustomFormatter);
    AutoPtr<ConsoleChannel> console(new ConsoleChannel);
    AutoPtr<FormattingChannel> customFormatting(
        new FormattingChannel(customFormatter, console));
    
    Logger::root().setChannel(customFormatting);
    
    Logger& logger = Logger::get("CustomFormat");
    
    // Сообщение без параметров
    logger.information("Запуск системы");
    
    // Сообщение с параметрами
    Message msg("CustomFormat", "Ошибка обработки данных", Message::PRIO_ERROR);
    msg.set("user_id", "123");
    msg.set("data_size", "1024");
    logger.log(msg);
}

int main()
{
    formatting_demo();
    custom_formatter_demo();
}

