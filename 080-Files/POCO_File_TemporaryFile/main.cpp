#include "Poco/TemporaryFile.h"
#include "Poco/File.h"
#include <iostream>
#include <fstream>

int main()
{
    // Файл автоматически удалится при разрушении объекта
    Poco::TemporaryFile temp;

    // Создание и использование файла
    std::ofstream stream(temp.path().c_str());
    stream << "Временные данные" << "\n";
    stream.close();

    // Не нужно явно удалять - деструктор сделает это автоматически
}

