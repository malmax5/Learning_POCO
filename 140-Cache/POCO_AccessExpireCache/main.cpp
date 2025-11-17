#include "Poco/AccessExpireCache.h"
#include <iostream>
#include <thread>
#include <chrono>

using Poco::AccessExpireCache;
using namespace std;

void accessexpirecache_demo()
{
    AccessExpireCache<int, string> cache(1000); // 1 секунда без доступа

    cache.add(1, "Data");

    this_thread::sleep_for(chrono::milliseconds(500));
    cache.get(1); // Обновляем время доступа

    this_thread::sleep_for(chrono::milliseconds(600));
    cout << "Size: " << cache.size() << endl; // 1 (ещё жив)

    this_thread::sleep_for(chrono::milliseconds(500));
    cout << "Size: " << cache.size() << endl; // 0 (умер)
}

int main()
{
    accessexpirecache_demo();
}
