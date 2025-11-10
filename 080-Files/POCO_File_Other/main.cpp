#include "Poco/TemporaryFile.h"
#include "Poco/File.h"
#include "Poco/Path.h"
#include <iostream>
#include <stack>

void recursiveList(const std::string& rootPath)
{
    std::stack<std::string> directories;
    directories.push(rootPath);
    
    while (!directories.empty())
    {
        std::string current = directories.top();
        directories.pop();
        
        Poco::File currentDir(current);
        if (currentDir.exists() && currentDir.isDirectory())
        {
            std::vector<std::string> entries;
            currentDir.list(entries);
            
            for (const auto& entry : entries)
            {
                Poco::Path entryPath(current, entry);
                Poco::File entryFile(entryPath.toString());
                
                if (entryFile.isDirectory())
                {
                    directories.push(entryPath.toString());
                    std::cout << "[DIR] " << entryPath.toString() << std::endl;
                }
                else
                {
                    std::cout << "[FILE] " << entryPath.toString() 
                              << " (" << entryFile.getSize() << " bytes)" << std::endl;
                }
            }
        }
    }
}

int main()
{
    recursiveList("/home/cbf/LearningPOCO/Simple/080-Files");
}

