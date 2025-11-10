#include "Poco/Path.h"
#include <iostream>

/*
    Конструкторы для комбинирования путей
    Poco::Path parent("/usr/local");
    Poco::Path file(parent, "include");        // "/usr/local/include"
    Poco::Path relative("lib", "shared");      // Комбинирование относительных путей
*/

int main()
{
    // Базовый парсинг
    Poco::Path path1("C:\\Windows\\system32\\cmd.exe");
    Poco::Path path2("/usr/local/include/stdio.h");

    // Явное указание стиля
    Poco::Path path3("DSK$PROJ:[POCO]BUILD.COM", Poco::Path::PATH_VMS);

    // Безопасный парсинг с проверкой
    Poco::Path path4;
    bool isValid = path4.tryParse("/usr/*/invalid", Poco::Path::Style::PATH_GUESS);

    std::cout << "is valid path: /usr/*/invalid : " << isValid << "\n";
}

