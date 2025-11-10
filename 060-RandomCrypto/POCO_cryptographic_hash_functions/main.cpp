// #include "Poco/MDMD2Engine.h"    // MD2 (устаревший) - Уже нету
#include "Poco/MD4Engine.h"    // MD4 (устаревший)  
#include "Poco/MD5Engine.h"    // MD5 (ненадежный для безопасности)
#include "Poco/SHA1Engine.h"   // SHA-1 (ослабленный)
#include <iostream>

int main()
{
    std::string line = "Hello World";

    Poco::MD5Engine md5;
    Poco::MD4Engine md4;
    Poco::SHA1Engine sha1;
    md5.update(line);
    md4.update(line);
    sha1.update(line);
    const Poco::DigestEngine::Digest& digestMD5 = md5.digest();
    const Poco::DigestEngine::Digest& digestMD4 = md4.digest();
    const Poco::DigestEngine::Digest& digestSHA1 = sha1.digest();

    std::string hashStrMD5 = Poco::DigestEngine::digestToHex(digestMD5);
    std::string hashStrMD4 = Poco::DigestEngine::digestToHex(digestMD4);
    std::string hashStrSHA1 = Poco::DigestEngine::digestToHex(digestSHA1);

    std::cout << "Строка до обработки: " << line << "\n";
    std::cout << "Строека после обработки MD4: " << hashStrMD4 << "\n";
    std::cout << "Строека после обработки MD5: " << hashStrMD5 << "\n";
    std::cout << "Строека после обработки SHA1: " << hashStrSHA1 << "\n";
}
