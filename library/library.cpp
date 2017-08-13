
#include <stdio.h>
#include <string>
#include <stdint.h>

#include "../whereami.h"
#include "print_chs/print_chs.h"


#ifdef WIN32
#include <windows.h>
// #define WINAPI __stdcall // even extern "C" cannot resolve name mangling
#define WINAPI 
#define CPP_EXPORT __declspec(dllexport)
#define GNUC_DLL_LOAD 
#else
#define WINAPI
#define CPP_EXPORT  __attribute__ ((visibility("default")))
#define GNUC_DLL_LOAD   __attribute__((constructor))
#endif

/*
测试的时候有个小 bug ：
 executable 和 library 都使用了 whereami.cpp ，都 link 了进去，
 运行和调试发现 library 里使用的是 executable 中 link 的 get_library_fullpath 
 导致获取的 fullpath 总是 executable 的，不符合预期。
 
 解决方案：
  给 executable/library 任一加编译选项 
  #target_compile_options(${PROJECT_NAME} PRIVATE -fvisibility=hidden)
  即可。
*/


extern "C"
CPP_EXPORT
int
WINAPI run(void)
{
    int err;
    std::wstring s1;

    err = get_executable_fullpath(&s1);

    printf("library->get_executable_fullpath() return:%d, addr:%p, size:%zu\n"
    ,err,s1.c_str(), s1.size());
    s1.append(L"\n");
    print_chs_w(s1);


    s1.clear();
    err = get_library_fullpath(&s1);
    printf("library->get_library_fullpath() return:%d, addr:%p, size:%zu\n"
    ,err,s1.c_str(), s1.size());
    s1.append(L"\n");
    print_chs_w(s1);


    return  0;
}

/*
    Function pointer type.
*/
typedef int (WINAPI * PFNrun)(void);

GNUC_DLL_LOAD
static
int load()
{
    // Called when the library was load.
    return 0;
}

