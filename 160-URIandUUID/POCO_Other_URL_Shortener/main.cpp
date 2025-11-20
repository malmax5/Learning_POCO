#include "Poco/UUID.h"
#include "Poco/UUIDGenerator.h"
#include "Poco/URI.h"
#include <iostream>
#include <map>

void url_shortener_demo()
{
    std::cout << "\n=== URL Shortener Demo ===" << std::endl;
    
    Poco::UUIDGenerator& generator = Poco::UUIDGenerator::defaultGenerator();
    std::map<std::string, std::string> url_database; // short_id -> original_url
    
    // Исходные длинные URL
    std::vector<std::string> long_urls =
    {
        "https://example.com/products/category/subcategory/item?id=12345&ref=homepage",
        "https://docs.pocoproject.org/current/Poco.URI.html#Poco.URI",
        "https://github.com/pocoproject/poco/blob/master/Foundation/include/Poco/UUID.h"
    };
    
    std::cout << "Generating short URLs..." << std::endl;
    
    for (const auto& long_url : long_urls)
    {
        // Создаем UUID на основе URL (детерминированный)
        Poco::UUID uuid = generator.createFromName(Poco::UUID::uri(), long_url);
        
        // Берем первые 8 символов для короткой ссылки
        std::string short_id = uuid.toString().substr(0, 8);
        
        // Сохраняем в "базу данных"
        url_database[short_id] = long_url;
        
        // Создаем короткий URI
        Poco::URI short_uri;
        short_uri.setScheme("https");
        short_uri.setHost("short.example.com");
        short_uri.setPath("/" + short_id);
        
        std::cout << "  " << long_url << std::endl;
        std::cout << "  → " << short_uri.toString() << std::endl;
    }
    
    // Демонстрация восстановления оригинального URL
    std::cout << "\nURL Resolution:" << std::endl;
    for (const auto& pair : url_database)
    {
        std::cout << "  Short ID: " << pair.first << " → " << pair.second << std::endl;
    }
}

int main()
{
    url_shortener_demo();
}
