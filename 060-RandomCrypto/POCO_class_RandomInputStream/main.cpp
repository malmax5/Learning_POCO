#include "Poco/RandomStream.h"
#include <iostream>
#include <vector>

std::vector<unsigned char> generateEncryptionKey(int keySize)
{
    Poco::RandomInputStream ris;
    std::vector<unsigned char> key(keySize);
    ris.read(reinterpret_cast<char*>(key.data()), keySize);
    return key;
}

int main()
{
    Poco::RandomInputStream ris;

    auto aesKey = generateEncryptionKey(32);

    std::cout << "Random key: ";
    for (auto ch : aesKey)
    {
        std::cout << ch << " ";
    }
    std::cout << "\n";
}
