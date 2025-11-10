#include "Poco/TemporaryFile.h"
#include "Poco/File.h"
#include <iostream>
#include <fstream>

int main()
{
    // Автоматическое управление временными файлами
    // Файл автоматически удалится при разрушении объекта
    Poco::TemporaryFile temp;

    // Создание и использование файла
    std::ofstream stream(temp.path().c_str());
    stream << "Временные данные" << "\n";
    stream.close();

    // Не нужно явно удалять - деструктор сделает это автоматически


    // Управление временем жизни временных файлов
    // Отключить автоматическое удаление
    temp.keep();  // Файл не будет удален при разрушении объекта

    // Удалить при завершении программы
    temp.keepUntilExit();  // Удалится при нормальном завершении программы

    // Регистрация произвольного файла для удаления при выходе
    Poco::TemporaryFile::registerForDeletion("/path/to/file.to.delete");
}

