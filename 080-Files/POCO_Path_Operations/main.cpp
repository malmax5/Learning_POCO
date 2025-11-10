#include "Poco/Path.h"
#include <iostream>

int main()
{
    // Преобразование типов путей
    Poco::Path path("projects/poco");

    // Преобразование в абсолютный путь
    path.makeAbsolute();                         // "/current/dir/projects/poco"
    path.makeAbsolute("/opt");                   // "/opt/projects/poco"

    // Получение родительского каталога
    Poco::Path parent = path.parent();           // "projects/"

    // Преобразование между файлом и директорией
    path.makeDirectory();                        // Добавляет trailing separator
    path.makeFile();                             // Убирает trailing separator


    // Разрешение путей
    Poco::Path base("/usr/include");
    Poco::Path relative("stdio.h");
    
    base.resolve(relative);                      // "/usr/include/stdio.h"
    
    Poco::Path absolute("/opt/local/include");
    base.resolve(absolute);                      // "/opt/local/include" (заменяет)
}

