#include "Poco/LocalDateTime.h"
#include <iostream>

int main()
{
    // Создание и использование
    Poco::LocalDateTime localNow;  // Текущее локальное время

    // С явным указанием смещения (в секундах)
    Poco::LocalDateTime moscowTime(3 * 3600, 2024, 12, 31, 23, 59, 0, 0, 0); // +3 hours = 180 minutes // UTC+3

    // Получение смещения
    int timezoneOffset = localNow.tzd();

    // Конвертация в UTC
    Poco::DateTime utcTime = localNow.utc();


    // Сравнение с учетом часовых поясов
    Poco::LocalDateTime nyTime(-5 * 3600, 2024, 1, 1, 0, 0, 0, 0, 0);  // Нью-Йорк UTC-5
    Poco::LocalDateTime londonTime(0, 2024, 1, 1, 5, 0, 0, 0, 0);      // Лондон UTC+0

    // Сравнение автоматически конвертирует в UTC
    if (nyTime == londonTime)
    {
        // Будет true, так как оба времени представляют один момент
    }

    std::cout << "timezone offset: " << timezoneOffset << "\n";
    std::cout << "NY time UTC-5 is same London UTC+0: " << (nyTime == londonTime) << "\n";
}

