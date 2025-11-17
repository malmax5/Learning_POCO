#include "Poco/UniqueExpireCache.h"
#include "Poco/ExpirationDecorator.h"
#include <iostream>
#include <thread>
#include <chrono>

using Poco::UniqueExpireCache;
using Poco::ExpirationDecorator;
using namespace std;

void uniqueexpirecache_demo()
{
    typedef ExpirationDecorator<string> ExpString;
    UniqueExpireCache<int, ExpString> cache;

    cache.add(1, ExpString("Short", 500));   // 500 мс
    cache.add(2, ExpString("Long", 2000));   // 2000 мс

    cout << "Size: " << cache.size() << endl; // 2

    this_thread::sleep_for(std::chrono::milliseconds(1000));

    cout << "Size after 1s: " << cache.size() << endl; // 1

    auto item = cache.get(1);
    if (item.isNull())
    {
        cout << "Key 1 expired" << endl;
    }
}

int main()
{
    uniqueexpirecache_demo();
}
