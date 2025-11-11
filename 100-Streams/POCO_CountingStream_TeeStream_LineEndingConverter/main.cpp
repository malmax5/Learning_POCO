#include "Poco/CountingStream.h"
#include "Poco/TeeStream.h"
#include "Poco/LineEndingConverter.h"
#include "Poco/NullStream.h"
#include <iostream>
#include <fstream>
#include <sstream>

/*
    CountingStream:
    Отслеживает: количество символов, строк, позицию в строке
    Использует конечный автомат для определения границ строк

    LineEndingConverter:
    Решает проблему кросс-платформенных переносов строк:
    Unix/Linux: \n (LF)
    Windows: \r\n (CRLF)
    Classic Mac: \r (CR)

    TeeStream: Реализует паттерн "Наблюдатель" для потоков вывода

    NullStream: Паттерн "Null Object" для потоков
*/

void utility_streams_examples()
{
    std::cout << "=== Counting Stream ===" << "\n";
    
    std::stringstream dataStream;
    dataStream << "Line 1\nLine 2\nLine 3\n";
    
    Poco::CountingInputStream counter(dataStream);
    
    std::string line;
    int lineCount = 0;
    while (std::getline(counter, line)) {
        lineCount++;
        std::cout << "Line " << lineCount << ": " << line << "\n";
        std::cout << "  Characters: " << counter.chars() 
                  << ", Lines: " << counter.lines() 
                  << ", Pos: " << counter.pos() << "\n";
    }
    
    std::cout << "\n=== Tee Stream ===" << "\n";
    
    Poco::TeeOutputStream tee(std::cout);  // Основной выход - консоль
    
    std::ofstream file1("/home/cbf/LearningPOCO/Simple/100-Streams/POCO_CountingStream_TeeStream_LineEndingConverter/tee_output1.txt");
    std::ofstream file2("/home/cbf/LearningPOCO/Simple/100-Streams/POCO_CountingStream_TeeStream_LineEndingConverter/tee_output2.txt");
    
    tee.addStream(file1);
    tee.addStream(file2);
    
    tee << "This goes to console and both files!\n";
    tee << "Multiple streams with one write operation.\n";
    
    file1.close();
    file2.close();
    std::cout << "Data written to tee_output1.txt and tee_output2.txt" << "\n";
    
    std::cout << "\n=== Line Ending Conversion ===" << "\n";
    
    std::stringstream unixStyle;
    unixStyle << "Unix\nLine\nEndings\n";
    
    std::stringstream windowsStyle;
    Poco::OutputLineEndingConverter converter(windowsStyle, Poco::LineEnding::NEWLINE_CRLF);
    converter << unixStyle.str();
    converter.clear();
    
    std::cout << "Unix style (LF): " << unixStyle.str();
    std::cout << "Windows style (CRLF): " << windowsStyle.str();
    
    std::cout << "\n=== Null Stream ===" << "\n";
    
    Poco::NullOutputStream nullStream;
    nullStream << "This data will be discarded and not stored anywhere!";
    
    std::cout << "Data successfully discarded using NullOutputStream" << "\n";
}

int main()
{
    utility_streams_examples();
}

