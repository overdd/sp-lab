#include <Windows.h>
#include <Windowsx.h>
#include <tchar.h>

//Прототипы методов (оконной процедуры)
LRESULT CALLBACK Pr2_WndProc(HWND, UINT, WPARAM, LPARAM);

//Глобальные объекты
LPCTSTR g_lpszClassName = TEXT("sp_pr2_class");
LPCTSTR g_lpszAplicationTitle = TEXT("Главное окно приложения. Программист Серко Антон");
LPCTSTR g_lpszDestroyMessage = TEXT("Поступило сообщение WM_DESTROY, из обработчика	которого и выполнен данный вывод. \nСообщение поступило в связи с разрушением окна приложения");
HINSTANCE g_hInst = nullptr;

//Точка входа в программу (приложение)
int APIENTRY _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	LPTSTR lpszCommanLine, int nCmdShow)
{
	g_hInst = hInstance;
	WNDCLASSEX wc;
	MSG msg;
	HWND hWnd;
	memset(&wc, 0, sizeof(WNDCLASSEX)); //ZeroMemory(&wc,sizeof(WNDCLASSEX);
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.lpszClassName = g_lpszClassName;
	wc.lpfnWndProc = Pr2_WndProc;
	wc.style = CS_VREDRAW | CS_HREDRAW;
	wc.hInstance = hInstance;
	wc.hIcon = LoadIcon(NULL, MAKEINTRESOURCE(IDI_QUESTION));
	wc.hCursor = LoadCursor(NULL, MAKEINTRESOURCE(IDC_ARROW));
	COLORREF  clr;
	clr = RGB(127, 127, 0);
	HBRUSH hbr; hbr = CreateSolidBrush(clr);
	wc.hbrBackground = hbr;
	wc.lpszMenuName = NULL;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;

	if (!RegisterClassEx(&wc))
	{
		MessageBox(NULL, TEXT("Ошибка регистрации класса окна!"),
			TEXT("Ошибка"), MB_OK | MB_ICONERROR);
		return FALSE;
	}

	hWnd = CreateWindowEx(NULL, g_lpszClassName, g_lpszAplicationTitle,
		(WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_THICKFRAME | WS_MAXIMIZEBOX), 50, 100, 500, 400,
		nullptr, nullptr, hInstance, nullptr);

	if (!hWnd) {
		MessageBox(NULL, TEXT("Окно не создано!"),
			TEXT("Ошибка"), MB_OK | MB_ICONERROR);
		return FALSE;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return msg.wParam;

}

LRESULT CALLBACK Pr2_WndProc(HWND hWnd, UINT msg,
	WPARAM wParam, LPARAM lParam)
{
	HDC hDC;
	static int xW;
	static int yH;

	static HWND hButtonSave;
	static HWND hButtonAdd;
	static HWND hButtonExit;
	static HWND hEdit;
	static HWND hListBox;

#define IDC_BTN_SAVE 150
#define IDC_BTN_ADD 151
#define IDC_EDIT1 152
#define IDC_LISTBOX 153

	static TCHAR pszTextBuff[500] = { 0 };

	switch (msg)
	{
	case WM_CREATE:
	{
		MessageBox(hWnd, TEXT("Выполняется обработка WM_CREATE"),
			TEXT("Вывод из WM_CREATE"), MB_OK);
		if (!(hEdit = CreateWindowEx(0L, TEXT("Edit"), TEXT("Какой-то текст"),
			WS_CHILD | WS_BORDER | WS_VISIBLE,
			20, 50, 160, 40, hWnd, (HMENU)(IDC_EDIT1),
			g_hInst, NULL))) return (-1);

		if (!(hListBox = CreateWindowEx(0L, TEXT("ListBox"), TEXT("Список"),
			WS_CHILD | WS_BORDER | WS_VISIBLE,
			200, 50, 160, 180, hWnd, (HMENU)(IDC_LISTBOX),
			g_hInst, NULL))) return (-1);

		if (!(hButtonSave = CreateWindowEx(0L, TEXT("Button"), TEXT("Сохранить"),
			WS_CHILD | WS_BORDER | WS_VISIBLE,
			20, 240, 80, 24, hWnd, (HMENU)(IDC_BTN_SAVE),
			g_hInst, NULL))) return (-1);
		if (!(hButtonAdd = CreateWindowEx(0L, TEXT("Button"), TEXT("В список"),
			WS_CHILD | WS_BORDER | WS_VISIBLE,
			120, 240, 80, 24, hWnd, (HMENU)(IDC_BTN_ADD),
			g_hInst, NULL))) return (-1);
		if (!(hButtonExit = CreateWindowEx(0L, TEXT("Button"), TEXT("Выход"),
			WS_CHILD | WS_BORDER | WS_VISIBLE,
			220, 240, 80, 24, hWnd, (HMENU)(IDCANCEL),
			g_hInst, NULL))) return (-1);

	}  return 0;

	case WM_COMMAND:
	{
		INT wmId = LOWORD(wParam);
		INT wmEvent = HIWORD(wParam);


		switch (wmId)
		{
		case IDCANCEL:
			DestroyWindow(hWnd);
			break;

		case IDC_BTN_SAVE:
		{
			int cch;
			cch = SendMessage(hEdit, WM_GETTEXT, (WPARAM)500,
				(LPARAM)pszTextBuff);
			if (cch == 0)
			{
				pszTextBuff[0] = '\0';
				MessageBox(hWnd, TEXT("Введите текст"), TEXT("Читаем Edit"), MB_OK);
			}
			else {
				TCHAR Buff1[500] = { 0 };
				SYSTEMTIME st; GetSystemTime(&st);
				wsprintf(Buff1, TEXT("Время по Гринвичу: %d ч %d мин %d сек\n"),
					st.wHour, st.wMinute, st.wSecond);
				lstrcat(Buff1, TEXT("Текст в памяти: "));
				lstrcat(Buff1, pszTextBuff);
				MessageBox(hWnd, Buff1, TEXT("Содержимое буфера"), MB_OK);
			}
		}
		break;

		case IDC_BTN_ADD:
		{ int ind;
		ind = SendMessage(hListBox, LB_ADDSTRING,
			(WPARAM)0, (LPARAM)pszTextBuff);
		if (ind == LB_ERR)
			MessageBox(hWnd, TEXT("Ошибка в списке"),
				TEXT(""), MB_OK);
		}
		break;

		default:
			return DefWindowProc(hWnd, msg, wParam, lParam);
		}
	}
	break;

	case WM_SIZE:
	{
		xW = LOWORD(lParam);
		yH = (INT)(SHORT)HIWORD(lParam);
	}break;

	case WM_LBUTTONDOWN:
	{
		HDC hdc;
		hdc = GetDC(hWnd);
		LPCTSTR lpszTxt1;
		lpszTxt1 = TEXT("Обработка сообщения WM_LBUTTONDOWN, \n которое посылается в окно при щелчке левой кнопки мыши");

		int x = GET_X_LPARAM(lParam);
		int y = GET_Y_LPARAM(lParam);
		TextOut(hdc, x, y, lpszTxt1, lstrlen(lpszTxt1));
		x += 10;
		y += 20;

		RECT rc;
		GetClientRect(hWnd, &rc);
		int xc, yc;
		xc = (rc.right - rc.left) / 2;
		yc = (rc.bottom - rc.top) / 2;
		TextOut(hdc, xc, yc, lpszTxt1, lstrlen(lpszTxt1));

		int  xc1, yc1;
		xc1 = (xW / 2) + 10;
		yc1 = (yH / 2) + 20;
		TextOut(hdc, xc1, yc1, lpszTxt1, lstrlen(lpszTxt1));

		ReleaseDC(hWnd, hdc);
	}break;


	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		hDC = BeginPaint(hWnd, &ps);
		LPCTSTR lpszTxt;
		lpszTxt = TEXT("Обработка сообщения WM_PAINT. Это соообщение окно получает после того, \n как оно было закрыто другим окном и затем открыто.");
		TextOut(hDC, 20, 100, lpszTxt, lstrlen(lpszTxt)); // Вывод в контекст
		EndPaint(hWnd, &ps);
	}
	break;
	case WM_DESTROY:
	{
		MessageBox(hWnd, g_lpszDestroyMessage, TEXT("Из WM_DESTROY"), MB_OK);
		PostQuitMessage(5);
	}break;


	default:
		return(DefWindowProc(hWnd, msg, wParam, lParam));
	}
	return 0;// Для ветвей с "break"
}
