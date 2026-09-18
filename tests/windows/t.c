#include <windows.h>
#define __NO_FSL__
#define _FSL_MEM_H
#include <fsl.h>

int WINAPI WinMain(
    HINSTANCE hInstance,
    HINSTANCE hPrevInstance,
    LPSTR lpCmdLine,
    int nCmdShow)
{
    char buff[1024];
    mem_cpy(buff, "Hello from FSL!", 17);
    MessageBoxA(
        NULL,
        "FSL, Native C / Win32",
        "Message Box",
        MB_OK
    );

    return 0;
}