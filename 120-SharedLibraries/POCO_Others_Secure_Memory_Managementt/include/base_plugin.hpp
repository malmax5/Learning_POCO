#pragma once

#include <string>
#include <vector>
#include <memory>

// Базовый интерфейс для всех плагинов
class BasePlugin
{
public:
    virtual ~BasePlugin() = default;
    
    // Основные методы
    virtual std::string getName() const = 0;
    virtual std::string getVersion() const = 0;
    virtual void initialize() = 0;
    virtual void execute(const std::string& input) = 0;
    virtual std::string getResult() const = 0;
    
    // Дополнительная информация
    virtual std::vector<std::string> getCapabilities() const = 0;
};

// Фабрика для создания плагинов (альтернативный подход)
class PluginFactory
{
public:
    virtual ~PluginFactory() = default;
    virtual std::unique_ptr<BasePlugin> createPlugin() = 0;
    virtual std::string getPluginType() const = 0;
};
