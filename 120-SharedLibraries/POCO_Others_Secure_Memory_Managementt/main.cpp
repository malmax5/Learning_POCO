// Безопасное управление памятью
#include "Poco/ClassLoader.h"
#include "Poco/SharedLibrary.h"
#include "include/base_plugin.hpp"
#include <memory>
#include <iostream>

using Poco::SharedLibrary;

template<typename BaseType>
class SafePluginWrapper
{
private:
    Poco::ClassLoader<BaseType>& _loader;
    BaseType* _instance;
    std::string _className;
    
public:
    SafePluginWrapper(Poco::ClassLoader<BaseType>& loader, 
                     const std::string& className)
        : _loader(loader), _className(className), _instance(nullptr)
    {
        _instance = _loader.create(className);
        if (_instance)
        {
            _loader.classFor(className).autoDelete(_instance);
        }
    }
    
    ~SafePluginWrapper()
    {
        // Память управляется MetaObject, поэтому не удаляем здесь
    }
    
    BaseType* operator->() const { return _instance; }
    BaseType* get() const { return _instance; }
    bool isValid() const { return _instance != nullptr; }
    
    // Запрещаем копирование
    SafePluginWrapper(const SafePluginWrapper&) = delete;
    SafePluginWrapper& operator=(const SafePluginWrapper&) = delete;
    
    // Разрешаем перемещение
    SafePluginWrapper(SafePluginWrapper&& other) noexcept
        : _loader(other._loader), _instance(other._instance), 
          _className(std::move(other._className))
    {
        other._instance = nullptr;
    }
    
    SafePluginWrapper& operator=(SafePluginWrapper&& other) noexcept
    {
        if (this != &other) {
            _instance = other._instance;
            _className = std::move(other._className);
            other._instance = nullptr;
        }
        return *this;
    }
};

void safe_plugin_demo()
{
    Poco::ClassLoader<BasePlugin> loader;
    loader.loadLibrary(std::string("./lib") + std::string("text_plugins") + SharedLibrary::suffix());
    
    SafePluginWrapper<BasePlugin> plugin(loader, "UpperCasePlugin");
    if (plugin.isValid())
    {
        plugin->initialize();
        plugin->execute("safe plugin demo");
        std::cout << "Result: " << plugin->getResult() << std::endl;
    }
}

int main()
{
    safe_plugin_demo();
}
