#include "Poco/LRUCache.h"
#include "Poco/Delegate.h"
#include "Poco/KeyValueArgs.h"
#include <iostream>

// Правильные обработчики событий
void onAdd(const void* pSender, const Poco::KeyValueArgs<int, std::string>& args)
{
    std::cout << "Added: " << args.key() << " -> " << args.value() << std::endl;
}

void onRemove(const void* pSender, const int& key)
{
    std::cout << "Removed: " << key << std::endl;
}

int main()
{
    Poco::LRUCache<int, std::string> cache(2);

    // Подписка на события
    cache.Add    += Poco::delegate(&onAdd);
    cache.Remove += Poco::delegate(&onRemove);

    cache.add(1, "One");
    cache.add(2, "Two");
    cache.add(3, "Three"); // Вытеснит 1 → вызовет Remove

    return 0;
}