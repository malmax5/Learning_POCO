#include "Poco/Process.h"
#include "Poco/SharedMemory.h"
#include "Poco/NamedMutex.h"
#include "Poco/File.h"
#include "Poco/Thread.h"
#include <iostream>
#include <vector>
#include <fstream>
#include <cstring>

using Poco::Process;
using Poco::SharedMemory;
using Poco::NamedMutex;
using Poco::Thread;
using Poco::File;
using namespace std;

void shared_memory_demo()
{
    std::cout << "\n=== Shared Memory Demo ===" << std::endl;
    
    const std::string memoryName = "MySharedMemory";
    std::size_t memorySize = 1024;
    Poco::NamedMutex mutex("MyAppMutex");
    
    try
    {
        Poco::NamedMutex::ScopedLock lock(mutex);
        // Создание или подключение к разделяемой памяти
        Poco::SharedMemory memory(memoryName, memorySize, 
                                 (SharedMemory::AccessMode)(Poco::SharedMemory::AM_READ | Poco::SharedMemory::AM_WRITE));
        
        std::cout << "Shared memory created/opened. Size: " << memorySize << " bytes" << std::endl;
        
        // Запись данных в разделяемую память
        const char* message = "Hello from process!";
        std::strncpy(memory.begin(), message, std::min(std::strlen(message), memorySize - 1));
        static_cast<char*>(memory.begin())[std::min(std::strlen(message), memorySize - 1)] = '\0';
        
        std::cout << "Written to shared memory: " << static_cast<char*>(memory.begin()) << std::endl;
        
        // Чтение данных обратно (для демонстрации)
        std::cout << "Reading from shared memory: " << static_cast<char*>(memory.begin()) << std::endl;
    }
    catch (Poco::Exception& e)
    {
        std::cout << "Shared memory error: " << e.displayText() << std::endl;
    }
}

int main(int argc, char* argv[])
{
    std::vector<std::string> args = { "--worker" };

    if (argc == 2 && std::string(argv[1]) == "--worker")
    {
        shared_memory_demo();
        return 0;
    }

    std::vector<Poco::ProcessHandle> handles;

    for (int i = 0; i < 3; i++)
    {
        Poco::ProcessHandle consumer = Process::launch(argv[0], args);
        handles.push_back(consumer);
    }

    for (auto& handle : handles)
    {
        handle.wait();
    }

    return 0;
}
