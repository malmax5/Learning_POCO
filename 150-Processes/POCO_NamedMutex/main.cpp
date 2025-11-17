#include "Poco/Process.h"
#include "Poco/NamedMutex.h"
#include "Poco/Thread.h"
#include <iostream>
#include <vector>
#include <fstream>

using Poco::Process;
using Poco::NamedMutex;
using Poco::Thread;
using namespace std;

void named_mutex_demo()
{
    std::cout << "\n=== Named Mutex Demo ===" << std::endl;
    
    Poco::NamedMutex mutex("MyAppMutex");
    
    std::cout << "Attempting to acquire named mutex..." << std::endl;
    
    {
        Poco::NamedMutex::ScopedLock lock(mutex);
        std::cout << "Mutex acquired! Writing to shared file..." << std::endl;
        
        // Запись в файл с защитой мьютексом
        std::ofstream file("shared_log.txt", std::ios::app);
        if (file.is_open())
        {
            file << "Process " << Poco::Process::id() << " wrote at: " 
                 << time(nullptr) << std::endl;
            file.close();
        }
        
        std::cout << "Working for 3 seconds..." << std::endl;
        Poco::Thread::sleep(3000);
        
        std::cout << "Releasing mutex..." << std::endl;
    }
    
    std::cout << "Mutex released!" << std::endl;
}

int main()
{
    named_mutex_demo();
}
