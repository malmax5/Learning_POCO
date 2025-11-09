#include "Poco/Environment.h"
#include <iostream>
#include <string>

void printEnvironmentVariableSafe(std::string name)
{
    if (Poco::Environment::has(name))
    {
        std::string homeDir = Poco::Environment::get(name);
        std::cout << name << " variable: " << homeDir << "\n";
    }
    else
    {
        std::cout << name << " variable not set" << "\n";
    }
}

void printEnvironmentVariable(std::string name)
{
    try
    {
        std::string path = Poco::Environment::get(name);
    }
    catch (const std::exception& ex)
    {
        std::cerr << "Variable not found: " << ex.what() << "\n";
    }
}

bool setEnvironmentVariable(std::string name, std::string value)
{
    Poco::Environment::set("CUSTOM_PATH", "/this/path/is/castom");

    if (Poco::Environment::has("CUSTOM_PATH"))
    {
        return true;
    }

    return false;
}

int main()
{
    std::string name = "CUSTOM_PATH";
    std::string val = "/this/path/is/castom";
    if(setEnvironmentVariable(name, val))
    {
        printEnvironmentVariableSafe(name);
    }
}
