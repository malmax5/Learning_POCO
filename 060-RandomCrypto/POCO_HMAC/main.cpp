#include "Poco/HMACEngine.h"
#include "Poco/SHA1Engine.h"
#include <iostream>

int main()
{
    std::string secretKey = "my-secret-key"; // HMAC hashing by secret key
    std::string line = "Hello World"; // string which we hash

    Poco::HMACEngine<Poco::SHA1Engine> hmac(secretKey);

    hmac.update(line);
    const Poco::DigestEngine::Digest& digestHMAC = hmac.digest();

    std::string hashStrHMAC = Poco::DigestEngine::digestToHex(digestHMAC);

    std::cout << "Строка до обработки: " << line << "\n";
    std::cout << "Секретный ключь для хэширования: " << secretKey << "\n";
    std::cout << "Строека после обработки HMAC: " << hashStrHMAC << "\n";
}
