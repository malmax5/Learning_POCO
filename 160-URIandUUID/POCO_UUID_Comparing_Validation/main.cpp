#include "Poco/UUID.h"
#include "Poco/UUIDGenerator.h"
#include <iostream>
#include <set>

void uuid_operations_demo()
{
    std::cout << "\n=== UUID Operations Demo ===" << std::endl;
    
    Poco::UUIDGenerator& generator = Poco::UUIDGenerator::defaultGenerator();
    
    // Генерация UUID для тестирования операций
    Poco::UUID uuid1 = generator.createRandom();
    Poco::UUID uuid2 = generator.createRandom();
    Poco::UUID uuid3 = uuid1; // Копия
    
    std::cout << "UUID1: " << uuid1.toString() << std::endl;
    std::cout << "UUID2: " << uuid2.toString() << std::endl;
    std::cout << "UUID3: " << uuid3.toString() << " (copy of UUID1)" << std::endl;
    
    // Сравнение UUID
    std::cout << "\nComparison Results:" << std::endl;
    std::cout << "UUID1 == UUID2: " << (uuid1 == uuid2 ? "true" : "false") << std::endl;
    std::cout << "UUID1 == UUID3: " << (uuid1 == uuid3 ? "true" : "false") << std::endl;
    std::cout << "UUID1 != UUID2: " << (uuid1 != uuid2 ? "true" : "false") << std::endl;
    std::cout << "UUID1 < UUID2:  " << (uuid1 < uuid2 ? "true" : "false") << std::endl;
    
    // Проверка на null UUID
    Poco::UUID null_uuid;
    std::cout << "\nNull UUID: " << null_uuid.toString() << std::endl;
    std::cout << "Is UUID1 null: " << (uuid1.isNull() ? "true" : "false") << std::endl;
    std::cout << "Is null_uuid null: " << (null_uuid.isNull() ? "true" : "false") << std::endl;
    
    // Преобразование строки в UUID
    std::string uuid_str = uuid1.toString();
    try
    {
        Poco::UUID from_string(uuid_str);
        std::cout << "\nString to UUID conversion successful: " << from_string.toString() << std::endl;
        std::cout << "Matches original: " << (from_string == uuid1 ? "YES" : "NO") << std::endl;
    }
    catch (const Poco::Exception& e)
    {
        std::cout << "Error converting string to UUID: " << e.displayText() << std::endl;
    }
    
    // Использование в контейнерах
    std::set<Poco::UUID> uuid_set;
    uuid_set.insert(uuid1);
    uuid_set.insert(uuid2);
    uuid_set.insert(uuid3); // Дубликат не добавится
    
    std::cout << "\nUUIDs in set: " << uuid_set.size() << " (unique only)" << std::endl;
}

int main()
{
    uuid_operations_demo();
}
