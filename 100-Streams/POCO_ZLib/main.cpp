#include "Poco/DeflatingStream.h"
#include "Poco/InflatingStream.h"
#include "Poco/StreamCopier.h"
#include <sstream>
#include <iostream>
#include <fstream>

void compression_examples()
{
    std::cout << "=== ZLib Compression ===" << "\n";
    
    // Исходные данные
    std::string originalData = "This is a test string that will be compressed using ZLib compression. "
                               "Repeating this text to make it longer for better compression demo. "
                               "This is a test string that will be compressed using ZLib compression.";
    
    std::cout << "Original size: " << originalData.size() << " bytes" << "\n";
    std::cout << "Original: " << originalData.substr(0, 50) << "..." << "\n";
    
    // Сжатие
    std::stringstream compressedStream;
    Poco::DeflatingOutputStream deflater(compressedStream, Poco::DeflatingStreamBuf::STREAM_ZLIB);
    deflater << originalData;
    deflater.close();
    
    std::string compressedData = compressedStream.str();
    std::cout << "Compressed size: " << compressedData.size() << " bytes" << "\n";
    std::cout << "Compression ratio: " << (1.0 - (double)compressedData.size() / originalData.size()) * 100 << "%" << "\n";
    
    // Распаковка
    std::stringstream decompressedStream;
    std::stringstream compressedInput(compressedData);
    Poco::InflatingInputStream inflater(compressedInput, Poco::InflatingStreamBuf::STREAM_ZLIB);
    
    Poco::StreamCopier::copyStream(inflater, decompressedStream);
    
    std::string decompressedData = decompressedStream.str();
    std::cout << "Decompressed size: " << decompressedData.size() << " bytes" << "\n";
    std::cout << "Data matches: " << (originalData == decompressedData ? "YES" : "NO") << "\n";
}

// Пример с файлом
void file_compression_example()
{
    std::cout << "\n=== File Compression (GZIP) ===" << "\n";
    
    // Запись сжатых данных в файл
    std::ofstream outputFile("/home/cbf/LearningPOCO/Simple/100-Streams/POCO_ZLib/compressed.txt.gz", std::ios::binary);
    Poco::DeflatingOutputStream gzipWriter(outputFile, Poco::DeflatingStreamBuf::STREAM_GZIP);
    
    gzipWriter << "This data will be compressed and written to file.\n";
    gzipWriter << "Multiple lines of text.\n";
    gzipWriter << "End of compressed data.";
    
    gzipWriter.close();
    outputFile.close();
    
    std::cout << "Compressed data written to compressed.txt.gz" << "\n";
    
    // Чтение и распаковка
    std::ifstream inputFile("compressed.txt.gz", std::ios::binary);
    Poco::InflatingInputStream gzipReader(inputFile, Poco::InflatingStreamBuf::STREAM_GZIP);
    
    std::stringstream decompressed;
    Poco::StreamCopier::copyStream(gzipReader, decompressed);
    
    std::cout << "Decompressed content:\n" << decompressed.str() << "\n";
}

int main()
{
    compression_examples();
    file_compression_example();
}
