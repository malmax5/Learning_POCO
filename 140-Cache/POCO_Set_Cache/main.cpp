#include "Poco/LRUCache.h"
#include "Poco/Void.h"
#include <iostream>

using Poco::LRUCache;
using Poco::Void;
using namespace std;

void cache_set_demo()
{
    LRUCache<string, Void> setCache(100);

    setCache.add("user1", Void());
    setCache.add("user2", Void());

    if (setCache.has("user1"))
    {
        cout << "user1 exists" << endl;
    }
}

int main()
{
    cache_set_demo();
}
