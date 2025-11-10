#include "Poco/Path.h"
#include <iostream>

void printSystemPaths()
{
    std::cout << "Current: " << Poco::Path::current() << "\n";
    std::cout << "Home: " << Poco::Path::home() << "\n";
    std::cout << "Temp: " << Poco::Path::temp() << "\n";
    std::cout << "Null: " << Poco::Path::null() << "\n";
    
    // Пример вывода на Linux:
    // Current: /home/user/projects
    // Home: /home/user
    // Temp: /tmp
    // Null: /dev/null
}

int main()
{
    // Работа с переменными окружения
    // Расширение переменных окружения в путях
    std::string configPath1 = "$HOME/.config/myapp.conf";  // Unix
    std::string configPath2 = "%APPDATA%\\myapp\\config.ini"; // Windows

    std::string expanded1 = Poco::Path::expand(configPath1);
    std::string expanded2 = Poco::Path::expand(configPath2);
    // На Unix: "/home/user/.config/myapp.conf"
    // На Windows: "C:\Users\user\AppData\Roaming\myapp\config.ini"


    // Корневые файловые системы
    std::vector<std::string> roots;
    Poco::Path::listRoots(roots);
    // На Windows: ["A:", "C:", "D:", ...]
    // На Unix: ["/"]
    // На OpenVMS: ["DSK1", "DSK2", ...]
}

