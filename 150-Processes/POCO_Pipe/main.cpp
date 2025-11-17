#include "Poco/Process.h"
#include "Poco/PipeStream.h"
#include "Poco/StreamCopier.h"
#include <iostream>
#include <vector>
#include <sstream>

using Poco::Process;
using Poco::PipeStreamBuf;
using Poco::PipeInputStream;
using Poco::PipeOutputStream;
using Poco::StreamCopier;
using namespace std;

void pipe_redirection_demo()
{
    std::cout << "\n=== Pipe Redirection Demo ===" << std::endl;
    
    #ifdef POCO_OS_FAMILY_UNIX
        std::string command = "ps";
        std::vector<std::string> args = {"-ef"};
    #else
        std::string command = "tasklist.exe";
        std::vector<std::string> args;
    #endif
    
    Poco::Pipe outPipe;
    Poco::ProcessHandle ph = Poco::Process::launch(command, args, 0, &outPipe, 0);
    
    // Чтение вывода процесса через PipeInputStream
    Poco::PipeInputStream istr(outPipe);
    std::stringstream ss;
    Poco::StreamCopier::copyStream(istr, ss);
    
    std::cout << "Process output (first 500 chars):" << std::endl;
    std::string output = ss.str();
    if (output.length() > 500)
    {
        std::cout << output.substr(0, 500) << "..." << std::endl;
    }
    else
    {
        std::cout << output << std::endl;
    }
    
    int exitCode = ph.wait();
    std::cout << "Process finished with exit code: " << exitCode << std::endl;
}

int main()
{
    pipe_redirection_demo();
}
