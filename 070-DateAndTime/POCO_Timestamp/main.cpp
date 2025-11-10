#include "Poco/Timestamp.h"
#include "Poco/Timespan.h"
#include <iostream>
#include <chrono>
#include <thread>

// Хранит монотонное UTC-время с микросекундным разрешением

// Основные типы
// Poco::Timestamp::TimeVal     // 64-битное время в микросекундах
// Poco::Timestamp::UtcTimeVal  // 64-битное время в 100-наносекундах
// Poco::Timestamp::TimeDiff    // Разница между временными метками

int main()
{
    Poco::Timestamp now; // Текущее время
    Poco::Timestamp specific = Poco::Timestamp::fromEpochTime(1234567890);

    // Измерение времени выполнения
    Poco::Timestamp start;
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    Poco::Timestamp::TimeDiff elapsed = start.elapsed();

    // Арифметические операции
    Poco::Timestamp future = now + Poco::Timespan::DAYS;
    Poco::Timestamp past = now - Poco::Timespan::HOURS * 2;
    Poco::Timestamp::TimeDiff diff = future - past;

    /* Эпохи и системы отсчета времени
     * Unix Epoch: С 1 января 1970 года (секунды)
     * UTC Time: С 15 октября 1582 года (100-наносекунды)
     */

    Poco::Timestamp ts;
    std::time_t unixTime = ts.epochTime();              // Время Unix
    Poco::Timestamp::UtcTimeVal utcTime = ts.utcTime(); // UTC время

    std::cout << "Now: " << now.epochTime() << "\n";
    std::cout << "Specific time: " << specific.epochTime() << "\n";
    std::cout << "time diff 1 sec: " << elapsed << "\n";
    std::cout << "future 1 day: " << future.epochTime() << "\n";
    std::cout << "past 2 hours: " << past.epochTime() << "\n";
    std::cout << "diff bet future and past: " << diff << "\n";
}

