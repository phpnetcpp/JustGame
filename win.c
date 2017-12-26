/* Windows interaction module */

#include "win.h"
#include "anim.h"

VOID WinInit( HINSTANCE hInstance )
{

// ��������� ��������� ������ ����
  WNDCLASSEX wc; // ������ ���������, ��� ��������� � ������ ������ WNDCLASSEX
  wc.cbSize = sizeof(WNDCLASSEX); // ������ ��������� (� ������)
  wc.style = CS_HREDRAW | CS_VREDRAW; // ����� ������ ������
  wc.cbClsExtra = 0; // ����� ������������� ������ ��� �������� ���������� ����������
  wc.cbWndExtra = sizeof(VOID *); // ����� ������������� ������ � ����� ���������
  wc.hbrBackground = (HBRUSH)COLOR_WINDOW; // ���������� ����� ��� �������� ���� ����
  wc.hCursor = LoadCursor(hInstance, (LPCWSTR)IDC_ARROW); // ���������� �������
  wc.hIconSm = (HICON)LoadImage(hInstance, (LPCWSTR)IDI_ERROR, IMAGE_ICON,
    GetSystemMetrics(SM_CXSMICON), GetSystemMetrics(SM_CYSMICON), 0); // ���������� ��������� ����������� (������)
  wc.hIcon = (HICON)LoadImage(hInstance, (LPCWSTR)IDI_ERROR, IMAGE_ICON,
    GetSystemMetrics(SM_CXICON), GetSystemMetrics(SM_CYICON), 0); // ��������� �����������
  wc.lpszMenuName = NULL; // ��������� �� ��� ����
  wc.hInstance = hInstance; // ��������� �� ������, ���������� ��� ����, ������������ ��� ������
  wc.lpfnWndProc = WinFunc; // ��������� �� ���������������� �������
  wc.lpszClassName = WND_CLASS_NAME; // ��������� �� ��� ������

  // ������������ ����� ����
  if (!RegisterClassEx(&wc))
  {
	  // � ������ ���������� ����������� ������:
		MessageBox(NULL, L"Error registering window class", L"ERROR",
		  MB_OK | MB_ICONSTOP);
		return;
  }

  /* Window creation */
  Anim.hWnd = CreateWindow( WND_CLASS_NAME, //��� ������
							L"difference", // ��� ���� (�� ��� ������)
							WS_OVERLAPPED | WS_MINIMIZEBOX | WS_SYSMENU, // ������ ����������� ������
							CW_USEDEFAULT, // ��������� ���� �� ��� �
						    CW_USEDEFAULT, // ������� ���� �� ��� �
						    CW_USEDEFAULT, // ������ ����
						    CW_USEDEFAULT, // ������ ����
							NULL, // ���������� ������������� ���� 
						    NULL, // ���������� ���� 
						    hInstance, // ���������� ���� 
						    NULL ); // ��������� �� ������, ������������ �� ���������������� �������

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

/*�� Windows � ��������� ������� ����� �������� ����������� ���������� ������ ��������� ������� ���������� ����������.
��� ��������� ����� ���� ���������� ��-�� ������� ������� ��� ������� ������ �� ���� � �.�. ��� ���� ������� � ���
���������� ��������� Msg, ������� ������ ���������� �� ���� ����������. � WinFunc() ���� �������
������ ��� ���� ���������. ���� �� ���������� ��������� WM_PAINT, ��������, �� � ����  ������ ���-�� ����������.
��� ��������� �������������� �������� switch(), ����������� �������� �������� Msg. Msg �� ������� ��� �������� ����� �������
WinFunc(). �������� �������� � ���� ���������, ��� ������� ���� �� ����� ����������� ����� ������������ � �� 
���������: WM_DESTROY. ����� ���� WM_PAINT, ������� ����� ��� ���������� � ���������� �������. WM � �� ���� Window Message*/

LRESULT CALLBACK WinFunc( HWND hWnd, // ���������� ����
						  UINT Msg, // ���������, ���������� ��
						  WPARAM wParam, // ���������
						  LPARAM lParam) // ���������, ��� ������������ ���������
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
  RECT r = {0, 0, Anim.W, Anim.H}; //����� ������������� ������ �������� ���� � ����������� ����� ����� ���� ����� ������������
  InvalidateRect(Anim.hWnd, &r, FALSE); //������ WinOnPaint
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
    if (PeekMessage(&msg, NULL, 0, 0, TRUE))//��� ��������� ���� �� � ������� ��������� � ��������� ����������� ������
    {
      if (msg.message == WM_QUIT)
        return;

      TranslateMessage(&msg);//��������� ��������� ������� ����������� ������ � ��������� �������
      DispatchMessage(&msg);//���������� ��������� ����
    }
    else
      SendMessage(Anim.hWnd, WM_TIMER, 0, 0);//�������� ������� ��������� ��� ��������� ���� � �� ���������� �������� �� ��� ���, ���� ������� ��������� �� ���������� ���������
  }
}

VOID WinDoExit( VOID )
{
  PostQuitMessage(0);
}
