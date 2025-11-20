#include "Poco/UUID.h"
#include "Poco/UUIDGenerator.h"
#include <iostream>
#include <vector>
#include <map>

void uuid_generation_demo()
{
    std::cout << "\n=== UUID Generation Demo ===" << std::endl;
    
    Poco::UUIDGenerator& generator = Poco::UUIDGenerator::defaultGenerator();
    
    // Генерация time-based UUID (требует MAC-адрес)
    std::cout << "Time-based UUIDs:" << std::endl;
    for (int i = 0; i < 3; ++i)
    {
        Poco::UUID uuid = generator.create();
        std::cout << "  " << uuid.toString() << std::endl;
        std::cout << "    Version: " << uuid.version() << std::endl;
        std::cout << "    Variant: " << uuid.variant() << std::endl;
    }
    
    // Генерация random UUID
    std::cout << "\nRandom UUIDs:" << std::endl;
    for (int i = 0; i < 3; ++i)
    {
        Poco::UUID uuid = generator.createRandom();
        std::cout << "  " << uuid.toString() << std::endl;
    }
    
    // Name-based UUID (детерминированные)
    std::cout << "\nName-based UUIDs:" << std::endl;
    
    // Использование пространства имен URI
    Poco::UUID uuid1 = generator.createFromName(Poco::UUID::uri(), "https://example.com/user/123");
    Poco::UUID uuid2 = generator.createFromName(Poco::UUID::uri(), "https://example.com/user/456");
    Poco::UUID uuid3 = generator.createFromName(Poco::UUID::uri(), "https://example.com/user/123"); // Должен совпасть с uuid1
    
    std::cout << "  User 123 (1): " << uuid1.toString() << std::endl;
    std::cout << "  User 456:     " << uuid2.toString() << std::endl;
    std::cout << "  User 123 (2): " << uuid3.toString() << std::endl;
    std::cout << "  Same for user 123: " << (uuid1 == uuid3 ? "YES" : "NO") << std::endl;
    
    // Использование пространства имен DNS
    Poco::UUID dns_uuid = generator.createFromName(Poco::UUID::dns(), "example.com");
    std::cout << "\nDNS-based UUID: " << dns_uuid.toString() << std::endl;
}

int main()
{
    uuid_generation_demo();
}
