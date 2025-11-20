#include "Poco/URIStreamOpener.h"
#include "Poco/Net/HTTPStreamFactory.h"
#include "Poco/Net/FTPStreamFactory.h"
#include "Poco/StreamCopier.h"
#include "Poco/URI.h"
#include "Poco/Exception.h"
#include <iostream>
#include <sstream>

void uristream_opener_demo()
{
    std::cout << "\n=== URIStreamOpener Demo ===" << std::endl;
    
    try
    {
        // Регистрация фабрик для различных схем
        Poco::Net::HTTPStreamFactory::registerFactory();
        Poco::Net::FTPStreamFactory::registerFactory();
        // Фабрика для file:// регистрируется по умолчанию
        
        Poco::URIStreamOpener& opener = Poco::URIStreamOpener::defaultOpener();
        
        // Пример с локальным файлом
        std::cout << "Reading local file via URI..." << std::endl;
        std::istream* local_stream = opener.open("file:///etc/hosts"); // На Unix-системах
        // std::istream* local_stream = opener.open("file:///C:/Windows/System32/drivers/etc/hosts"); // На Windows
        
        if (local_stream)
        {
            std::stringstream ss;
            Poco::StreamCopier::copyStream(*local_stream, ss);
            std::cout << "File content (first 200 chars): " 
                     << ss.str().substr(0, 200) << "..." << std::endl;
            
            delete local_stream;
        }
        
        // Пример с HTTP
        std::cout << "\nReading HTTP resource..." << std::endl;
        std::istream* http_stream = opener.open("http://httpbin.org/json");
        if (http_stream)
        {
            std::stringstream ss;
            Poco::StreamCopier::copyStream(*http_stream, ss);
            std::cout << "HTTP response: " << ss.str() << std::endl;

            delete http_stream;
        }
        
        
    }
    catch (Poco::Exception& e)
    {
        std::cout << "Error: " << e.displayText() << std::endl;
    }
}

int main()
{
    uristream_opener_demo();
}
