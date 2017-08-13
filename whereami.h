#ifndef WHERE_AM_I_H
#define WHERE_AM_I_H

#include <string.h>
#include <string>

/*

Write code in the library or executable.
In library:
    get_library_fullpath() get the library fullpath,
    get_executable_fullpath() get the executable which load the library fullpath.

if error return -1, if succeed return 0.

stackoverflow:
    https://stackoverflow.com/questions/1023306/finding-current-executables-path-without-proc-self-exe
    https://stackoverflow.com/questions/933850/how-do-i-find-the-location-of-the-executable-in-c

*/

enum
{
    ERROR_FAIL = -1,
    ERROR_NOERROR = 0,
    ERROR_NOIMPL = 1,

};

int get_library_fullpath(std::wstring *out);
int get_executable_fullpath(std::wstring *out);

#endif //WHERE_AM_I_H
