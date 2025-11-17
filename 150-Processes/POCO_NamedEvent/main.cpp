#include "Poco/Process.h"
#include "Poco/NamedEvent.h"
#include "Poco/Thread.h"
#include <iostream>
#include <vector>
#include <fstream>

using Poco::Process;
using Poco::NamedEvent;
using Poco::Thread;
using namespace std;

void named_event_producer()
{
    std::cout << "=== Named Event Producer ===" << std::endl;
    
    Poco::NamedEvent event("MyAppEvent");
    
    for (int i = 1; i <= 3; ++i)
    {
        std::cout << "Sending event signal #" << i << "..." << std::endl;
        event.set();
        Poco::Thread::sleep(2000);
    }
    
    std::cout << "Producer finished!" << std::endl;
}

void named_event_consumer() {
    std::cout << "=== Named Event Consumer ===" << std::endl;
    
    Poco::NamedEvent event("MyAppEvent");
    
    for (int i = 1; i <= 3; ++i) {
        std::cout << "Waiting for event #" << i << "..." << std::endl;
        event.wait();
        std::cout << "Received event #" << i << " at: " << time(nullptr) << std::endl;
    }
    
    std::cout << "Consumer finished!" << std::endl;
}

void named_event_demo(std::string role)
{
    if (role == "--consumer")
    {
        named_event_consumer();
    }
    else if (role == "--producer")
    {
        named_event_producer();
    }
}

int main(int argc, char* argv[])
{
    if (argc == 2)
    {
        std::string role = argv[1];
        named_event_demo(role);

        return 0;
    }

    // --- Режим запуска обоих процессов из main ---
    std::cout << "=== Starting IPC demo with Poco::Process ===" << std::endl;

    // Запускаем consumer как отдельный процесс
    std::vector<std::string> args = { "--consumer" };
    Poco::ProcessHandle consumer = Process::launch(argv[0], args);

    // Даём consumer'у время инициализироваться
    Thread::sleep(500);

    // Запускаем producer в текущем процессе
    named_event_producer();

    // Ждём завершения consumer'а
    int exitCode = consumer.wait();
    std::cout << "=== Consumer exited with code: " << exitCode << " ===" << std::endl;

    return 0;
}
