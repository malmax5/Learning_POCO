// Практический пример: базовая архитектура
#include "Poco/Logger.h"
#include "Poco/Message.h"
#include "Poco/ConsoleChannel.h"
#include "Poco/AutoPtr.h"

using Poco::Logger;
using Poco::Message;
using Poco::ConsoleChannel;
using Poco::AutoPtr;

/*
    Message - контейнер для данных лога
    Logger - точка входа, фильтрация по приоритету
    Channel - доставка сообщений к месту назначения
*/

/*
    A message has
    - a priority
    - a source
    - a text
    - a timestamp
    - a process and thread identifier
    - optional parameters (name-value pairs)
*/

void basic_architecture_demo()
{
    // 1. Создаем канал (Channel)
    AutoPtr<ConsoleChannel> consoleChannel(new ConsoleChannel);
    
    // 2. Настраиваем корневой логгер (Logger)
    Logger::root().setChannel(consoleChannel);
    Logger::root().setLevel(Message::PRIO_INFORMATION);
    
    // 3. Получаем специализированный логгер
    Logger& logger = Logger::get("NetworkModule");
    
    // 4. Создаем и отправляем сообщения (Message)
    logger.information("Сетевое соединение установлено");
    logger.warning("Высокая задержка в сети");
    logger.error("Разрыв соединения");
}

// Демонстрация ручного создания Message
void manual_message_demo()
{
    Logger& logger = Logger::get("ManualLogger");
    
    // Создаем сообщение вручную
    Message msg (
        "ManualLogger",          // источник
        "Пользовательские данные", // текст
        Message::PRIO_INFORMATION  // приоритет
    );
    
    // Добавляем дополнительные параметры
    msg.set("user_id", "12345");
    msg.set("session_id", "abcde");
    
    logger.log(msg);
}

int main()
{
    basic_architecture_demo();
    manual_message_demo();
}

