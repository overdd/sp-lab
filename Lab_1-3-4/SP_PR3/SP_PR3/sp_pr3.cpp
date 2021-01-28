#include <windows.h>
#include <Windowsx.h>
#include <winuser.h>
#include <tchar.h>
#include "sp_pr3.h"
#include "resource.h"

//Прототипы методов (оконной процедуры)

LRESULT CALLBACK Pr2_WndProc(HWND, UINT, WPARAM, LPARAM);

//Глобальные объекты
LPCTSTR g_lpszClassName = MESSAGE_ClassName;
LPCTSTR g_lpszAplicationTitle = MESSAGE_AplicationTitle;
LPCTSTR g_lpszDestroyMessage = MESSAGE_DestroyMessage;
HINSTANCE g_hInst = nullptr;
HMENU hMenu = nullptr;
void Pr2_WndProc_OnDestroy(HWND);
void Pr2_WndProc_OnCommand(HWND, int, HWND, UINT);


//Точка входа в программу (приложение)
int APIENTRY _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	LPTSTR lpszCommanLine, int nCmdShow)
{
	g_hInst = hInstance;
	HACCEL hAccel;  // подключаем акселератор
	hAccel = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDR_ACCELERATOR1));

	WNDCLASSEX wc;
	MSG msg;
	HWND hWnd;
	memset(&wc, 0, sizeof(WNDCLASSEX)); //ZeroMemory(&wc,sizeof(WNDCLASSEX);
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.lpszClassName = g_lpszClassName;
	wc.lpfnWndProc = Pr2_WndProc;
	wc.style = CS_VREDRAW | CS_HREDRAW;
	wc.hInstance = hInstance;
	wc.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1));
	wc.hCursor = LoadCursor(hInstance, MAKEINTRESOURCE(IDC_POINTER));
	COLORREF  clr;
	clr = RGB(127, 127, 0);
	HBRUSH hbr; hbr = CreateSolidBrush(clr);
	wc.hbrBackground = hbr;
	wc.lpszMenuName = MAKEINTRESOURCE(IDR_MENU1);
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;

	if (!RegisterClassEx(&wc))
	{
		MessageBox(NULL, TEXT("Ошибка регистрации класса окна!"),
			TEXT("Ошибка"), MB_OK | MB_ICONERROR);
		return FALSE;
	}

	hMenu = LoadMenu(hInstance, MAKEINTRESOURCE(IDR_MENU1)); // дескриптор меню

	hWnd = CreateWindowEx(NULL, g_lpszClassName, g_lpszAplicationTitle, // имя класса окна, заголовок окна
		(WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_THICKFRAME | WS_MAXIMIZEBOX), //стиль окна
		50,
		100,
		500,
		400, // размеры
		nullptr, // дескриптор родительского окна
		hMenu /*nullptr*/, // нет меню
		hInstance, //дескриптор приложения
		nullptr); // доп информации нет

	if (!hWnd) {
		MessageBox(NULL, TEXT("Окно не создано!"),
			TEXT("Ошибка"), MB_OK | MB_ICONERROR);
		return FALSE;
	}

	ShowWindow(hWnd, nCmdShow); // Показать окно
	UpdateWindow(hWnd);

	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!TranslateAccelerator(hWnd, hAccel, &msg))
		{
			TranslateMessage(&msg); // функция трансляции кодов нажатой кнопки
			DispatchMessage(&msg); // посылает сообщения функции WndProc()
		}
	}
	return  msg.wParam;
}

BOOL CreateMenuItem(HMENU hMenu, TCHAR* str, UINT uIns, UINT uCom, HMENU hSubMenu, BOOL flag, UINT fType)
{
	MENUITEMINFO mii;
	mii.cbSize = sizeof(MENUITEMINFO);
	mii.fMask = MIIM_STATE | MIIM_TYPE | MIIM_SUBMENU | MIIM_ID;
	mii.fType = fType;
	mii.fState = MFS_ENABLED;
	mii.dwTypeData = str;
	mii.cch = sizeof(str);
	mii.wID = uCom;
	mii.hSubMenu = hSubMenu;
	return InsertMenuItem(hMenu, uIns, flag, &mii);
}


LRESULT CALLBACK Pr2_WndProc(HWND hWnd, UINT msg,
	WPARAM wParam, LPARAM lParam)
{
	HDC hDC;
	static int xW;
	static int yH;
	HMENU hMenu;
	static HWND hButtonSave;
	static HWND hButtonAdd;
	static HWND hButtonExit;
	static HWND hEdit;
	static HWND hListBox;
	static UINT nFlag = MF_ENABLED;

#define IDC_BTN_SAVE 150
#define IDC_BTN_ADD 151
#define IDC_EDIT1 152
#define IDC_LISTBOX 153
#define IDM_FILE_CLOSE 154

	static TCHAR pszTextBuff[500] = { 0 };

	switch (msg)
	{

	case WM_CREATE:
	{
		MessageBox(hWnd, TEXT("Выполняется обработка WM_CREATE"),
			TEXT("Вывод из WM_CREATE"), MB_OK);
		if (!(hEdit = CreateWindowEx(0L, TEXT("Edit"), TEXT("Редактор"),
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

		hMenu = GetMenu(hWnd);
		HMENU hFile = GetSubMenu(hMenu, 0);
		InsertMenu(hFile, 2, MF_BYPOSITION | MF_STRING, IDM_FILE_CLOSE, TEXT("Закрыть документ"));
	}  return 0;//return WM_CREATE

	case WM_COMMAND:
	{
		INT wmId = LOWORD(wParam);
		INT wmEvent = HIWORD(wParam);

		switch (wmId)
		{

		case IDM_FILE_NEW:
		{
			MessageBox(hWnd, TEXT("Выбрана команда \"Создать\""),
				TEXT("Из WM_COMMAND"), MB_OK);
			hMenu = GetMenu(hWnd);
			EnableMenuItem(hMenu, IDM_EDIT_SELECT, MF_BYCOMMAND | MF_ENABLED);
		} break;

		case IDM_FILE_OPEN:
		{
			MessageBox(hWnd, TEXT("Выбрана команда \"Открыть\""),
				TEXT("Из WM_COMMAND"), MB_OK);
		} break;


		case IDM_EDIT_SELECT:
		{
			MessageBox(hWnd, TEXT("Выбрана команда \"Выделить\""),
				TEXT("Из WM_COMMAND"), MB_OK);
			hMenu = GetMenu(hWnd);
			EnableMenuItem(hMenu, IDM_EDIT_COPY, MF_BYCOMMAND | MF_ENABLED);
		} break;

		case IDM_EDIT_COPY:
		{
			MessageBox(hWnd, TEXT("Выбрана команда \"Копировать\""),
				TEXT("Из WM_COMMAND"), MB_OK);
		} break;

		case IDM_EDIT_CUT:
		{
			MessageBox(hWnd, TEXT("Выбрана команда \"Вырезать\""),
				TEXT("Из WM_COMMAND"), MB_OK);
		} break;

		case IDM_EDIT_INSERT:
		{
			MessageBox(hWnd, TEXT("Выбрана команда \"Вставить\""),
				TEXT("Из WM_COMMAND"), MB_OK);
		} break;

		case IDM_REFERENCE_HELP:
		{
			MessageBox(hWnd, TEXT("Выбрана команда \"Помощь\""),
				TEXT("Из WM_COMMAND"), MB_OK);
		} break;

		case IDM_REFERENCE_ABOUT:
		{
			MessageBox(hWnd, TEXT("Выбрана команда \"О программе\""),
				TEXT("Из WM_COMMAND"), MB_OK);
		} break;

		case IDCANCEL:
		{
			DestroyWindow(hWnd);
		}
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
				wsprintf(Buff1, TEXT("Время по Гринвичу : %d ч %d мин %d сек\n"),
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

		case IDM_FILE_CLOSE:
		{
			hMenu = GetMenu(hWnd);
			EnableMenuItem(hMenu, IDM_EDIT_SELECT, MF_BYCOMMAND | MF_GRAYED);
			EnableMenuItem(hMenu, IDM_EDIT_COPY, MF_BYCOMMAND | MF_GRAYED);
			EnableMenuItem(hMenu, IDM_EDIT_CUT, MF_BYCOMMAND | MF_GRAYED);
			EnableMenuItem(hMenu, IDM_EDIT_INSERT, MF_BYCOMMAND | MF_GRAYED);
		}

		default: //обработка по умолчанию для WM_COMMAND
			return DefWindowProc(hWnd, msg, wParam, lParam);
		} // end switch(wmId)
	} // end WM_COMMAND
	break;

	case WM_SIZE:
	{
		xW = LOWORD(lParam);
		yH = (INT)(SHORT)HIWORD(lParam);
	}break;

	case WM_RBUTTONDOWN:
	{
		//Берем экранные координаты курсора мыши 
		DWORD xyPos = GetMessagePos();
		WORD xPos = LOWORD(xyPos),
			yPos = HIWORD(xyPos);
		MENUITEMINFO mii;

		int i = 0;
		hMenu = GetMenu(hWnd);
		HMENU hPopMenu = GetSubMenu(hMenu, 1);
		GetMenuItemInfo(hPopMenu, IDM_EDIT_SELECT | IDM_EDIT_COPY, FALSE, &mii);
		TrackPopupMenu(hPopMenu, TPM_LEFTALIGN | TPM_LEFTBUTTON | TPM_TOPALIGN, xPos, yPos, 0, hWnd, NULL);

	}break;
	case WM_LBUTTONDOWN:
	{
		HDC hdc;
		hdc = GetDC(hWnd);
		LPCTSTR lpszTxt1;
		lpszTxt1 = TEXT("Обработка сообщения WM_LBUTTONDOWN,"
			"которое посылается в окно при щелчке левой кнопки мыши");
		int x = GET_X_LPARAM(lParam);
		int y = GET_Y_LPARAM(lParam);
		TextOut(hdc, x, y, lpszTxt1, lstrlen(lpszTxt1)); // Вывод в контекст
														 // Вывод с использованием DrawText
		x = x + 10;
		y += 20;

		RECT rc;
		GetClientRect(hWnd, &rc);
		int xc, yc;
		xc = (rc.right - rc.left) / 2;
		yc = (rc.bottom - rc.top) / 2;
		TextOut(hdc, xc, yc, lpszTxt1, lstrlen(lpszTxt1));

		int  xc1, yc1;
		xc1 = (xW / 2) + 10;// из WM_SIZE
		yc1 = (yH / 2) + 20;
		TextOut(hdc, xc1, yc1, lpszTxt1, lstrlen(lpszTxt1));

		ReleaseDC(hWnd, hdc);
	}break;

	case WM_PAINT:    // Вывод при обновлении окна
	{
		PAINTSTRUCT ps;
		hDC = BeginPaint(hWnd, &ps); // Получение контекста для
									  // обновления окна
		LPCTSTR lpszTxt;
		lpszTxt = TEXT("Обработка сообщения WM_PAINT. Это соообщение окно получает после того, "
			"как оно было закрыто другим окном и затем открыто.");
		TextOut(hDC, 20, 100, lpszTxt, lstrlen(lpszTxt)); // Вывод в контекст
		EndPaint(hWnd, &ps); // Завершение обновления окна
	}
	break;

	case WM_DESTROY:  // Завершение работы приложения
	{
		MessageBox(hWnd, g_lpszDestroyMessage, TEXT("Из WM_DESTROY"), MB_OK);

		PostQuitMessage(5);//Завершение программы с кодом 5
	}break;
	case WM_MENUSELECT:
	{HDC hdc1;
	LPTSTR lpszMsgSpace;
	TCHAR Buf[300];
	HINSTANCE hInst;
	hInst = (HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE);
	int size;
	size = LoadString(hInst, LOWORD(wParam), Buf, 300);
	hdc1 = GetDC(hWnd);
	RECT rc;
	GetClientRect(hWnd, &rc);

	Rectangle(hdc1, rc.left, rc.bottom - 35, rc.right, rc.bottom);
	TextOut(hdc1, rc.left + 10, rc.bottom - 30, Buf, lstrlen(Buf));

	ReleaseDC(hWnd, hdc1);
	break;
	}

	default: // Вызов "Обработчика по умолчанию"
		return(DefWindowProc(hWnd, msg, wParam, lParam));
	}// switch
	return 0;// Для ветвей с "break"
}
void Pr2_WndProc_OnDestroy(HWND hWnd)
{
	MessageBox(hWnd, g_lpszDestroyMessage, TEXT("Из WM_DESTROY"), MB_OK);
	PostQuitMessage(5);//Завершение программы с кодом 5
}

void Pr2_WndProc_OnCommand(HWND hWnd, int id, HWND hwndCtl, UINT codeNotify)
{
	static HWND hEdit;
	static HWND hListBox;
	static TCHAR pszTextBuff[500] = { 0 };

	switch (id)
	{
	case IDM_FILE_NEW:
	{
		MessageBox(hWnd, TEXT("Выбрана команда \"Создать\""),
			TEXT("Из WM_COMMAND"), MB_OK);
		hMenu = GetMenu(hWnd);
		EnableMenuItem(hMenu, IDM_EDIT_SELECT, MF_BYCOMMAND | MF_ENABLED);
	} break;

	case IDM_FILE_OPEN:
	{
		MessageBox(hWnd, TEXT("Выбрана команда \"Открыть\""),
			TEXT("Из WM_COMMAND"), MB_OK);
	} break;


	case IDM_EDIT_SELECT:
	{
		MessageBox(hWnd, TEXT("Выбрана команда \"Выделить\""),
			TEXT("Из WM_COMMAND"), MB_OK);
		hMenu = GetMenu(hWnd);
		EnableMenuItem(hMenu, IDM_EDIT_COPY, MF_BYCOMMAND | MF_ENABLED);
	} break;

	case IDM_EDIT_COPY:
	{
		MessageBox(hWnd, TEXT("Выбрана команда \"Копировать\""),
			TEXT("Из WM_COMMAND"), MB_OK);
	} break;

	case IDM_EDIT_CUT:
	{
		MessageBox(hWnd, TEXT("Выбрана команда \"Вырезать\""),
			TEXT("Из WM_COMMAND"), MB_OK);
	} break;

	case IDM_EDIT_INSERT:
	{
		MessageBox(hWnd, TEXT("Выбрана команда \"Вставить\""),
			TEXT("Из WM_COMMAND"), MB_OK);
	} break;

	case IDM_REFERENCE_HELP:
	{
		MessageBox(hWnd, TEXT("Выбрана команда \"Помощь\""),
			TEXT("Из WM_COMMAND"), MB_OK);
	} break;

	case IDM_REFERENCE_ABOUT:
	{
		MessageBox(hWnd, TEXT("Выбрана команда \"О программе\""),
			TEXT("Из WM_COMMAND"), MB_OK);
	} break;

	case IDCANCEL:
	{
		DestroyWindow(hWnd);
	}
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

	case IDM_FILE_CLOSE:
	{
		hMenu = GetMenu(hWnd);
		EnableMenuItem(hMenu, IDM_EDIT_SELECT, MF_BYCOMMAND | MF_GRAYED);
		EnableMenuItem(hMenu, IDM_EDIT_COPY, MF_BYCOMMAND | MF_GRAYED);
		EnableMenuItem(hMenu, IDM_EDIT_CUT, MF_BYCOMMAND | MF_GRAYED);
		EnableMenuItem(hMenu, IDM_EDIT_INSERT, MF_BYCOMMAND | MF_GRAYED);
	}

	default:
		FORWARD_WM_COMMAND(hWnd, id, hwndCtl, codeNotify, DefWindowProc);
	}
}
