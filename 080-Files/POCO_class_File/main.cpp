#include "Poco/File.h"
#include <iostream>

int main()
{
    Poco::File file("/home/cbf/LearningPOCO/Simple/080-Files/POCO_class_File/pathToThisFile.txt");

    // Проверка атрибутов файла
    // Существование и тип
    bool exists = file.exists();
    bool isFile = file.isFile();
    bool isDir = file.isDirectory();
    bool isLink = file.isLink();
    bool isHidden = file.isHidden();

    // Права доступа
    bool readable = file.canRead();
    bool writable = file.canWrite();
    bool executable = file.canExecute();

    // Метаданные
    Poco::Timestamp created = file.created();
    Poco::Timestamp modified = file.getLastModified();
    Poco::File::FileSize size = file.getSize();  // 64-битный беззнаковый

    std::cout << "This file path: " << file.path() << "\n";
    std::cout << "exists: " << exists << "\n";
    std::cout << "isFile: " << isFile << "\n";
    std::cout << "isDir: " << isDir << "\n";
    std::cout << "isLink: " << isLink << "\n";
    std::cout << "isHidden: " << isHidden << "\n";
    std::cout << "readable: " << readable << "\n";
    std::cout << "writable: " << writable << "\n";
    std::cout << "executable: " << executable << "\n";
    std::cout << "created: " << created.elapsed() << "\n";
    std::cout << "modified: " << modified.elapsed() << "\n";
    std::cout << "size: " << size << "\n";


    // Изменение атрибутов
    // Изменение времени модификации
    file.setLastModified(Poco::Timestamp());

    // Изменение размера (усечение файла)
    file.setSize(0);  // Обнулить файл

    // Управление правами
    file.setReadOnly(true);   // Только для чтения
    file.setWriteable(false);  // Альтернативный способ
}

