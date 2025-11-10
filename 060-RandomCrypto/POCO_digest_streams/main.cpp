#include "Poco/DigestStream.h"
#include "Poco/MD5Engine.h"
#include <sstream>
#include <iostream>

// DigestOutputStream — вычисление хэша при записи
// DigestInputStream — вычисление хэша при чтении

int main()
{
    // Запись данных в поток (параллельно вычисляется хэш)
    Poco::MD5Engine md5;
    Poco::DigestOutputStream dos(md5);

    dos << "first part" << "second part";

    dos.flush();

    const auto& digestOutput = md5.digest();
    std::string hashOutput = Poco::DigestEngine::digestToHex(digestOutput);

    std::cout << "Hash output stream: " << hashOutput << "\n";

    // Чтение данных (параллельно вычисляется хэш)
    std::istringstream dataStream("first partsecond part");
    Poco::DigestInputStream dis(md5, dataStream);

    std::string content;
    dis >> content;

    const auto& digestInput = md5.digest();
    std::string hashInput = Poco::DigestEngine::digestToHex(digestInput);

    std::cout << "Hash output stream: " << hashInput << "\n";
}
