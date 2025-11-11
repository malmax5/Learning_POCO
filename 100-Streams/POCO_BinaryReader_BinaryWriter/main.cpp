#include "Poco/BinaryReader.h"
#include "Poco/BinaryWriter.h"
#include "Poco/Types.h"
#include <fstream>
#include <iostream>
#include <vector>

void binary_data_examples()
{
    std::cout << "=== Binary Data Writing ===" << "\n";
    
    std::ofstream outFile("/home/cbf/LearningPOCO/Simple/100-Streams/POCO_BinaryReader_BinaryWriter/data.bin", std::ios::binary);
    Poco::BinaryWriter writer(outFile);
    
    // Запись BOM (Byte Order Mark)
    writer.writeBOM();
    
    // Запись различных типов данных
    writer << "Hello Binary";    // Строка
    writer << Poco::Int32(42);         // 32-битное целое
    writer << double(3.14159);   // Число с плавающей точкой
    writer << bool(true);        // Булево значение
    
    // Компактное кодирование чисел
    writer.write7BitEncoded(Poco::UInt32(12345));
    writer.write7BitEncoded(Poco::UInt32(127));    // Умещается в 1 байт
    writer.write7BitEncoded(Poco::UInt32(128));    // Требует 2 байта
    
    // Сырые данные
    std::vector<char> rawData = {'R', 'A', 'W', '!'};
    writer.writeRaw(std::string(rawData.begin(), rawData.end()));
    
    outFile.close();
    std::cout << "Binary data written to data.bin" << "\n";
    
    std::cout << "\n=== Binary Data Reading ===" << "\n";
    
    std::ifstream inFile("/home/cbf/LearningPOCO/Simple/100-Streams/POCO_BinaryReader_BinaryWriter/data.bin", std::ios::binary);
    Poco::BinaryReader reader(inFile);
    
    // Чтение BOM и автоматическое определение порядка байтов
    reader.readBOM();
    
    // Чтение данных
    std::string text;
    Poco::Int32 intValue;
    double doubleValue;
    bool boolValue;
    Poco::UInt32 encodedValue1, encodedValue2, encodedValue3;
    std::string rawDataRead;
    
    reader >> text >> intValue >> doubleValue >> boolValue;
    reader.read7BitEncoded(encodedValue1);
    reader.read7BitEncoded(encodedValue2);
    reader.read7BitEncoded(encodedValue3);
    reader.readRaw(4, rawDataRead);
    
    std::cout << "Text: " << text << "\n";
    std::cout << "Int32: " << intValue << "\n";
    std::cout << "Double: " << doubleValue << "\n";
    std::cout << "Bool: " << boolValue << "\n";
    std::cout << "7BitEncoded: " << encodedValue1 << ", " 
         << encodedValue2 << ", " << encodedValue3 << "\n";
    std::cout << "Raw Data: " << rawDataRead << "\n";
    
    inFile.close();
}

// Пример с разным порядком байтов
void byte_order_example()
{
    std::cout << "\n=== Byte Order Example ===" << "\n";
    
    // Запись в little-endian (x86 порядок)
    std::ofstream littleFile("/home/cbf/LearningPOCO/Simple/100-Streams/POCO_BinaryReader_BinaryWriter/little_endian.bin", std::ios::binary);
    Poco::BinaryWriter littleWriter(littleFile, Poco::BinaryWriter::LITTLE_ENDIAN_BYTE_ORDER);
    littleWriter << Poco::Int32(0x12345678);
    littleFile.close();
    
    // Запись в big-endian (сетевой порядок)
    std::ofstream bigFile("/home/cbf/LearningPOCO/Simple/100-Streams/POCO_BinaryReader_BinaryWriter/big_endian.bin", std::ios::binary);
    Poco::BinaryWriter bigWriter(bigFile, Poco::BinaryWriter::BIG_ENDIAN_BYTE_ORDER);
    bigWriter << Poco::Int32(0x12345678);
    bigFile.close();
    
    std::cout << "Data written in different byte orders for cross-platform compatibility" << "\n";
}

int main()
{
    binary_data_examples();
    byte_order_example();
}
