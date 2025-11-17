#include "Poco/LRUCache.h"
#include <iostream>

using Poco::LRUCache;
using namespace std;

void lrucache_demo()
{
    LRUCache<int, string> cache(3);

    cache.add(1, "Apple");
    cache.add(2, "Banana");
    cache.add(3, "Cherry");

    cout << "Size after adding 3 elements: " << cache.size() << endl; // 3

    cache.add(4, "Date"); // Вытеснит "Apple" (самый старый)

    auto item = cache.get(1);
    if (item.isNull())
    {
        cout << "Key 1 was evicted" << endl;
    }
}

int main()
{
    lrucache_demo();
}