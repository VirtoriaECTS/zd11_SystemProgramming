// pr11.cpp : Определяет точку входа для приложения.
//

#include "framework.h"
#include "pr11.h"
#include <commctrl.h>
#include "windows.h"
#include "commdlg.h"
#include <chrono>
#define MAX_LOADSTRING 100

// Глобальные переменные:
HINSTANCE hInst;                                // текущий экземпляр
WCHAR szTitle[MAX_LOADSTRING];                  // Текст строки заголовка
WCHAR szWindowClass[MAX_LOADSTRING];            // имя класса главного окна

// Отправить объявления функций, включенных в этот модуль кода:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Разместите код здесь.

    // Инициализация глобальных строк
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_PR11, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Выполнить инициализацию приложения:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_PR11));

    MSG msg;

    // Цикл основного сообщения:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}



//
//  ФУНКЦИЯ: MyRegisterClass()
//
//  ЦЕЛЬ: Регистрирует класс окна.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_PR11));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_PR11);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   ФУНКЦИЯ: InitInstance(HINSTANCE, int)
//
//   ЦЕЛЬ: Сохраняет маркер экземпляра и создает главное окно
//
//   КОММЕНТАРИИ:
//
//        В этой функции маркер экземпляра сохраняется в глобальной переменной, а также
//        создается и выводится главное окно программы.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Сохранить маркер экземпляра в глобальной переменной

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }
   HWND hTab = CreateWindowEx(0, TEXT("SysTabControl32"), nullptr,  WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN, 10, 10, 370, 200, hWnd, NULL, hInstance, NULL);
   const char* tabText1 = "Tab 1";
   const char* tabText2 = "Tab 2";
   int len1 = MultiByteToWideChar(CP_ACP, 0, tabText1, -1, NULL, 0);
   int len2 = MultiByteToWideChar(CP_ACP, 0, tabText2, -1, NULL, 0);
   LPWSTR wideTabText1 = new WCHAR[len1];
   LPWSTR wideTabText2 = new WCHAR[len2];
   MultiByteToWideChar(CP_ACP, 0, tabText1, -1, wideTabText1, len1);
   MultiByteToWideChar(CP_ACP, 0, tabText2, -1, wideTabText2, len2);

   // Create tabs
   TCITEMW tie;
   tie.mask = TCIF_TEXT;
   tie.pszText = wideTabText1;
   TabCtrl_InsertItem(hTab, 0, &tie);
   tie.pszText = wideTabText2;
   TabCtrl_InsertItem(hTab, 1, &tie);

   // Create group box on the first tab
   HWND hGroupBox = CreateWindow(TEXT("BUTTON"), TEXT("Group Box"), WS_CHILD | WS_VISIBLE | BS_GROUPBOX, 20, 40, 150, 100, hTab, NULL, hInstance, NULL);

   // Create IP address control on the second tab
   HWND hIPAddress = CreateWindowEx(0,TEXT("SysIPAddress32"), nullptr, WS_CHILD | WS_VISIBLE | WS_BORDER, 20, 40, 150, 25, hTab, NULL, hInstance, NULL);

   // Create slider on the second tab
   HWND hSlider = CreateWindowEx(0, TRACKBAR_CLASS, TEXT("msctls_statusbar32"), WS_CHILD | WS_VISIBLE | TBS_AUTOTICKS, 20, 80, 150, 30, hTab, NULL, hInstance, NULL);
  

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  ФУНКЦИЯ: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  ЦЕЛЬ: Обрабатывает сообщения в главном окне.
//
//  WM_COMMAND  - обработать меню приложения
//  WM_PAINT    - Отрисовка главного окна
//  WM_DESTROY  - отправить сообщение о выходе и вернуться
//
//
TCHAR szFindWhat[80] = TEXT("");
TCHAR szReplaceWith[80] = TEXT("");
BOOL showMasag = FALSE;
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    OPENFILENAME ofn;
    FINDREPLACE ofn1;

    static TCHAR szFileName[MAX_PATH];
    switch (message)
    {
    case WM_CREATE: {
        SetTimer(hWnd, 1, 10000, NULL);
    }break;
    case WM_CHAR: {
        MessageBox(hWnd, L" Пользователь нажал на клавишу буквы", L"Информация", MB_OK | MB_ICONINFORMATION);
    }break;
    case WM_KEYUP: {
        MessageBox(hWnd, L" Отпущена не системная клавиша", L"Информация", MB_OK | MB_ICONINFORMATION);
    }break;
    case WM_LBUTTONDBLCLK: {
        MessageBox(hWnd, L" Двойной клик", L"Информация", MB_OK | MB_ICONINFORMATION);
    }break;
        //case WM_LBUTTONDOWN: {
        //    MessageBox(hWnd, L" Левая кнопка мыши нажата", L"Информация", MB_OK | MB_ICONINFORMATION);
        //    break;}       
        //case WM_LBUTTONUP: {
        //    MessageBox(hWnd, L" Левая кнопка мыши отпущена", L"Информация", MB_OK | MB_ICONINFORMATION);
        //break;
        //}  
    case WM_MOUSEMOVE: {
        MessageBox(hWnd, L"Курсор перемещается", L"Информация", MB_OK | MB_ICONINFORMATION);
    }break;
    case WM_MOVE: {
        MessageBox(hWnd, L"Окно перемещено", L"Информация", MB_OK | MB_ICONINFORMATION);
    }break;
    case WM_COMMAND:
    {
        int wmId = LOWORD(wParam);
        // Разобрать выбор в меню:
        switch (wmId)
        {
        case ID_OPEN:
            ZeroMemory(&ofn, sizeof(ofn));
            ofn.lStructSize = sizeof(ofn);
            ofn.hwndOwner = NULL;
            ofn.lpstrFile = szFileName;
            ofn.lpstrFile[0] = '\0';
            ofn.nMaxFile = sizeof(szFileName);
            ofn.lpstrFilter = TEXT("All Files\0*.*\0");
            ofn.nFilterIndex = 1;
            ofn.lpstrFileTitle = NULL;
            ofn.nMaxFileTitle = 0;
            ofn.lpstrInitialDir = NULL;
            ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
            if (GetOpenFileName(&ofn) == TRUE) { MessageBox(hWnd, szFileName, TEXT("Выберите файл"), MB_OK); }
            /*               DialogBox(hInst, MAKEINTRESOURCE(IDD_SAVE_AS), hWnd, SaveAs);*/
            break;

        case ID_FIND:
            ZeroMemory(&ofn1, sizeof(ofn1));
            ofn1.lStructSize = sizeof(ofn1);
            ofn1.hwndOwner = hWnd;
            ofn1.wFindWhatLen = sizeof(szFindWhat);
            ofn1.wReplaceWithLen = sizeof(szReplaceWith);
            ofn1.Flags = FR_DOWN;
            ofn1.lpstrFindWhat = szFindWhat;
            ofn1.lpstrReplaceWith = szReplaceWith;
            if (FindText(&ofn1) != FALSE) {}
            break;
        }
    }
    break;
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);
        // TODO: Добавьте сюда любой код прорисовки, использующий HDC...
        EndPaint(hWnd, &ps);
    }
    break;
    case WM_TIMER: {
        if (wParam == 1 && !showMasag) {
            showMasag = TRUE;
            KillTimer(hWnd, 1);
            MessageBox(hWnd, L"Сработал таймер посе 10 секунд", L"Информация", MB_OK | MB_ICONINFORMATION);

        }
        break;
    case WM_DESTROY:
        if (!showMasag) {
            KillTimer(hWnd, 1);
        }
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
                 return 0;
    }
}

// Обработчик сообщений для окна "О программе".
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}
