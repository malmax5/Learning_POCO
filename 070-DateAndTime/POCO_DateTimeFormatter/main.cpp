#include "Poco/DateTimeFormatter.h"
#include "Poco/DateTimeFormat.h"
#include <iostream>
#include <chrono>
#include <thread>

/*
    Предопределенные форматы
    Poco::DateTimeFormat::ISO8601_FORMAT    // "2005-01-01T12:00:00+01:00"
    Poco::DateTimeFormat::RFC1123_FORMAT    // "Sat, 1 Jan 2005 12:00:00 +0100"  
    Poco::DateTimeFormat::SORTABLE_FORMAT   // "2005-01-01 12:00:00"
    Poco::DateTimeFormat::ASCTIME_FORMAT    // "Sat Jan  1 12:00:00 2005"
*/

int main()
{
    Poco::DateTime dt(2024, 12, 31, 23, 59, 59);
    Poco::Timespan ts(1, 2, 3, 4, 567890);

    // Форматирование DateTime
    std::string custom = Poco::DateTimeFormatter::format(dt, "%Y-%m-%d %H:%M:%S");
    // "2024-12-31 23:59:59"

    std::string iso = Poco::DateTimeFormatter::format(dt, 
                      Poco::DateTimeFormat::ISO8601_FORMAT);
    
    // Форматирование Timespan
    std::string interval = Poco::DateTimeFormatter::format(ts, 
                           "%d days, %H hours, %M minutes, %S seconds");
    // "1 days, 02 hours, 03 minutes, 04 seconds"

    // Эффективное добавление к существующей строке
    std::string logEntry = "Event occurred at: ";
    Poco::DateTimeFormatter::append(logEntry, dt, 
                                    Poco::DateTimeFormat::SORTABLE_FORMAT);
    
    std::cout << "custom: " << custom << "\n";
    std::cout << "iso: " << iso << "\n";
    std::cout << "interval: " << interval << "\n";
    std::cout << "log entry: " << logEntry << "\n";
}

