#include "../include/base_plugin.hpp"
#include "Poco/ClassLibrary.h"
#include <iostream>
#include <cmath>
#include <sstream>
#include <map>

// Плагин для математических операций
class MathPlugin : public BasePlugin
{
public:
    std::string getName() const override
    {
        return "MathPlugin";
    }
    
    std::string getVersion() const override
    {
        return "1.0.0";
    }
    
    void initialize() override
    {
        _result = "0";
    }
    
    void execute(const std::string& input) override
    {
        // Простой парсинг выражений вида "5 + 3" или "sqrt(16)"
        try
        {
            if (input.find("+") != std::string::npos)
            {
                size_t pos = input.find("+");
                double a = std::stod(input.substr(0, pos));
                double b = std::stod(input.substr(pos + 1));
                _result = std::to_string(a + b);
            }
            else if (input.find("sqrt") != std::string::npos)
            {
                size_t start = input.find("(") + 1;
                size_t end = input.find(")");
                double val = std::stod(input.substr(start, end - start));
                _result = std::to_string(std::sqrt(val));
            }
            else
            {
                _result = "Unknown operation: " + input;
            }
        }
        catch (const std::exception& e)
        {
            _result = "Error: " + std::string(e.what());
        }
    }
    
    std::string getResult() const override
    {
        return _result;
    }
    
    std::vector<std::string> getCapabilities() const override
    {
        return {"math_operations", "addition", "square_root"};
    }
    
private:
    std::string _result;
};

// Плагин для статистических вычислений
class StatisticsPlugin : public BasePlugin
{
public:
    std::string getName() const override
    {
        return "StatisticsPlugin";
    }
    
    std::string getVersion() const override
    {
        return "1.0.0";
    }
    
    void initialize() override
    {
        _numbers.clear();
        _result = "Statistics ready";
    }
    
    void execute(const std::string& input) override
    {
        try
        {
            double number = std::stod(input);
            _numbers.push_back(number);
            
            double sum = 0;
            for (double n : _numbers) sum += n;
            double mean = sum / _numbers.size();
            
            _result = "Count: " + std::to_string(_numbers.size()) +
                     ", Mean: " + std::to_string(mean);
        }
        catch (const std::exception& e)
        {
            _result = "Error: Invalid number - " + std::string(e.what());
        }
    }
    
    std::string getResult() const override
    {
        return _result;
    }
    
    std::vector<std::string> getCapabilities() const override
    {
        return {"statistics", "mean_calculation"};
    }
    
private:
    std::vector<double> _numbers;
    std::string _result;
};

POCO_BEGIN_MANIFEST(BasePlugin)
    POCO_EXPORT_CLASS(MathPlugin)
    POCO_EXPORT_CLASS(StatisticsPlugin)
POCO_END_MANIFEST

void pocoInitializeLibrary()
{
    std::cout << "=== MathPlugins Library Initialized ===" << std::endl;
}

void pocoUninitializeLibrary()
{
    std::cout << "=== MathPlugins Library Uninitialized ===" << std::endl;
}