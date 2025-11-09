#include "Poco/Environment.h"
#include <iostream>

void printSystemInfo()
{
    std::cout << "Operating System: " << Poco::Environment::osName() << std::endl;
    std::cout << "OS Version: " << Poco::Environment::osVersion() << std::endl;
    std::cout << "OS Architecture: " << Poco::Environment::osArchitecture() << std::endl;
    std::cout << "Node Name: " << Poco::Environment::nodeName() << std::endl;
    std::cout << "Node ID: " << Poco::Environment::nodeId() << std::endl;
}

int main()
{
    printSystemInfo();
}
