#include "include.h"
#include <windows.h>
#include <string>
#define ALLOC_BUTTON_ID 0x1
#define FREE_BUTTON_ID 0x2
#define COMBOBOX_ID 0x3

HINSTANCE hInst;
HFONT hButtonFont;
HFONT hTextBoxFont;
HWND hTextBox;
HWND hComboBox;
HWND hAllocButton;
HWND hFreeButton;
int unit = MB;
bool isFreeButtonEnabled = false;
std::vector<char> memoryPool;

LRESULT WINAPI WndProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam)
{
    switch (Message)
    {
    case WM_DESTROY:
    {
        DeleteObject(hButtonFont);
        DeleteObject(hTextBoxFont);
        PostQuitMessage(0);
        break;
    }

    case WM_CREATE:
    {
        hButtonFont = CreateFontA(
            26,
            0,
            0,
            0,
            FW_NORMAL,
            FALSE,
            FALSE,
            FALSE,
            DEFAULT_CHARSET,
            OUT_DEFAULT_PRECIS,
            CLIP_DEFAULT_PRECIS,
            DEFAULT_QUALITY,
            DEFAULT_PITCH | FF_SWISS,
            "Arial Bold");

        hTextBoxFont = CreateFontA(
            50,
            0,
            0,
            0,
            FW_BOLD,
            FALSE,
            FALSE,
            FALSE,
            DEFAULT_CHARSET,
            OUT_DEFAULT_PRECIS,
            CLIP_DEFAULT_PRECIS,
            DEFAULT_QUALITY,
            DEFAULT_PITCH | FF_SWISS,
            "Arial Bold");

        hTextBox = CreateWindowA(
            "EDIT",
            "0",
            WS_VISIBLE | WS_CHILD | WS_BORDER | ES_AUTOHSCROLL | ES_RIGHT,
            20, 20, 200, 60,
            hwnd, nullptr, hInst, nullptr);

        SendMessage(hTextBox, WM_SETFONT, (WPARAM)hTextBoxFont, TRUE);

        hComboBox = CreateWindowA(
            "COMBOBOX",
            "MB",
            WS_VISIBLE | WS_CHILD | ES_READONLY | CBS_DROPDOWN | WS_VSCROLL | WS_BORDER,
            230, 21, 120, 180,
            hwnd, (HMENU)COMBOBOX_ID, hInst, nullptr);

        SendMessage(hComboBox, CB_ADDSTRING, 0, (LPARAM) "MB");
        SendMessage(hComboBox, CB_ADDSTRING, 0, (LPARAM) "GB");
        SendMessage(hComboBox, CB_SETCURSEL, 0, 0);
        SendMessage(hComboBox, WM_SETFONT, (WPARAM)hTextBoxFont, TRUE);

        hAllocButton = CreateWindowA(
            "BUTTON",
            "Fill Memory",
            WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON | WS_BORDER,
            20, 90, 170, 50,
            hwnd, (HMENU)ALLOC_BUTTON_ID, hInst, nullptr);

        SendMessage(hAllocButton, WM_SETFONT, (WPARAM)hButtonFont, TRUE);

        hFreeButton = CreateWindowA(
            "BUTTON",
            "Free Memory",
            WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON | WS_BORDER,
            200, 90, 149, 50,
            hwnd, (HMENU)FREE_BUTTON_ID, hInst, nullptr);

        SendMessage(hFreeButton, WM_SETFONT, (WPARAM)hButtonFont, TRUE);

        break;
    }

    case WM_COMMAND:
    {
        switch (LOWORD(wParam))
        {
        case COMBOBOX_ID:
        {
            if (HIWORD(wParam) == CBN_SELCHANGE)
            {
                int index = SendMessage(hComboBox, CB_GETCURSEL, 0, 0);
                if (index != CB_ERR)
                {
                    char selectedText[3];
                    SendMessage(hComboBox, CB_GETLBTEXT, index, (LPARAM)selectedText);
                    if (strcmp(selectedText, "MB") == 0)
                    {
                        unit = MB;
                    }
                    else if (strcmp(selectedText, "GB") == 0)
                    {
                        unit = GB;
                    }
                }
            }
            break;
        }

        case ALLOC_BUTTON_ID:
        {
            char size_str[50];
            GetWindowText(hTextBox, size_str, sizeof(size_str));
            bool is_valid_input = true;

            long long unsigned size = 0;
            char numbers[10] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9'};
            for (int i = 0; i < strlen(size_str); i++)
            {
                bool is_digit = false;
                for (int j = 0; j < 10; j++)
                {
                    if (size_str[i] == numbers[j])
                    {
                        is_digit = true;
                        break;
                    }
                }
                if (!is_digit)
                {
                    MessageBoxA(hwnd, "Only positive integers are accepted", "Error", MB_ICONEXCLAMATION | MB_OK);
                    is_valid_input = false;
                    break;
                }
            }

            if (!is_valid_input)
            {
                break;
            }

            size = std::stoi(size_str);

            size_t final_size = size * unit;
            memoryPool.resize(final_size);
            std::fill(memoryPool.begin(), memoryPool.end(), 0);

            MessageBoxA(hwnd, "Memory filled successfully", "Message", MB_ICONINFORMATION | MB_OK);
            isFreeButtonEnabled = true;
            EnableWindow(hAllocButton, FALSE);
            EnableWindow(hTextBox, FALSE);
            EnableWindow(hComboBox, FALSE);
            break;
        }

        case FREE_BUTTON_ID:
        {
            memoryPool.clear();
            memoryPool.shrink_to_fit();
            MessageBoxA(hwnd, "Memory freed successfully", "Message", MB_ICONINFORMATION | MB_OK);
            isFreeButtonEnabled = false;
            EnableWindow(hAllocButton, TRUE);
            EnableWindow(hTextBox, TRUE);
            EnableWindow(hComboBox, TRUE);
            break;
        }
        }
    }
    default:
        return DefWindowProc(hwnd, Message, wParam, lParam);
    }
    return 0;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    WNDCLASSEX wc;
    HWND hwnd;
    MSG msg;

    memset(&wc, 0, sizeof(wc));
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInstance;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);

    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wc.lpszClassName = "WindowClass";
    wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

    if (!RegisterClassEx(&wc))
    {
        MessageBoxA(NULL, "Failed to register window class", "Error", MB_ICONEXCLAMATION | MB_OK);
        return 0;
    }

    hwnd = CreateWindowExA(WS_EX_CLIENTEDGE,
                          "WindowClass",
                          "memfill GUI",
                          WS_VISIBLE | WS_OVERLAPPEDWINDOW & ~WS_MAXIMIZEBOX,
                          CW_USEDEFAULT,
                          CW_USEDEFAULT,
                          385,
                          205,
                          NULL, NULL, hInstance, NULL);

    if (hwnd == NULL)
    {
        MessageBoxA(NULL, "Failed to create window", "Error", MB_ICONEXCLAMATION | MB_OK);
        return 0;
    }

    while (GetMessage(&msg, NULL, 0, 0) > 0)
    {
        if (isFreeButtonEnabled)
        {
            EnableWindow(hFreeButton, TRUE);
        }
        else
        {
            EnableWindow(hFreeButton, FALSE);
        }

        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return msg.wParam;
}