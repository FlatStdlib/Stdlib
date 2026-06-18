#include <windows.h>
#define ID_BTN 1
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {

    case WM_CREATE:
        CreateWindowEx(
            0,
            "BUTTON",              // class name
            "Click me",            // text
            WS_VISIBLE | WS_CHILD,
            50, 50, 120, 40,       // x, y, w, h
            hwnd,
            (HMENU)ID_BTN,
            NULL,
            NULL
        );
        break;

    case WM_COMMAND:
        if (LOWORD(wParam) == ID_BTN) {
            MessageBox(hwnd, "Button clicked!", "Info", MB_OK);
        }
        break;

    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    }

    return DefWindowProc(hwnd, msg, wParam, lParam);
}

int main() {
    const char CLASS_NAME[] = "MyWindow";

    WNDCLASS wc = {0};
    wc.lpfnWndProc = WndProc;
    wc.hInstance = GetModuleHandle(NULL);
    wc.lpszClassName = CLASS_NAME;

    RegisterClass(&wc);

    HWND hwnd = CreateWindowEx(
        0,
        CLASS_NAME,
        "Win32 Button Example",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 500, 300,
        NULL, NULL, wc.hInstance, NULL
    );

    ShowWindow(hwnd, SW_SHOW);

    MSG msg = {0};
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}