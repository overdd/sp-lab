#include <Windows.h>
#include "sp_pr1.h"

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	LPSTR lpszCmdLine, int nCmdShow)
{

	//первое приветственное ОКНО
	LPCTSTR lpszHelloText = MESSAGE_TEXT;
	LPCTSTR lpszHelloWndTitle = MESSAGE_TITLE;

	MessageBox(NULL, lpszHelloText, lpszHelloWndTitle, MB_OK);

	////////////////////////////////////////////////////////////////////////////////////////////////////////
	// второе ОКНО

	LPCTSTR lpszHelloRussText = MESSAGE_TEXT_0;
	LPCTSTR lpszRunTimeText = MESSAGE_TEXT_00;

	// помещаем текст в буфер
	TCHAR	Buff1[200] = { 0 };
	lstrcpy(Buff1, lpszHelloRussText);

	// приводим время к формату и помещаем в буфер
	SYSTEMTIME st;
	GetLocalTime(&st);

	TCHAR	Buff2[200] = { 0 };
	wsprintf(Buff2, lpszRunTimeText, st.wDay, st.wMonth, st.wYear, st.wHour, st.wMinute, st.wSecond);
	// склеиваем две строки
	lstrcat(Buff1, Buff2);
	// выводим результат склеивания
	MessageBox(NULL, Buff1, lpszHelloWndTitle, MB_OK | MB_ICONEXCLAMATION);
	////////////////////////////////////////////////////////////////////////////////////////////////////////
	// 4.2

	//int iRetValue1, iRetValue2;

	//LPCTSTR lpszMesBoxTitle = TEXT("Изучаем MessageBox");

	//TCHAR lpszResponce[200] = { 0 };
	//do {
	//	iRetValue1 = MessageBox(NULL, TEXT("Нажмите одну из кнопок."), lpszMesBoxTitle,
	//		MB_ABORTRETRYIGNORE | MB_ICONEXCLAMATION | MB_SETFOREGROUND);
	//	switch (iRetValue1)
	//	{
	//	case IDABORT:
	//		lstrcpy(lpszResponce, TEXT("Нажата кнопка Прервать (ABORT)"));
	//		break;
	//	case IDRETRY:
	//		lstrcpy(lpszResponce, TEXT("Нажата кнопка Повторить (RETRY)"));
	//		break;
	//	case IDIGNORE:
	//		lstrcpy(lpszResponce, TEXT("Нажата кнопка Пропустить (IGNORE)"));
	//		break;
	//	default: lstrcpy(lpszResponce, TEXT("Ответ мне не понятен."));
	//	}

	//	TCHAR buf[200] = TEXT("");
	//	lstrcat(buf, lpszResponce);
	//	lstrcat(buf, TEXT(" Продолжить изучение возвращаемых значений?"));
	//	iRetValue2 = MessageBox(NULL, buf, lpszMesBoxTitle,
	//		MB_YESNO | MB_ICONQUESTION | MB_SETFOREGROUND);
	//}

	//while (iRetValue2 != IDNO);

	//////////////////////////////////////////////////////////////////////////////////////////////////////
	//4.3 Согласно индивидуального задания

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
			lstrcpy(lpszResponce, TEXT("Нажата кнопка Повторить (RETRY)"));
			break;
		case IDCANCEL:
			lstrcpy(lpszResponce, TEXT("Нажата кнопка Отменить (CANCEL)"));
			break;
		default: lstrcpy(lpszResponce, TEXT("Ответ мне не понятен."));
		}

		TCHAR buf[200] = TEXT("");
		lstrcat(buf, lpszResponce);
		lstrcat(buf, TEXT(" Продолжить изучение возвращаемых значений?"));
		iRetValue2 = MessageBox(NULL, buf, lpszMesBoxTitle,
			MB_YESNO | MB_ICONQUESTION | MB_SETFOREGROUND | MB_DEFBUTTON2);
	}

	while (iRetValue2 != IDNO);
	return 0;
}