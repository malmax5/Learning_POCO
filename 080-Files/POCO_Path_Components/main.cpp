#include "Poco/Path.h"
#include <iostream>

int main()
{
    // Извлечение частей пути
    Poco::Path path("C:\\Projects\\Poco\\build_vs80.cmd", Poco::Path::PATH_WINDOWS);
    std::string device = path.getDevice();        // "C"
    int depth = path.depth();                     // 2
    std::string dir1 = path.directory(0);         // "Projects"
    std::string dir2 = path[1];                   // "Poco" (через operator[])
    std::string fileName = path.getFileName();    // "build_vs80.cmd"

    // Работа с именем файла
    std::string baseName = path.getBaseName();    // "build_vs80"
    std::string extension = path.getExtension();  // "cmd"

    std::cout << "path: " << path.toString() << "\n";
    std::cout << "device: " << device << "\n";
    std::cout << "depth: " << depth << "\n";
    std::cout << "dir1: " << dir1 << "\n";
    std::cout << "dir2: " << dir2 << "\n";
    std::cout << "fileName: " << fileName << "\n";
    std::cout << "baseName: " << baseName << "\n";
    std::cout << "extension: " << extension << "\n";

    // Изменение компонентов
    path.setBaseName("build_vs71");               // "build_vs71.cmd"
    path.setExtension("bat");                     // "build_vs71.bat"

    std::cout << "baseName after changing: " << path.getBaseName() << "\n";
    std::cout << "extension after changing: " << path.getExtension() << "\n";
}

