#include "Poco/Path.h"
#include "Poco/Environment.h"
#include <iostream>

// Поиск исполняемых файлов в PATH

bool findExecutable(const std::string& execName, Poco::Path& foundPath)
{
    std::string pathEnv = Poco::Environment::get("PATH");
    return Poco::Path::find(pathEnv, execName, foundPath);
}

int main()
{
    Poco::Path gitPath;
    if (findExecutable("git", gitPath))
    {
        std::cout << "Git found at: " << gitPath.toString() << "\n";
    }
}

