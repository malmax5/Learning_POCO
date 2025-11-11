// Создание собственных потоков: Кастомный поток для шифрования ROT13
#include "Poco/UnbufferedStreamBuf.h"
#include <iostream>
#include <cctype>

// ROT13 шифрование символов
char rot13(char c)
{
    if (isalpha(c)) {
        char base = islower(c) ? 'a' : 'A';
        return (c - base + 13) % 26 + base;
    }
    return c;
}

// Кастомный stream buffer для ROT13
class Rot13StreamBuf : public Poco::UnbufferedStreamBuf
{
public:
    Rot13StreamBuf(std::ostream& ostr)
        : _ostr(ostr)
    {

    }
    
protected:
    int writeToDevice(char c)
    {
        _ostr.put(rot13(c));
        return charToInt(c);
    }
    
private:
    std::ostream& _ostr;
};

// Классы для работы с потоком ROT13
class Rot13IOS : public virtual std::ios
{
public:
    Rot13IOS(std::ostream& ostr)
        : _buf(ostr)
    {
        poco_ios_init(&_buf);
    }
    
    Rot13StreamBuf* rdbuf()
    {
        return &_buf;
    }
    
protected:
    Rot13StreamBuf _buf;
};

class Rot13OutputStream : public Rot13IOS, public std::ostream
{
public:
    Rot13OutputStream(std::ostream& ostr)
        : Rot13IOS(ostr),
          std::ostream(&_buf)
    {

    }
};

void custom_stream_example()
{
    std::cout << "=== Custom ROT13 Stream ===" << "\n";
    
    std::string original = "Hello World! This is a secret message.";
    std::cout << "Original: " << original << "\n";
    
    Rot13OutputStream rot13Stream(std::cout);
    std::cout << "Encrypted: ";
    rot13Stream << original << "\n";
    
    // Двойное применение ROT13 возвращает оригинальный текст
    Rot13OutputStream doubleRot13(std::cout);
    std::cout << "Double ROT13: ";
    doubleRot13 << "Uryyb Jbeyq! Guvf vf n frperg zrffntr." << "\n";
}

int main()
{
    custom_stream_example();
}

