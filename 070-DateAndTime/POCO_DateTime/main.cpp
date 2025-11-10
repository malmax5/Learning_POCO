#include "Poco/DateTime.h"
#include <iostream>

int main()
{
    // Создание и манипуляции
    Poco::DateTime now; // Текущее время UTC
    Poco::DateTime specific(2024, 12, 31, 23, 59, 59); // Конкретная дата
    // Poco::DateTime fromTs(someTimestamp); // Из временной метки

    int year = now.year();
    int month = now.month();  // 1-12
    int day = now.day();      // 1-31
    int hour = now.hour();    // 0-23
    int minute = now.minute();
    int second = now.second();
    int millisecond = now.millisecond();
    int microsecond = now.microsecond();

    int dayOfWeek = now.dayOfWeek();  // 0=воскресенье, 1=понедельник...
    int dayOfYear = now.dayOfYear();  // 1-366
    int weekNumber = now.week();      // Неделя по ISO 8601


    // Статические методы для проверок
    bool leap2000 = Poco::DateTime::isLeapYear(2000);  // true
    bool leap2001 = Poco::DateTime::isLeapYear(2001);  // false

    // Количество дней в месяце
    int daysFeb2000 = Poco::DateTime::daysOfMonth(2000, 2);  // 29
    int daysFeb2001 = Poco::DateTime::daysOfMonth(2001, 2);  // 28

    // Валидация даты
    bool valid = Poco::DateTime::isValid(2024, 2, 29);  // true (високосный)
    bool invalid = Poco::DateTime::isValid(2023, 2, 29); // false

    std::cout << "Now: " << now.timestamp().epochTime() << "\n";
    std::cout << "year: " << year << "\n";
    std::cout << "month: " << month << "\n";
    std::cout << "day: " << day << "\n";
    std::cout << "hour: " << hour << "\n";
    std::cout << "minute: " << minute << "\n";
    std::cout << "second: " << second << "\n";
    std::cout << "millisecond: " << millisecond << "\n";
    std::cout << "microsecond: " << microsecond << "\n";
    std::cout << "day of week: " << dayOfWeek << "\n";
    std::cout << "day of year: " << dayOfYear << "\n";
    std::cout << "week number: " << weekNumber << "\n";
    std::cout << "is leap: " << Poco::DateTime::isLeapYear(year) << "\n";
    std::cout << "days in this month: " << Poco::DateTime::daysOfMonth(year, month) << "\n";
    std::cout << "is valid: " << Poco::DateTime::isValid(year, month, day) << "\n";
}

