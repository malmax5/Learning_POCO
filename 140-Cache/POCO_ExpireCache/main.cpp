#include "Poco/ExpireCache.h"
#include <iostream>
#include <thread>
#include <chrono>

using Poco::ExpireCache;
using namespace std;

void expirecache_demo()
{
    ExpireCache<int, string> cache(2000); // 2 секунды

    cache.add(1, "Hello");
    cache.add(2, "World");

    cout << "Size: " << cache.size() << endl; // 2

    this_thread::sleep_for(chrono::milliseconds(2500));

    cout << "Size after 2.5s: " << cache.size() << endl; // 0
}

int main()
{
    expirecache_demo();
}
