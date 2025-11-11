#include "Poco/Base64Encoder.h"
#include "Poco/Base64Decoder.h"
#include "Poco/HexBinaryEncoder.h"
#include "Poco/HexBinaryDecoder.h"
#include <iostream>
#include <sstream>

/*
    Base64Encoder/Decoder: Кодирование в формат Base64 (RFC 4648)
    Использует 64 символа: A-Z, a-z, 0-9, +, /
    Коэффициент расширения: 4/3 (33% overhead)
    Применение: email attachments, XML binary data, HTTP авторизация
*/

/*
    HexBinaryEncoder/Decoder: Шестнадцатеричное кодирование
    Коэффициент расширения: 2/1 (100% overhead)
    Применение: отладочный вывод, протоколы связи
*/

void encoding_examples()
{
    std::cout << "=== Base64 Encoding ===" << "\n";
    
    std::stringstream original;
    original << "Hello, POCO Streams!";
    
    // Base64 кодирование
    std::stringstream base64Encoded;
    Poco::Base64Encoder base64Encoder(base64Encoded);
    base64Encoder << original.rdbuf();
    base64Encoder.close();
    
    std::cout << "Original: " << original.str() << "\n";
    std::cout << "Base64: " << base64Encoded.str() << "\n";
    
    // Base64 декодирование
    std::stringstream base64Decoded;
    Poco::Base64Decoder base64Decoder(base64Encoded);
    base64Decoder >> base64Decoded.rdbuf();
    
    std::cout << "Decoded: " << base64Decoded.str() << "\n";
    
    std::cout << "\n=== HexBinary Encoding ===" << "\n";
    
    // HexBinary кодирование
    std::stringstream hexEncoded;
    Poco::HexBinaryEncoder hexEncoder(hexEncoded);
    hexEncoder << "ABC";
    hexEncoder.close();
    
    std::cout << "HexBinary: " << hexEncoded.str() << "\n";
    
    // HexBinary декодирование
    std::stringstream hexDecoded;
    Poco::HexBinaryDecoder hexDecoder(hexEncoded);
    hexDecoder >> hexDecoded.rdbuf();
    
    std::cout << "Decoded: " << hexDecoded.str() << "\n";
}

int main()
{
    encoding_examples();
}

