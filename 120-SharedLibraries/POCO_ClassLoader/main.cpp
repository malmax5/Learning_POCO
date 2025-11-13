// Пример системы плагинов
#include "Poco/ClassLoader.h"
#include "Poco/Manifest.h"
#include "Poco/SharedLibrary.h"
#include "include/base_plugin.hpp"
#include <iostream>
#include <memory>
#include <map>
#include <vector>
#include <algorithm>

using Poco::ClassLoader;
using Poco::Manifest;
using Poco::SharedLibrary;

class PluginManager
{
private:
    ClassLoader<BasePlugin> _loader;
    std::map<std::string, std::unique_ptr<BasePlugin>> _plugins;
    std::map<std::string, std::string> _libraryMap; // plugin -> library
    
public:
    ~PluginManager()
    {
        unloadAllLibraries();
    }
    
    // Загрузка библиотеки плагинов
    bool loadLibrary(const std::string& libraryPath)
    {
        try
        {
            std::string fullPath = std::string("./lib") + libraryPath + SharedLibrary::suffix();
            _loader.loadLibrary(fullPath);
            
            std::cout << "Loaded library: " << fullPath << std::endl;
            
            // Регистрируем плагины из этой библиотеки
            auto it = _loader.begin();
            auto end = _loader.end();
            
            for (; it != end; ++it)
            {
                if (it->first.find(libraryPath) != std::string::npos)
                {
                    Manifest<BasePlugin>::Iterator manIt = it->second->begin();
                    Manifest<BasePlugin>::Iterator manEnd = it->second->end();
                    
                    for (; manIt != manEnd; manIt++)
                    {
                        std::string pluginName = manIt->name();
                        _libraryMap[pluginName] = fullPath;
                        std::cout << "  Registered plugin: " << pluginName << std::endl;
                    }
                }
            }
            
            return true;
            
        }
        catch (const Poco::Exception& e)
        {
            std::cerr << "Failed to load library " << libraryPath 
                      << ": " << e.displayText() << std::endl;
            return false;
        }
    }
    
    // Создание экземпляра плагина
    std::unique_ptr<BasePlugin> createPlugin(const std::string& pluginName)
    {
        try
        {
            BasePlugin* plugin = _loader.create(pluginName);
            
            // Настраиваем автоматическое удаление через MetaObject
            _loader.classFor(pluginName);
            
            return std::unique_ptr<BasePlugin>(plugin);
        }
        catch (const Poco::Exception& e)
        {
            std::cerr << "Failed to create plugin " << pluginName 
                      << ": " << e.displayText() << std::endl;
            return nullptr;
        }
    }
    
    // Получение синглтона
    BasePlugin* getSingleton(const std::string& pluginName)
    {
        try
        {
            return &_loader.instance(pluginName);
        }
        catch (const Poco::Exception& e)
        {
            std::cerr << "Failed to get singleton " << pluginName 
                      << ": " << e.displayText() << std::endl;
            return nullptr;
        }
    }
    
    // Поиск плагинов по возможностям
    std::vector<std::string> findPluginsByCapability(const std::string& capability)
    {
        std::vector<std::string> result;
        
        auto it = _loader.begin();
        auto end = _loader.end();
        
        for (; it != end; ++it) {
            Manifest<BasePlugin>::Iterator manIt = it->second->begin();
            Manifest<BasePlugin>::Iterator manEnd = it->second->end();
            
            for (; manIt != manEnd; ++manIt)
            {
                // Создаем временный экземпляр для проверки возможностей
                BasePlugin* plugin = nullptr;
                bool shouldDelete;

                try
                {

                    try
                    {
                        plugin = &_loader.instance(manIt->name());
                        shouldDelete = false;
                    }
                    catch(...)
                    {
                        plugin = _loader.create(manIt->name());
                        shouldDelete = true;
                    }

                    if (plugin)
                    {
                        auto capabilities = plugin->getCapabilities();
                        if (std::find(capabilities.begin(), capabilities.end(), capability) 
                            != capabilities.end())
                        {
                            result.push_back(manIt->name());
                        }

                        if (shouldDelete)
                        {
                            delete plugin;
                        }
                    }
                }
                catch (const Poco::Exception& e)
                {
                    std::cout << "Warning: Could not check capabilities for " 
                            << manIt->name() << ": " << e.displayText() << std::endl;
                    
                    if (shouldDelete && plugin)
                    {
                        delete plugin;
                    }
                }
                catch (const std::exception& e)
                {
                    std::cout << "Warning: Standard error checking capabilities for " 
                            << manIt->name() << ": " << e.what() << std::endl;
                    
                    if (shouldDelete && plugin)
                    {
                        delete plugin;
                    }
                }
                catch (...)
                {
                    std::cout << "Warning: Unknown error checking capabilities for " 
                            << manIt->name() << std::endl;
                    
                    if (shouldDelete && plugin)
                    {
                        delete plugin;
                    }
                }
            }
        }
        
        return result;
    }
    
    // Выгрузка библиотеки
    void unloadLibrary(const std::string& libraryPath)
    {
        try
        {
            std::string fullPath = libraryPath;

            _loader.unloadLibrary(fullPath);
            std::cout << "Unloaded library: " << fullPath << std::endl;
            
            // Удаляем из карты
            for (auto it = _libraryMap.begin(); it != _libraryMap.end();)
            {
                if (it->second == fullPath)
                {
                    it = _libraryMap.erase(it);
                }
                else
                {
                    it++;
                }
            }
            
        }
        catch (const Poco::Exception& e)
        {
            std::cerr << "Error unloading library " << libraryPath 
                      << ": " << e.displayText() << std::endl;
        }
    }
    
    void unloadAllLibraries()
    {
        _plugins.clear();

        // Сначала собираем все пути к библиотекам
        std::vector<std::string> libraryPaths;
        auto it = _loader.begin();
        while (it != _loader.end())
        {
            libraryPaths.push_back(it->first);
            it++;
        }

        // Затем выгружаем библиотеки
        for (const auto& libPath : libraryPaths)
        {
            try
            {
                _loader.unloadLibrary(libPath);
            }
            catch (...)
            {
                // Игнорируем ошибки при выгрузке
            }
        }
        _libraryMap.clear();
    }
    
    // Получение списка загруженных плагинов
    std::vector<std::string> getLoadedPlugins() const
    {
        std::vector<std::string> plugins;
        for (const auto& pair : _libraryMap)
        {
            plugins.push_back(pair.first);
        }

        return plugins;
    }
};

void plugin_system_demo()
{
    std::cout << "=== Advanced Plugin System Demo ===" << std::endl;
    
    PluginManager pluginManager;
    
    // Загружаем библиотеки плагинов
    pluginManager.loadLibrary("text_plugins");
    pluginManager.loadLibrary("math_plugins");
    
    // Тестируем текстовые плагины
    {
        std::cout << "\n--- Testing Text Plugins ---" << std::endl;
        auto upperPlugin = pluginManager.createPlugin("UpperCasePlugin");
        if (upperPlugin)
        {
            upperPlugin->initialize();
            upperPlugin->execute("Hello World");
            std::cout << "UpperCase: " << upperPlugin->getResult() << std::endl;
        }

        auto reversePlugin = pluginManager.createPlugin("ReversePlugin");
        if (reversePlugin)
        {
            reversePlugin->initialize();
            reversePlugin->execute("Hello World");
            std::cout << "Reverse: " << reversePlugin->getResult() << std::endl;
        }
    }

    // Работа с синглтоном
    {
        std::cout << "\n--- Testing Singleton Plugin ---" << std::endl;
        auto counterPlugin = pluginManager.getSingleton("CounterPlugin");
        if (counterPlugin)
        {
            counterPlugin->initialize();
            counterPlugin->execute("First call");
            std::cout << "Counter: " << counterPlugin->getResult() << std::endl;

            counterPlugin->execute("Second call");
            std::cout << "Counter: " << counterPlugin->getResult() << std::endl;
        }
    }
    
    
    // Тестируем математические плагины
    {
        std::cout << "\n--- Testing Math Plugins ---" << std::endl;
        auto mathPlugin = pluginManager.createPlugin("MathPlugin");
        if (mathPlugin)
        {
            mathPlugin->initialize();
            mathPlugin->execute("5 + 3");
            std::cout << "Math: 5 + 3 = " << mathPlugin->getResult() << std::endl;

            mathPlugin->execute("sqrt(16)");
            std::cout << "Math: sqrt(16) = " << mathPlugin->getResult() << std::endl;
        }

        auto statsPlugin = pluginManager.createPlugin("StatisticsPlugin");
        if (statsPlugin)
        {
            statsPlugin->initialize();
            statsPlugin->execute("10");
            statsPlugin->execute("20");
            statsPlugin->execute("30");
            std::cout << "Statistics: " << statsPlugin->getResult() << std::endl;
        }
    }
    
    // Поиск плагинов по возможностям
    {
        std::cout << "\n--- Plugin Discovery ---" << std::endl;

        auto textPlugins = pluginManager.findPluginsByCapability("text_transform");
        std::cout << "Text transform plugins: ";
        for (const auto& name : textPlugins)
        {
            std::cout << name << " ";
        }
        std::cout << std::endl;

        auto mathPlugins = pluginManager.findPluginsByCapability("math_operations");
        std::cout << "Math operations plugins: ";
        for (const auto& name : mathPlugins)
        {
            std::cout << name << " ";
        }
        std::cout << std::endl;
    }
    
    {
        std::cout << "\n--- Loaded Plugins ---" << std::endl;

        auto loadedPlugins = pluginManager.getLoadedPlugins();
        for (const auto& name : loadedPlugins)
        {
            std::cout << "- " << name << std::endl;
        }
    }
    
    // Выгрузка библиотек произойдет автоматически в деструкторе PluginManager
}

int main()
{
    try
    {
        plugin_system_demo();
    }
    catch (const std::exception& e)
    {
        std::cerr << "Fatal error: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}
