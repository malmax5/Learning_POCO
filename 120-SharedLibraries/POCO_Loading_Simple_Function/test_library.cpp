#include "test_library.hpp"

void hello()
{
    std::cout << "Hello from shared library!" << std::endl;
}

int add(int a, int b)
{
    return a + b;
}

const char* get_message()
{
    return "Message from shared library";
}
