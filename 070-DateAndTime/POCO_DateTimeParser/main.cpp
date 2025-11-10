#include "Poco/DateTimeParser.h"
#include "Poco/DateTimeFormat.h"

#include "Poco/Exception.h"
#include <iostream>
#include <chrono>
#include <thread>

int main()
{
    // Гибкий парсинг с поддержкой различных форматов
    // Парсинг с известным форматом
    std::string dateStr = "2024-12-31T23:59:59+03:00";
    int tzd;
    Poco::DateTime dt;

    bool success = Poco::DateTimeParser::tryParse(
        Poco::DateTimeFormat::ISO8601_FORMAT, dateStr, dt, tzd);

    // Автоматическое определение формата
    Poco::DateTime parsed = Poco::DateTimeParser::parse("Sat, 31 Dec 2024 23:59:59 GMT", tzd);

    // Парсинг с пользовательским форматом
    Poco::DateTimeParser::parse("%d.%m.%Y", "31.12.2024", dt, tzd);


    // Обработка ошибок
    std::string userInput = "invalid date";
    int timezoneOffset;
    Poco::DateTime result;

    if (Poco::DateTimeParser::tryParse(userInput, result, timezoneOffset))
    {
        // Успешный парсинг
    }
    else
    {
        // Обработка ошибки
    }

    // Парсинг с исключениями
    try
    {
        Poco::DateTime dt = Poco::DateTimeParser::parse(userInput, timezoneOffset);
    }
    catch (Poco::SyntaxException& ex)
    {
        // Обработка синтаксической ошибки
    }
}

