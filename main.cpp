#include <iostream>
#include <Windows.h>
#include <vector>
#include <map>

/////////////// Function Declarations ///////////////
void PressKey(WORD Key);
void Click();
    
int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nShowCmd) {
    /////////////// Windows Initialization ///////////////
    SetProcessDPIAware();

    /*AllocConsole();

    FILE* fp;
    freopen_s(&fp, "CONIN$", "r", stdin);
    freopen_s(&fp, "CONOUT$", "w", stdout);
    freopen_s(&fp, "CONOUT$", "w", stderr);*/

    /////////////// Variable Initialization ///////////////
    POINT FirstBoxPos = { 643, 412 };
    int SquareLength = 67;
    std::vector<int> SideLength = { 9, 8 };

    POINT CursorPos = FirstBoxPos;

    POINT EndPos = { 940, 440 };
    COLORREF EndColor = RGB(77, 193, 249);
    COLORREF CurrentEndColor = RGB(0, 0, 0);
    HDC Context = GetDC(NULL);

    bool bQuit = false;

    /////////////// Run Once ///////////////
    Sleep(1000);

    SetCursorPos(CursorPos.x, CursorPos.y);

    /////////////// Game Loop ///////////////
    while (true) {
        // Checks if ESCAPE is pressed, exits if true
        if (GetAsyncKeyState(VK_ESCAPE)) {
            std::cout << "Spacebar pressed. Exiting." << std::endl;
            bQuit = true;
            break;
        }

        // Debug: Prints cursor location and color
        GetCursorPos(&CursorPos);
        std::cout << "{" << CursorPos.x << ", " << CursorPos.y << "} \n";

        COLORREF Pos = GetPixel(Context, CursorPos.x, CursorPos.y);

        BYTE red = GetRValue(Pos);
        BYTE green = GetGValue(Pos);
        BYTE blue = GetBValue(Pos);

        std::cout << "COLORREF (RGB): R: " << static_cast<int>(red)
            << ", G: " << static_cast<int>(green)
            << ", B: " << static_cast<int>(blue) << std::endl;

        // Loops through all of the squares
        for (int i = 0; i < SideLength[0] * SideLength[1]; i++) {
            CurrentEndColor = GetPixel(Context, EndPos.x, EndPos.y);

            if (i % SideLength[0] == 0 && i != 0 && i != 1) {
                CursorPos.y += SquareLength;
                CursorPos.x = FirstBoxPos.x;
            }

            CursorPos.x += SquareLength;

            SetCursorPos(CursorPos.x, CursorPos.y);
            Click();

            if (CurrentEndColor == EndColor) {
                std::cout << "Color \n";
                PressKey(VK_SPACE);
                i = 0;
                CursorPos = FirstBoxPos;

                Sleep(500);

                SetCursorPos(CursorPos.x, CursorPos.y);
                Click();
            }

            if (GetAsyncKeyState(VK_ESCAPE)) {
                std::cout << "Spacebar pressed. Exiting." << std::endl;
                bQuit = true;
                break;
            }
             
            Sleep(100);
        }
          
        bQuit = true;
          
        if (bQuit) break;
    }    

    /////////////// Wrapping Up Before Close ///////////////
    DeleteDC(Context);
    ReleaseDC(NULL, Context);
    return 0;
}

/////////////// Function Definitions ///////////////
void Click() {
    INPUT inputs[2] = {};

    inputs[0].type = INPUT_MOUSE;
    inputs[0].mi.dwFlags = MOUSEEVENTF_LEFTDOWN;

    inputs[1].type = INPUT_MOUSE;
    inputs[1].mi.dwFlags = MOUSEEVENTF_LEFTUP;

    SendInput(2, inputs, sizeof(INPUT));
}

void PressKey(WORD Key) {
    INPUT ip[2];

    // --- Key Press ---
    ip[0].type = INPUT_KEYBOARD;
    ip[0].ki.wVk = Key;
    ip[0].ki.wScan = 0;
    ip[0].ki.dwFlags = 0; 
    ip[0].ki.time = 0;
    ip[0].ki.dwExtraInfo = 0;

    // --- Key Release ---
    ip[1].type = INPUT_KEYBOARD;
    ip[1].ki.wVk = Key;
    ip[1].ki.wScan = 0;
    ip[1].ki.dwFlags = KEYEVENTF_KEYUP;
    ip[1].ki.time = 0;
    ip[1].ki.dwExtraInfo = 0;

    SendInput(2, ip, sizeof(INPUT));
}