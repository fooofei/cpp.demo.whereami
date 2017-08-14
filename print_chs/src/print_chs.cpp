
#include <stdarg.h>
#include <stdio.h>
#include "../include/print_chs/print_chs.h"
#include "encoding/encoding_std.h"

int print_chs_w(const wchar_t *ptr, size_t size)
{
#ifdef WIN32
    const char *local = "chs";
#else
    const char *local = "zh_CN.UTF-8";
#endif

    char *restore = setlocale(LC_ALL, local);
    size_t bytes_size = size * sizeof(wchar_t);
    // printf("[addr:0x%p - size:%u]value:(%.*ls)\n",(const void*)ptr,size,p_size,ptr);
    printf("%.*ls", (int)bytes_size, ptr);
    setlocale(LC_ALL, restore);
    return size;
}

int print_chs_w(const std::wstring &s)
{
    return print_chs_w(s.c_str(), s.size());
}

int print_chs_w(const wchar_t *format, ...)
{
    std::wstring s;
    int ret;
    s.resize(4096);

    va_list args;
    va_start(args, format);
    ret = vswprintf(&s[0], s.size(), format, args);
    va_end(args);

    if (ret > 0)
    {
        return print_chs_w(s.c_str(), ret);
    }
    return ret;
}

int print_chs(const char *format, ...)
{
    std::string s;
    int ret;
    s.resize(4096);

    va_list args;
    va_start(args, format);
    ret = vsnprintf(&s[0], s.size(), format, args);
    va_end(args);

    if (ret > 0)
    {
        return print_chs(s.c_str(), ret);
    }
    return ret;
}
int print_chs(const char *ptr, size_t size)
{
    /*
    try on Windows:
    _setmode(_fileno(stdout), _O_U8TEXT); // crash, debug assert
    SetConsoleOutputCP(CP_UTF8); // not work
    
    have not found the way to print utf-8 bytes string on Windows.

    */
    
#ifdef WIN32
    std::string s(ptr, size);
    std::wstring ws;
    HRESULT hr;

    hr = utf8_2_wstring(s, ws);
    if (FAILED(hr)) return -1;
    return print_chs_w(ws);
#else
    const char *local = "zh_CN.UTF-8";
    char *restore = setlocale(LC_ALL, local);
    printf("%.*s", (int)size, ptr);
    setlocale(LC_ALL, restore);
    return size;
#endif // !WIN32

}
int print_chs(const std::string &s)
{
    return print_chs(s.c_str(), s.size());
}