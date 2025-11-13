// Базовый пример: загрузка простой функции
#include "Poco/SharedLibrary.h"
#include <iostream>
#include <memory>

using Poco::SharedLibrary;

// Объявляем типы функций
typedef void (*HelloFunc)();
typedef int (*AddFunc)(int, int);
typedef const char* (*GetMessageFunc)();

void basic_shared_library_demo()
{
    std::cout << "=== Basic Shared Library Demo ===" << std::endl;
    
    try
    {
        // Загружаем библиотеку
        std::string prefix = "./lib";
        std::string libName = "test_library";
        std::string libPath = prefix + libName + SharedLibrary::suffix(); // Добавляет .so или .dll
        
        SharedLibrary library(libPath);
        std::cout << "Library loaded: " << libPath << std::endl;
        
        // Получаем указатели на функции
        HelloFunc helloFunc = (HelloFunc)library.getSymbol("hello");
        AddFunc addFunc = (AddFunc)library.getSymbol("add");
        GetMessageFunc getMessageFunc = (GetMessageFunc)library.getSymbol("get_message");
        
        // Вызываем функции
        helloFunc();
        std::cout << "5 + 3 = " << addFunc(5, 3) << std::endl;
        std::cout << "Message: " << getMessageFunc() << std::endl;
        
        // Проверяем наличие символов
        if (library.hasSymbol("hello"))
        {
            std::cout << "Symbol 'hello' exists" << std::endl;
        }
        
        // Выгружаем библиотеку
        library.unload();
        std::cout << "Library unloaded" << std::endl;
        
    }
    catch (const Poco::Exception& e)
    {
        std::cerr << "Error: " << e.displayText() << std::endl;
    }
}

// Более безопасная версия с RAII
class SafeLibraryLoader
{
private:
    std::unique_ptr<SharedLibrary> _library;
    
public:
    bool load(const std::string& path)
    {
        try
        {
            _library = std::make_unique<SharedLibrary>(path);
            return true;
        }
        catch (const Poco::Exception&)
        {
            return false;
        }
    }
    
    template<typename FuncType>
    FuncType getFunction(const std::string& path)
    {
        if (!_library)
        {
            throw std::runtime_error("Library not loaded");
        }

        return (FuncType)_library->getSymbol(path);
    }
    
    bool hasFunction(const std::string& path)
    {
        return _library && _library->hasSymbol(path);
    }
};

void safe_library_loading_demo()
{
    std::cout << "\n=== Safe Library Loading Demo ===" << std::endl;
    
    SafeLibraryLoader loader;
    std::string libName = std::string("./lib") + std::string("test_library") + SharedLibrary::suffix();
    
    if (loader.load(libName))
    {
        if (loader.hasFunction("hello"))
        {
            auto helloFunc = loader.getFunction<HelloFunc>("hello");
            helloFunc();
        }
    } 
    else
    {
        std::cerr << "Failed to load library: " << libName << std::endl;
    }
}

int main()
{
    basic_shared_library_demo();
    safe_library_loading_demo();
}
