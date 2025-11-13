#pragma once

#include <iostream>
#include <string>

#if defined(_WIN32)
    #define LIBRARY_API __declspec(dllexport)
#else
    #define LIBRARY_API
#endif

extern "C"
{
    LIBRARY_API void hello();
    LIBRARY_API int add(int a, int b);
    LIBRARY_API const char* get_message();
}
