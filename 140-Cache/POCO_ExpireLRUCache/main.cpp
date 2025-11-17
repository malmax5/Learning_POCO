#include "Poco/ExpireLRUCache.h"
#include <iostream>
#include <thread>
#include <chrono>

using Poco::ExpireLRUCache;
using namespace std;

void expirelrucache_demo()
{
    ExpireLRUCache<int, string> cache(3, 2000); // LRU=3, expire=2s

    cache.add(1, "A");
    cache.add(2, "B");
    cache.add(3, "C");

    cout << "Size init: " << cache.size() << endl; // 3
    cout << "Cache elements: ";
    for (auto& elem : cache.getAllKeys())
    {
        std::cout << elem << " ";
    }
    cout << endl;

    cache.add(4, "D"); // Вытеснит 1 (LRU)

    cout << "Size after adding: " << cache.size() << endl; // 3
    cout << "Cache elements: ";
    for (auto& elem : cache.getAllKeys())
    {
        std::cout << elem << " ";
    }
    cout << endl;

    this_thread::sleep_for(chrono::milliseconds(2500));

    cout << "Size after expire: " << cache.size() << endl; // 0
}

int main()
{
    expirelrucache_demo();
}
