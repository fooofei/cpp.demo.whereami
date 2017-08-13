

#ifdef WIN32

#include <windows.h>
#include "whereami.h"

/* Used on MS_Windows*/

static int _get_module_fullpath(HMODULE m, std::wstring *out)
{
    if (!out)
        return ERROR_FAIL;
    out->clear();

    DWORD size1 = 0x10;
    DWORD r = 0;
    std::wstring buf1;
    do
    {
        buf1.clear();
        size1 *= 2;
        buf1.resize(size1);

        // If succeed, return value not include the terminating null character.
        r = GetModuleFileNameW(m, &buf1[0], size1);
        if (r == 0)
            return ERROR_FAIL;
        out->assign(buf1.c_str(), r);
    } while (!(r < size1));

    return ERROR_NOERROR;
}

int get_library_fullpath(std::wstring *out)
{
    HMODULE m = NULL;
    BOOL b = GetModuleHandleEx(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS | GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT, (LPCTSTR)get_library_fullpath, &m);
    if (!b)
        return ERROR_FAIL;
    if (!m)
        return ERROR_FAIL;

    return _get_module_fullpath(m, out);
}
int get_executable_fullpath(std::wstring *out)
{
    return _get_module_fullpath(NULL, out);
}

#endif //WIN32

#ifdef __linux__

#include <stdlib.h>
#include <limits.h>
#include <stdint.h>
#include <stdio.h>
#include <inttypes.h>
#include "whereami.h"

static unsigned get_path_max_size()
{
    unsigned path_max;
// http://man7.org/linux/man-pages/man3/realpath.3.html
#ifdef PATH_MAX
    path_max = PATH_MAX;
#else
    path_max = pathconf(path, _PC_PATH_MAX);
    if (path_max <= 0)
        path_max = 4096;
#endif
    return path_max;
}

/* string is utf-8 bytes*/
static int get_executable_fullpath_utf8(std::string *out)
{
    const char *self = "/proc/self/exe";
    std::string buf;
    char *ret;
    const unsigned path_max = get_path_max_size();

    buf.resize(path_max + 1);

    ret = realpath(self, &buf[0]);
    if (!ret)
        return ERROR_FAIL;

    if (!out)
        return ERROR_FAIL;
    out->clear();
    out->assign(ret);
    return ERROR_NOERROR;
}

/* string is utf-8 bytes */
static int get_library_fullpath_utf8(std::string *out)
{
    FILE *fmaps = NULL;
    const char *pmaps = "/proc/self/maps";
    char line[0x800];
    char *p;
    char *p2;
    const unsigned path_max = get_path_max_size();
    int retval = ERROR_FAIL;
    int retval_temp;
    uint64_t addr0 = (uint64_t)(uintptr_t)(void *)"";
    (void)addr0;
    uint64_t addr1 = (uint64_t)(uintptr_t)(void *)get_library_fullpath_utf8;
    (void)addr1;
    uint64_t addr = (uint64_t)(uintptr_t)(void *)__builtin_extract_return_addr(__builtin_return_address(0));

    std::string path2;

    // line
    uint64_t low;
    uint64_t high;
    char perms[5];
    uint64_t offset;
    uint32_t major;
    uint32_t minor;
    uint32_t inode;
    std::string path;

    if (!out)
        return ERROR_FAIL;
    fmaps = fopen(pmaps, "r");
    path.resize(path_max + 1);
    path2.resize(path_max + 1);

    if (fmaps)
    {
        for (; !feof(fmaps);)
        {
            memset(line, 0, sizeof(line));
            p = fgets(line, sizeof(line), fmaps);

            if (p)
            {
                /* Address like 7f95d4cbd000-7f95d4e7c000 r-xp 00000000 08:01 2257311  /lib/x86_64-linux-gnu/libc-2.23.so */
                retval_temp = sscanf(p, "%" PRIx64 "-%" PRIx64 " %s %" PRIx64 " %x:%x %u %s\n",
                                     &low, &high, perms, &offset, &major, &minor, &inode, &path[0]);
                if (retval_temp == 8)
                {
                    // Must be the the number of items in the argument list
                    if (addr >= low && addr <= high)
                    {
                        p2 = realpath(path.c_str(), &path2[0]);
                        if (!p2)
                        {
                            retval = ERROR_FAIL;
                            break;
                        }
                        out->assign(p2);
                        retval = ERROR_NOERROR;
                        break;
                    }
                }
            }
        }

        fclose(fmaps);
    }

    return retval;
}

#endif // __linux__

#ifdef __APPLE__

#define _DARWIN_BETTER_REALPATH
#include <mach-o/dyld.h>
#include <stdint.h>
#include <limits.h>
#include <stdlib.h>
#include <dlfcn.h>

#include "whereami.h"

static int get_executable_fullpath_utf8(std::string *out)
{
    std::string s;
    std::string s2;
    uint32_t size = 10;
    int retval_temp;
    char *p;

    s.clear();
    s.resize(size);
    retval_temp = _NSGetExecutablePath(&s[0], &size);
    // size include the '\0' .
    if (-1 == retval_temp && size < (uint32_t)-1)
    {
        size += 1; // No need plus 1, only for safe.
        s.clear();
        s.resize(size);
        // If the size is large than the need size, then only return it.
        // Not the real size.
        retval_temp = _NSGetExecutablePath(&s[0], &size);
    }
    if (0 != retval_temp)
        return ERROR_FAIL;

    s2.resize(PATH_MAX);
    p = realpath(s.c_str(), &s2[0]);
    if (p && out)
    {
        out->assign(p);
        return ERROR_NOERROR;
    }
    return ERROR_FAIL;
}

static int get_library_fullpath_utf8(std::string *out)
{
    Dl_info info;
    char *p;
    const char *cp;
    std::string s;
    int retval;

    memset(&info, 0, sizeof(info));
    retval = dladdr((void *)"", &info);
    if (retval != 0)
    {
        cp = info.dli_fname;
        s.resize(PATH_MAX);
        p = realpath(cp, &s[0]);
        if (p && out)
        {
            out->assign(p);
            return ERROR_NOERROR;
        }
    }
    return ERROR_FAIL;
}

// int get_library_fullpath(std::wstring * out);
// int get_executable_fullpath(std::wstring * out);

#endif // __APPLE__

#ifndef WIN32
#include "encoding/encoding_std.h"
int get_library_fullpath(std::wstring *out)
{
    std::string s;
    int err;

    err = get_library_fullpath_utf8(&s);
    if (err != ERROR_NOERROR)
        return err;
    // utf-8 -> wstring

    HRESULT hr;

    if (!out)
        return ERROR_FAIL;
    hr = utf8_2_wstring(s, *out);
    if (FAILED(hr))
        return ERROR_FAIL;

    return ERROR_NOERROR;
}

int get_executable_fullpath(std::wstring *out)
{
    std::string s;
    int err;

    err = get_executable_fullpath_utf8(&s);
    if (err != ERROR_NOERROR)
        return err;
    // utf-8 -> wstring

    HRESULT hr;

    if (!out)
        return ERROR_FAIL;
    hr = utf8_2_wstring(s, *out);
    if (FAILED(hr))
        return ERROR_FAIL;

    return ERROR_NOERROR;
}

#endif // !WIN32
