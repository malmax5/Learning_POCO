#include "Poco/File.h"
#include "Poco/DirectoryIterator.h"
#include <iostream>

int main()
{
    // Простое получение списка файлов
    Poco::File dir("/home/cbf/LearningPOCO/Simple/080-Files/POCO_File_Read/PathToThisDirectory");
    std::vector<std::string> files;
    dir.list(files);  // Заполняет вектор именами файлов и поддиректорий

    for (const auto& fileName : files)
    {
        std::cout << fileName << "\n";
    }


    // Использование DirectoryIterator
    Poco::Path path = "/home/cbf/LearningPOCO/Simple/080-Files/POCO_File_Read/PathToThisDirectory";
    Poco::DirectoryIterator it(path);
    Poco::DirectoryIterator end;
    while (it != end)
    {
        std::cout << it.name();
        
        if (it->isFile())
        {
            std::cout << " (" << it->getSize() << " bytes)";
        }
        else if (it->isDirectory())
        {
            std::cout << " [DIR]";
        }
        else if (it->isLink())
        {
            std::cout << " [LINK]";
        }
        
        std::cout << "\n";
        it++;
    }
}

