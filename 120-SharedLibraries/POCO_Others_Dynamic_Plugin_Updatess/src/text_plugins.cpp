#include "../include/base_plugin.hpp"
#include "Poco/ClassLibrary.h"
#include <iostream>
#include <algorithm>
#include <cctype>
#include <sstream>

// Плагин для преобразования текста в верхний регистр
class UpperCasePlugin : public BasePlugin
{
public:
    std::string getName() const override
    {
        return "UpperCasePlugin";
    }
    
    std::string getVersion() const override
    {
        return "1.0.0";
    }
    
    void initialize() override
    {
        _result.clear();
    }
    
    void execute(const std::string& input) override
    {
        _result = input;
        std::transform(_result.begin(), _result.end(), _result.begin(), ::toupper);
    }
    
    std::string getResult() const override
    {
        return _result;
    }
    
    std::vector<std::string> getCapabilities() const override
    {
        return {"text_uppercase", "text_transform"};
    }
    
private:
    std::string _result;
};

// Плагин для обращения строки
class ReversePlugin : public BasePlugin
{
public:
    std::string getName() const override
    {
        return "ReversePlugin";
    }
    
    std::string getVersion() const override
    {
        return "1.0.0";
    }
    
    void initialize() override
    {
        _result.clear();
    }
    
    void execute(const std::string& input) override
    {
        _result = std::string(input.rbegin(), input.rend());
    }
    
    std::string getResult() const override
    {
        return _result;
    }
    
    std::vector<std::string> getCapabilities() const override
    {
        return {"text_reverse", "text_transform"};
    }
    
private:
    std::string _result;
};

// Синглтон-плагин (глобальный счетчик)
class CounterPlugin : public BasePlugin
{
public:
    static CounterPlugin& instance()
    {
        static CounterPlugin instance;
        return instance;
    }
    
    std::string getName() const override
    {
        return "CounterPlugin";
    }
    
    std::string getVersion() const override
    {
        return "1.0.0";
    }
    
    void initialize() override
    {
        _count = 0;
        _result = "Counter initialized";
    }
    
    void execute(const std::string& input) override
    {
        _count++;
        _result = "Execution count: " + std::to_string(_count) + 
                 ", Last input: " + input;
    }
    
    std::string getResult() const override
    {
        return _result;
    }
    
    std::vector<std::string> getCapabilities() const override
    {
        return {"counter", "statistics"};
    }
    
    int getCount() const
    {
        return _count;
    }
    
    CounterPlugin()
        : _count(0), _result("")
    {

    }

private:
    int _count;
    std::string _result;
};

// Регистрируем классы в манифесте
POCO_BEGIN_MANIFEST(BasePlugin)
    POCO_EXPORT_CLASS(UpperCasePlugin)
    POCO_EXPORT_CLASS(ReversePlugin)
    POCO_EXPORT_SINGLETON(CounterPlugin)
POCO_END_MANIFEST

// Функции инициализации библиотеки
void pocoInitializeLibrary()
{
    std::cout << "=== TextPlugins Library Initialized ===" << std::endl;
    std::cout << "Available plugins: UpperCasePlugin, ReversePlugin, CounterPlugin" << std::endl;
}

void pocoUninitializeLibrary()
{
    std::cout << "=== TextPlugins Library Uninitialized ===" << std::endl;
}