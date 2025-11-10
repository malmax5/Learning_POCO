#include "Poco/File.h"
#include <iostream>

int main()
{
    // Создание файлов и директорий
    Poco::File dir("/home/cbf/LearningPOCO/Simple/080-Files/POCO_File_Operations/PathToThisDirectory");
    // Создание одной директории (бросает исключение если родительской нет)
    bool created = dir.createDirectory();  // Возвращает true если создана

    // Рекурсивное создание директорий
    dir.createDirectories();  // Создает все промежуточные директории

    // Атомарное создание файла
    Poco::File newFile("/home/cbf/LearningPOCO/Simple/080-Files/POCO_File_Operations/PathToThisDirectory/file.txt");
    bool fileCreated = newFile.createFile();  // true если создан, false если уже существует


    // Копирование, перемещение, удаление
    Poco::File source("source.txt");
    Poco::File destination("backup.txt");

    // Копирование
    source.copyTo(destination.path());  // Копирует файл

    // Перемещение/переименование
    source.moveTo("renamed.txt");       // Копирует и удаляет оригинал
    source.renameTo("new_name.txt");    // Переименовывает файл

    // Удаление
    source.remove();                    // Удаляет файл

    // Рекурсивное удаление директории
    // Poco::File tempDir("/tmp/mydir");
    // tempDir.remove(true);               // true = рекурсивное удаление
}

