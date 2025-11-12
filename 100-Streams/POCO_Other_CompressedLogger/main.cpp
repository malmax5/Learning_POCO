#include "Poco/DeflatingStream.h"
#include "Poco/TeeStream.h"
#include "Poco/CountingStream.h"
#include "Poco/DateTimeFormatter.h"
#include "Poco/DateTime.h"
#include <fstream>
#include <iostream>
#include <sstream>

class CompressedLogger
{
public:
    CompressedLogger(const std::string& baseFilename)
    {
        // Основной файл лога
        _logFile.open(baseFilename + ".log");
        
        // Сжатая версия лога
        _compressedFile.open(baseFilename + ".log.gz", std::ios::binary);
        _compressedStream = new Poco::DeflatingOutputStream(_compressedFile, Poco::DeflatingStreamBuf::STREAM_GZIP);
        
        // Tee stream пишет в оба места
        _teeStream = new Poco::TeeOutputStream(_logFile);
        _teeStream->addStream(*_compressedStream);
    }
    
    ~CompressedLogger()
    {
        _compressedStream->close();
        _logFile.close();
        _compressedFile.close();
        delete _compressedStream;
        delete _teeStream;
    }
    
    void log(const std::string& message)
    {
        std::string timestamp = Poco::DateTimeFormatter::format(Poco::DateTime(), "%Y-%m-%d %H:%M:%S");
        *_teeStream << "[" << timestamp << "] " << message << "\n";
    }
    
private:
    std::ofstream _logFile;
    std::ofstream _compressedFile;
    Poco::DeflatingOutputStream* _compressedStream;
    Poco::TeeOutputStream* _teeStream;
};

void logging_example()
{
    std::cout << "=== Compressed Logging System ===" << "\n";
    
    CompressedLogger logger("application");
    
    for (int i = 0; i < 10; ++i)
    {
        logger.log("Application event #" + std::to_string(i));
        logger.log("User action performed at step " + std::to_string(i));
    }
    
    logger.log("Application shutdown completed");
    
    std::cout << "Logs written to application.log and application.log.gz" << "\n";
}

int main()
{
    logging_example();
}
