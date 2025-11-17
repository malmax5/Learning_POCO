#include "Poco/Process.h"
#include <iostream>
#include <vector>

using Poco::Process;
using namespace std;

void process_basic_demo()
{
    std::cout << "=== Basic Process Demo ===" << std::endl;
    
    // Получение информации о текущем процессе
    Poco::Process::PID pid = Poco::Process::id();
    std::cout << "Current process PID: " << pid << std::endl;
    
    long userTime, kernelTime;
    Poco::Process::times(userTime, kernelTime);
    std::cout << "User time: " << userTime << "s, Kernel time: " << kernelTime << "s" << std::endl;
    
    // Запуск простой команды (ls на Unix, dir на Windows)
    #ifdef POCO_OS_FAMILY_UNIX
        std::string command = "ls";
        std::vector<std::string> args = {"-la"};
    #else
        std::string command = "cmd.exe";
        std::vector<std::string> args = {"/c", "dir"};
    #endif
    
    Poco::ProcessHandle ph = Poco::Process::launch(command, args);
    std::cout << "Launched process with PID: " << ph.id() << std::endl;
    
    int exitCode = ph.wait();
    std::cout << "Process exited with code: " << exitCode << std::endl;
}

int main()
{
    process_basic_demo();
}
