/* Windows interaction module */

#include "win.h"
#include "anim.h"

VOID WinInit( HINSTANCE hInstance )
{

// Заполняем структуру класса окна
  WNDCLASSEX wc; // Создаём экземпляр, для обращения к членам класса WNDCLASSEX
  wc.cbSize = sizeof(WNDCLASSEX); // Размер структуры (в байтах)
  wc.style = CS_HREDRAW | CS_VREDRAW; // Стиль класса окошка
  wc.cbClsExtra = 0; // Число освобождаемых байтов при создании экземпляра приложения
  wc.cbWndExtra = sizeof(VOID *); // Число освобождаемых байтов в конце структуры
  wc.hbrBackground = (HBRUSH)COLOR_WINDOW; // дескриптор кисти для закраски фона окна
  wc.hCursor = LoadCursor(hInstance, (LPCWSTR)IDC_ARROW); // дескриптор курсора
  wc.hIconSm = (HICON)LoadImage(hInstance, (LPCWSTR)IDI_ERROR, IMAGE_ICON,
    GetSystemMetrics(SM_CXSMICON), GetSystemMetrics(SM_CYSMICON), 0); // Дескриптор маленькой пиктограммы (значка)
  wc.hIcon = (HICON)LoadImage(hInstance, (LPCWSTR)IDI_ERROR, IMAGE_ICON,
    GetSystemMetrics(SM_CXICON), GetSystemMetrics(SM_CYICON), 0); // Декриптор пиктограммы
  wc.lpszMenuName = NULL; // Указатель на имя меню
  wc.hInstance = hInstance; // Указатель на строку, содержащую имя меню, применяемого для класса
  wc.lpfnWndProc = WinFunc; // Указатель на пользовательскую функцию
  wc.lpszClassName = WND_CLASS_NAME; // Указатель на имя класса

  // Регистрируем класс окна
  if (!RegisterClassEx(&wc))
  {
	  // в случае отсутствия регистрации класса:
		MessageBox(NULL, L"Error registering window class", L"ERROR",
		  MB_OK | MB_ICONSTOP);
		return;
  }

  /* Window creation */
  Anim.hWnd = CreateWindow( WND_CLASS_NAME, //Имя класса
							L"difference", // Имя окна (то что сверху)
							WS_OVERLAPPED | WS_MINIMIZEBOX | WS_SYSMENU, // Режимы отображения окошка
							CW_USEDEFAULT, // Положение окна по оси х
						    CW_USEDEFAULT, // Позиция окна по оси у
						    CW_USEDEFAULT, // Ширина окна
						    CW_USEDEFAULT, // Высота окна
							NULL, // Дескриптор родительского окна 
						    NULL, // Дескриптор меню 
						    hInstance, // Дескриптор окна 
						    NULL ); // Указатель на данные, передаваемые из пользовательской функции

  if (Anim.hWnd == NULL)
  {
		MessageBox(NULL, L"Error creating window", L"ERROR",
		  MB_OK | MB_ICONSTOP);
    return;
  }

  /* Show and update window */
  ShowWindow(Anim.hWnd, SW_SHOWMAXIMIZED); 
  //ShowWindow(Anim.hWnd, SW_SHOWNORMAL);
  //UpdateWindow(Anim.hWnd);
  //SetWindowPos(Anim.hWnd, 0, 0, 0, 1366, 768, 0);

}

/*ОС Windows и различные функции наших программ ежесекундно отправляют тысячи сообщений каждому отдельному приложению.
Эти сообщения могут быть отправлены из-за нажатия клавиши или нажатия кнопки на мыши и т.д. Для этих случаев у нас
существует структура Msg, которая хранит информацию об этих сообщениях. В WinFunc() есть условия
выбора для этих сообщений. Если ОС отправляет сообщение WM_PAINT, например, то в окне  должно что-то рисоваться.
Эти сообщения обрабатываются условием switch(), параметрами которого являются Msg. Msg мы создали при описании нашей функции
WinFunc(). Основной значение в этом операторе, без которых окно не будет обновляться после сворачивания и не 
закроется: WM_DESTROY. Также есть WM_PAINT, который нужен для прорисовки в клиентской области. WM – от слов Window Message*/

LRESULT CALLBACK WinFunc( HWND hWnd, // Дескриптор окна
						  UINT Msg, // Сообщение, посылаемое ОС
						  WPARAM wParam, // Параметры
						  LPARAM lParam) // сообщений, для последующего обращения
{
  switch (Msg)
  {
	  case WM_CREATE:
		SetWindowLong(hWnd, 0, (LONG)((CREATESTRUCT *)lParam)->lpCreateParams);
		Anim.hWnd = hWnd;
		Anim.hDC = GetDC(hWnd);
		Anim.Frame = ImageCreateEmpty(Anim.hDC, 1, 1);
	  default:
		switch (Msg)
		{
			case WM_CREATE:
			  return WinOnCreate((CREATESTRUCT *)lParam) ? 0 : -1; 
			case WM_DESTROY:
			  WinOnDestroy(); 
			  return 0;
			case WM_SIZE:
			  WinOnSize((UINT)wParam,
				(INT)(SHORT)LOWORD(lParam), (INT)(SHORT)HIWORD(lParam));
			  return 0;
			case WM_ACTIVATE:
			  WinOnActivate((UINT)LOWORD(wParam), (HWND)(lParam),
				(BOOL)HIWORD(wParam));
			  return 0;
			case WM_ERASEBKGND:
			  return (LRESULT)WinOnEraseBkgnd((HDC)wParam);
			case WM_PAINT:
			  WinOnPaint();
			  return 0;
			case WM_TIMER:
			  WinOnTimer((UINT)wParam);
			  return 0;
			case WM_MOUSEWHEEL:
			  WinOnMouseWheel((INT)(SHORT)LOWORD(lParam), (INT)(SHORT)HIWORD(lParam),
				(INT)(SHORT)HIWORD(wParam), (UINT)(SHORT)LOWORD(wParam));
			  return 0;
			case WM_MOUSEMOVE:
			  WinOnMouseMove((INT)(SHORT)LOWORD(lParam),
				(INT)(SHORT)HIWORD(lParam), (UINT)(SHORT)LOWORD(wParam));
			  return 0;
			case WM_LBUTTONDOWN:
			  WinOnButtonDown(FALSE, (INT)(SHORT)LOWORD(lParam),
				(INT)(SHORT)HIWORD(lParam), (UINT)(SHORT)LOWORD(wParam));
			  return 0;
			case WM_LBUTTONUP:
			  WinOnButtonUp((INT)(SHORT)LOWORD(lParam),
				(INT)(SHORT)HIWORD(lParam), (UINT)(SHORT)LOWORD(wParam));
			  return 0;
		}
		return DefWindowProc(hWnd, Msg, wParam, lParam);
    
  }
  return DefWindowProc(hWnd, Msg, wParam, lParam);
}

BOOL WinOnCreate( CREATESTRUCT *CS )
{
  return TRUE;
}

VOID WinOnDestroy( VOID )
{
  PostQuitMessage(0);
}

VOID WinOnSize( UINT State, INT W, INT H )
{
  AnimResize(W, H); 
} 

BOOL WinOnEraseBkgnd( HDC hDC )
{
  return FALSE;
}

VOID WinOnPaint( VOID )
{
  HDC hDC;
  PAINTSTRUCT ps;

  AnimResponse(); //
  hDC = BeginPaint(Anim.hWnd, &ps);
  AnimRender(hDC); //
  EndPaint(Anim.hWnd, &ps);
} 

VOID WinOnActivate( UINT ReasWinOn, HWND hWndActDeact, BOOL IsMinimized )
{
}

VOID WinOnTimer( INT Id ) 
{
  RECT r = {0, 0, Anim.W, Anim.H}; //Берет прямоугольник равный размерам окна и показываешь какую часть окна нужно перерисовать
  InvalidateRect(Anim.hWnd, &r, FALSE); //Отсюда WinOnPaint
}

VOID WinOnMouseWheel( INT X, INT Y, INT Z, UINT Keys )
{
}

VOID WinOnMouseMove( INT X, INT Y, UINT Keys )
{
}

VOID WinOnButtonDown( BOOL IsDoubleClick, INT X, INT Y, UINT Keys )
{
  SetCapture(Anim.hWnd);
}

VOID WinOnButtonUp( INT X, INT Y, UINT Keys )
{
  SetCapture(NULL);
}

VOID WinRunGameLoop( VOID )
{
  MSG msg;

  while(TRUE)
  {
    if (PeekMessage(&msg, NULL, 0, 0, TRUE))//Она проверяет есть ли в очереди сообщение и программа выполняется дальше
    {
      if (msg.message == WM_QUIT)
        return;

      TranslateMessage(&msg);//Переводит сообщения формата виртуальных клавиш в сообщения символы
      DispatchMessage(&msg);//Отправляет сообщение окну
    }
    else
      SendMessage(Anim.hWnd, WM_TIMER, 0, 0);//Вызывает оконную процедуру для заданного окна и не возвращает значение до тех пор, пока оконная процедура не обработает сообщение
  }
}

VOID WinDoExit( VOID )
{
  PostQuitMessage(0);
}
