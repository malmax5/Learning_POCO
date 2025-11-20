#include "Poco/URI.h"
#include "Poco/Exception.h"
#include <iostream>
#include <vector>

void uri_parsing_demo()
{
    std::cout << "=== URI Parsing Demo ===" << std::endl;
    
    // Разбор различных типов URI
    std::vector<std::string> uris = {
        "https://user:pass@example.com:8080/path/to/resource?query=value&sort=asc#section1",
        "ftp://anonymous@ftp.example.com/pub/files/archive.zip",
        "mailto:user@example.com?subject=Hello&body=Test",
        "file:///home/user/document.txt",
        "http://appinf.com/poco/docs/Poco.URI.html#details"
    };
    
    for (const auto& uri_str : uris)
    {
        std::cout << "\nParsing: " << uri_str << std::endl;
        
        try
        {
            Poco::URI uri(uri_str);
            
            std::cout << "  Scheme: " << uri.getScheme() << std::endl;
            std::cout << "  Authority: " << uri.getAuthority() << std::endl;
            std::cout << "  Host: " << uri.getHost() << std::endl;
            std::cout << "  Port: " << uri.getPort() << std::endl;
            std::cout << "  Path: " << uri.getPath() << std::endl;
            std::cout << "  Query: " << uri.getQuery() << std::endl;
            std::cout << "  Fragment: " << uri.getFragment() << std::endl;
            std::cout << "  PathEtc: " << uri.getPathEtc() << std::endl;
            
            // Декодирование компонентов
            std::cout << "  Decoded Path: " << uri.getPath() << std::endl;
            
        }
        catch (const Poco::Exception& e)
        {
            std::cout << "  Error: " << e.displayText() << std::endl;
        }
    }
}

int main()
{
    uri_parsing_demo();
}
