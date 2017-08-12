
#include <stdio.h>
#include <string>

#include "../whereami.h"
#include "../print_chs.h"


#if defined(_WIN32)

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#define WINAPI __stdcall

#define RTLD_LAZY 1
#define RTLD_NOW 2
#define RTLD_GLOBAL 4
#define RTLD_LOCAL 8

#include "../../encoding/encoding_std.h"

static 
void* dlopen(const char * name, int mode)
{
    if (name)
    {
        std::string s(name);
        std::wstring ws;

        utf8_2_wstring(s, ws);

        return (void*)LoadLibraryW(ws.c_str());

    }
    return NULL;
}

static 
int dlclose(void * handle)
{
    return FreeLibrary((HMODULE)handle) ? 0 : -1;
}

static
FARPROC dlsym(void * handle, const char * symbol)
{
    return GetProcAddress((HMODULE)handle, symbol);
}

#else
#include <dlfcn.h>
#define WINAPI
#endif // WIN32




typedef int (WINAPI * PFNrun)(void);

int main()
{

    int err;
    std::wstring s1;
    void * handle;


    err = get_executable_fullpath(&s1);

    printf("executable->get_executable_fullpath() return:%d\n",err);
    print_chs_w(s1);


    s1.clear();
    err = get_library_fullpath(&s1);
    printf("executable->get_library_fullpath() return:%d\n",err);
    print_chs_w(s1);


#ifdef __linux__
    handle = dlopen("libwhereami.so", RTLD_NOW);
#elif defined(__APPLE__)
    handle = dlopen("libwhereami.dylib", RTLD_NOW);
#elif defined (WIN32)
    handle = dlopen("whereami.dll", RTLD_NOW);
#endif
    if (handle)
    {
        PFNrun fn = (PFNrun)dlsym(handle, "run");
        if (fn)
        {
            fn();
        }
        else {
            printf("executable->false dlsym\n");
            err = -1;
        }

        dlclose(handle);
    }
    else
    {
        printf("executable->false dlopen\n");
        err = -1;
    }

    return err;
}
