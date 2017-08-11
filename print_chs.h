
#include <locale.h> 
int print_chs_w(const wchar_t * ptr, unsigned size)
{
#ifdef WIN32
    const char * local = "chs";
#else
    const char * local = "zh_CN.UTF-8";
#endif

    char * restore = setlocale(LC_ALL,local);
    unsigned p_size = size*sizeof(wchar_t); // TODO ?

    printf("[addr:0x%p - size:%u]value:(%.*ls)\n",(const void*)ptr,size,p_size,ptr);

    setlocale(LC_ALL, restore);
    return 0;
}

int print_chs_w(const std::wstring & s)
{
    return print_chs_w(s.c_str(), (unsigned)s.size());
}