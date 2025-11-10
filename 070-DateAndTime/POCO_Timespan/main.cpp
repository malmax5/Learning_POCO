#include "Poco/Timespan.h"
#include <iostream>

/*
    Константы для удобства
    Poco::Timespan::MILLISECONDS  - 1000
    Poco::Timespan::SECONDS       - 1000000  
    Poco::Timespan::MINUTES       - 60000000
    Poco::Timespan::HOURS         - 3600000000
    Poco::Timespan::DAYS          - 86400000000
*/

int main()
{
    // Создание и использование
    Poco::Timespan interval1(1, 11, 45, 22, 123433); // 1д 11ч 45м 22.123433с
    Poco::Timespan interval2(33 * Poco::Timespan::SECONDS); // 33 секунды
    Poco::Timespan interval3(2, 5000000); // 2 секунды 5 миллисекунд
    
    int days = interval1.days(); // 1
    int hours = interval1.hours(); // 11 (в пределах дня)
    int totalHours = interval1.totalHours(); // 35 (всего часов)
    int minutes = interval1.minutes(); // 45 (в пределах часа)
    int totalMinutes = interval1.totalMinutes(); // 2145 (всего минут)
    int seconds = interval1.seconds(); // 22 (в пределах минуты)
    int totalSeconds = interval1.totalSeconds(); // 128722 (всего секунд)

    long long milliseconds = interval1.milliseconds(); // 123 (в пределах миллисекунд)
    long long totalMilliseconds = interval1.totalMilliseconds(); // 128722123 (всего миллисекунд)
    long long microseconds = interval1.microseconds(); // 433 (в пределах микросекунд)
    long long totalMicroseconds = interval1.totalMicroseconds(); // 128722123433 (всего микросекунд)

    std::cout << "days: " << days << "\n";
    std::cout << "hours: " << hours << "\n";
    std::cout << "minutes: " << minutes << "\n";
    std::cout << "seconds: " << seconds << "\n";
    std::cout << "milliseconds: " << milliseconds << "\n";
    std::cout << "microseconds: " << microseconds << "\n";
    std::cout << "totalHours: " << totalHours << "\n";
    std::cout << "totalMinutes: " << totalMinutes << "\n";
    std::cout << "totalSeconds: " << totalSeconds << "\n";
    std::cout << "totalMilliseconds: " << totalMilliseconds << "\n";
    std::cout << "totalMicroseconds: " << totalMicroseconds << "\n";
}

