#include "Poco/Timezone.h"
#include <iostream>

/*
    DST — Daylight Saving Time (летнее время)
    Это дополнительное смещение, которое добавляется к стандартному времени в тёплые месяцы, чтобы лучше использовать дневной свет.
    Например, в Москве раньше действовало летнее время: зимой — UTC+3, летом — UTC+4 (до 2011 года).

    TZD — Time Zone Differential (разность часового пояса)
    Это общее смещение текущего локального времени относительно UTC, включая DST, если оно действует.
*/

int main()
{
    // Основная информация
    int utcOffset = Poco::Timezone::utcOffset();  // Смещение без DST
    int dstOffset = Poco::Timezone::dst();        // Смещение DST
    int totalOffset = Poco::Timezone::tzd();      // Общее смещение

    // Проверка летнего времени
    bool isDstNow = Poco::Timezone::isDst(Poco::Timestamp());
    
    // Названия часовых поясов
    std::string currentName = Poco::Timezone::name();
    std::string standardName = Poco::Timezone::standardName();
    std::string dstName = Poco::Timezone::dstName();

    std::cout << "utc offset: " << utcOffset << "\n";
    std::cout << "dst offset: " << dstOffset << "\n";
    std::cout << "total offset: " << totalOffset << "\n";
    std::cout << "is dst now: " << isDstNow << "\n";
    std::cout << "current name: " << currentName << "\n";
    std::cout << "standard name: " << standardName << "\n";
    std::cout << "dst name: " << dstName << "\n";
}

