#include <windows.h>

#pragma comment(lib, "winmm.lib")

int eff_time_win32(void) {
    return (int) timeGetTime();
}
