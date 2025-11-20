#include "Poco/URI.h"
#include "Poco/Exception.h"
#include <iostream>

void uri_construction_demo()
{
    std::cout << "\n=== URI Construction Demo ===" << std::endl;
    
    // Создание URI с нуля
    Poco::URI uri;
    uri.setScheme("https");
    uri.setHost("api.example.com");
    uri.setPort(443);
    uri.setPath("/v1/users");
    uri.addQueryParameter("limit", "10");
    uri.addQueryParameter("offset", "0");
    uri.addQueryParameter("sort", "name");
    uri.setFragment("results");
    
    std::cout << "Constructed URI: " << uri.toString() << std::endl;
    
    // Модификация существующего URI
    Poco::URI base_uri("https://example.com/api/v1");
    
    // Разрешение относительных путей
    std::cout << "\nURI Resolution Examples:" << std::endl;
    
    Poco::URI resolved1 = base_uri;
    resolved1.resolve("/products");
    std::cout << "Resolve '/products': " << resolved1.toString() << std::endl;
    
    Poco::URI resolved2 = base_uri;
    resolved2.resolve("items/123");
    std::cout << "Resolve 'items/123': " << resolved2.toString() << std::endl;
    
    Poco::URI resolved3 = base_uri;
    resolved3.resolve("../v2/users");
    std::cout << "Resolve '../v2/users': " << resolved3.toString() << std::endl;
    
    // Нормализация пути
    Poco::URI messy_uri("https://example.com/.././path/../new/./file.html");
    std::cout << "\nOriginal: " << messy_uri.toString() << std::endl;

    messy_uri.normalize();
    std::cout << "Normalized: " << messy_uri.toString() << std::endl;
}

int main()
{
    uri_construction_demo();
}
