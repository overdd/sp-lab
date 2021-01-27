#include <Windows.h>
#include <Windowsx.h>
#include <tchar.h>
//-- Prototypes -------------------

LRESULT CALLBACK SimWndProc(HWND, UINT, WPARAM, LPARAM);

//-- Global Variables ------------


//  ��������� �������

int APIENTRY _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,

	LPTSTR lpszCmdLine, int nCmdShow)
{

	WNDCLASSEX wc;
	MSG msg;
	HWND hWnd;

	memset(&wc, 0, sizeof(WNDCLASSEX));
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.lpszClassName = TEXT("SimpleClassName");
	wc.lpfnWndProc = SimWndProc;
	wc.style = CS_VREDRAW | CS_HREDRAW;
	wc.hInstance = hInstance;
	wc.hIcon = LoadIcon(NULL, MAKEINTRESOURCE(IDI_ASTERISK));
	wc.hCursor = LoadCursor(NULL, MAKEINTRESOURCE(IDC_HAND));
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 2);
	wc.lpszMenuName = NULL;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;

	if (!RegisterClassEx(&wc))

	{
		MessageBox(NULL, TEXT("������ ����������� ������ ����!"),
			TEXT("������"), MB_OK | MB_ICONERROR);
		return FALSE;

	}

	hWnd = CreateWindowEx(NULL, TEXT("SimpleClassName"),
		TEXT("Simple Application with Message handling"),
		WS_OVERLAPPEDWINDOW,
		250,
		250,
		300,
		300,
		NULL,
		NULL,
		hInstance,
		NULL
	);

	if (!hWnd)
	{
		MessageBox(NULL, TEXT("���� �� �������!"),
			TEXT("������"), MB_OK | MB_ICONERROR);
		return FALSE;
	}
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);
	while (GetMessage(&msg, NULL, 0, 0))
	{
		DispatchMessage(&msg);
	}
	return msg.wParam;
}

// ������� ���������
LRESULT CALLBACK SimWndProc(HWND hWnd, UINT msg,

	WPARAM wParam, LPARAM lParam)
{
	HDC hDC;

	switch (msg)
	{
	case WM_PAINT:    // ����� ��� ���������� ����
		PAINTSTRUCT ps;
		hDC = BeginPaint(hWnd, &ps); // ��������� ��������� ���
									 // ���������� ���� 
		TextOut(hDC, 10, 10, TEXT("Hello, World!"), 13); // ����� � ��������
		EndPaint(hWnd, &ps); // ���������� ���������� ����
		break;

	case WM_DESTROY:  // ���������� ������ ����������
		PostQuitMessage(0); // ������� WM_QUIT ����������
		break;

	default: // ����� "����������� �� ���������"

		return(DefWindowProc(hWnd, msg, wParam, lParam));
	}
	return FALSE;// ��� ������ � "break"
}
