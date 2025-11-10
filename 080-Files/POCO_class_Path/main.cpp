#include "Poco/Path.h"
#include <iostream>

/*
    Стили путей
    Poco::Path::PATH_WINDOWS    // "C:\Windows\system32"
    Poco::Path::PATH_UNIX       // "/usr/local/bin"
    Poco::Path::PATH_VMS        // "NODE::DEVICE:[DIR.SUBDIR]FILE.EXT"
    Poco::Path::PATH_NATIVE     // Стиль текущей ОС
    Poco::Path::PATH_GUESS      // Автоопределение стиля
*/

int main()
{
    // Создание абсолютного пути
    Poco::Path absolutePath(true);
    absolutePath.setDevice("C");
    absolutePath.pushDirectory("Windows");
    absolutePath.pushDirectory("system32");
    absolutePath.setFileName("cmd.exe");

    // Создание относительного пути
    Poco::Path relativePath(false);
    relativePath.pushDirectory("projects");
    relativePath.pushDirectory("poco");
    relativePath.setFileName("main.cpp");
}

