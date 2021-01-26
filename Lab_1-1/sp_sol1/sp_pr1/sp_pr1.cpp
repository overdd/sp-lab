#include <Windows.h>
#include "sp_pr1.h"

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	LPSTR lpszCmdLine, int nCmdShow)
{

	//������ �������������� ����
	LPCTSTR lpszHelloText = MESSAGE_TEXT;
	LPCTSTR lpszHelloWndTitle = MESSAGE_TITLE;

	MessageBox(NULL, lpszHelloText, lpszHelloWndTitle, MB_OK);

	////////////////////////////////////////////////////////////////////////////////////////////////////////
	// ������ ����

	LPCTSTR lpszHelloRussText = MESSAGE_TEXT_0;
	LPCTSTR lpszRunTimeText = MESSAGE_TEXT_00;

	// �������� ����� � �����
	TCHAR	Buff1[200] = { 0 };
	lstrcpy(Buff1, lpszHelloRussText);

	// �������� ����� � ������� � �������� � �����
	SYSTEMTIME st;
	GetLocalTime(&st);

	TCHAR	Buff2[200] = { 0 };
	wsprintf(Buff2, lpszRunTimeText, st.wDay, st.wMonth, st.wYear, st.wHour, st.wMinute, st.wSecond);
	// ��������� ��� ������
	lstrcat(Buff1, Buff2);
	// ������� ��������� ����������
	MessageBox(NULL, Buff1, lpszHelloWndTitle, MB_OK | MB_ICONEXCLAMATION);
	////////////////////////////////////////////////////////////////////////////////////////////////////////
	// 4.2

	//int iRetValue1, iRetValue2;

	//LPCTSTR lpszMesBoxTitle = TEXT("������� MessageBox");

	//TCHAR lpszResponce[200] = { 0 };
	//do {
	//	iRetValue1 = MessageBox(NULL, TEXT("������� ���� �� ������."), lpszMesBoxTitle,
	//		MB_ABORTRETRYIGNORE | MB_ICONEXCLAMATION | MB_SETFOREGROUND);
	//	switch (iRetValue1)
	//	{
	//	case IDABORT:
	//		lstrcpy(lpszResponce, TEXT("������ ������ �������� (ABORT)"));
	//		break;
	//	case IDRETRY:
	//		lstrcpy(lpszResponce, TEXT("������ ������ ��������� (RETRY)"));
	//		break;
	//	case IDIGNORE:
	//		lstrcpy(lpszResponce, TEXT("������ ������ ���������� (IGNORE)"));
	//		break;
	//	default: lstrcpy(lpszResponce, TEXT("����� ��� �� �������."));
	//	}

	//	TCHAR buf[200] = TEXT("");
	//	lstrcat(buf, lpszResponce);
	//	lstrcat(buf, TEXT(" ���������� �������� ������������ ��������?"));
	//	iRetValue2 = MessageBox(NULL, buf, lpszMesBoxTitle,
	//		MB_YESNO | MB_ICONQUESTION | MB_SETFOREGROUND);
	//}

	//while (iRetValue2 != IDNO);

	//////////////////////////////////////////////////////////////////////////////////////////////////////
	//4.3 �������� ��������������� �������

	int iRetValue1, iRetValue2;

	LPCTSTR lpszMesBoxTitle = MESSAGE_TITLE_1;
	LPCTSTR lpszMesBoxText = MESSAGE_TEXT_1;

	TCHAR lpszResponce[200] = { 0 };
	do {
		iRetValue1 = MessageBox(NULL, lpszMesBoxText, lpszMesBoxTitle,
			MB_RETRYCANCEL | MB_ICONHAND | MB_SETFOREGROUND | MB_DEFBUTTON2);
		switch (iRetValue1)
		{
		case IDRETRY:
			lstrcpy(lpszResponce, TEXT("������ ������ ��������� (RETRY)"));
			break;
		case IDCANCEL:
			lstrcpy(lpszResponce, TEXT("������ ������ �������� (CANCEL)"));
			break;
		default: lstrcpy(lpszResponce, TEXT("����� ��� �� �������."));
		}

		TCHAR buf[200] = TEXT("");
		lstrcat(buf, lpszResponce);
		lstrcat(buf, TEXT(" ���������� �������� ������������ ��������?"));
		iRetValue2 = MessageBox(NULL, buf, lpszMesBoxTitle,
			MB_YESNO | MB_ICONQUESTION | MB_SETFOREGROUND | MB_DEFBUTTON2);
	}

	while (iRetValue2 != IDNO);
	return 0;
}