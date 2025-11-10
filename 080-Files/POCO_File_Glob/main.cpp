#include "Poco/Glob.h"
#include "Poco/File.h"
#include <iostream>

/*
    Паттерны Glob
    * — любая последовательность символов
    ? — любой одиночный символ
    [abc] — любой символ из набора
    [a-z] — любой символ из диапазона
    [!abc] — любой символ не из набора
*/

/*
    Poco::Glob::Options
    GLOB_DEFAULT            - Поведение по умолчанию
    GLOB_DOT_SPECIAL        - «Точка особенная» — имитирует поведение оболочки Unix (bash и др.)
                            - Например, шаблон * не найдёт .profile, а ls * в bash тоже его не показывает.
    GLOB_FOLLOW_SYMLINKS    - Разыменовывать символические ссылки
                            - Если встречаются символические ссылки на каталоги, Glob зайдёт внутрь (следует по ссылке).
    GLOB_CASELESS           - Игнорировать регистр символов при сопоставлении
    GLOB_DIRS_ONLY          - Искать только каталоги.
*/

int main()
{
    // Поиск всех header файлов в поддиректориях с опцией
    std::set<std::string> headerFiles;
    Poco::Glob::glob("/home/cbf/LearningPOCO/Simple/080-Files/*/*.cpp", headerFiles, Poco::Glob::GLOB_DOT_SPECIAL);

    // Проверка соответствия шаблону
    Poco::Glob pattern("*.cpp");
    bool matches = pattern.match("main.cpp");  // true

    std::cout << "main.cpp match *.cpp: " << matches << "\n";
    std::cout << "header files /*/*.cpp in this parent directory:\n";
    for (auto& header : headerFiles)
    {
        std::cout << header << "\n";
    }
}

