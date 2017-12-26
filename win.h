/* Windows interaction module */

#ifndef __WIN_H_
#define __WIN_H_

#include <windows.h>

#define WND_CLASS_NAME "difference game"

VOID WinInit( HINSTANCE hInstance ); // ����������� � ������� ����
VOID WinRunGameLoop( VOID ); // ��������� ��������� � ���������� �� � ������� WinFunc

/* Windows callbacks */
LRESULT CALLBACK WinFunc( HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam ); // ������� ������� �������� �� ���������
BOOL WinOnCreate( CREATESTRUCT *CS ); // ������� ����
VOID WinOnDestroy( VOID ); // ���������� ����
VOID WinOnSize( UINT State, INT W, INT H ); // �������� ������ ����
BOOL WinOnEraseBkgnd( HDC hDC ); // ������� ��� ����
VOID WinOnPaint( VOID ); // ������
VOID WinOnActivate( UINT Reason, HWND hWndActDeact, BOOL IsMinimized ); // ������� �������� �����
VOID WinOnTimer( INT Id ); // ����������� ����
VOID WinOnMouseWheel( INT X, INT Y, INT Z, UINT Keys ); // ��������� ������ ����
VOID WinOnMouseMove( INT X, INT Y, UINT Keys ); // ��������� ��������� ����
VOID WinOnButtonDown( BOOL IsDoubleClick, INT X, INT Y, UINT Keys ); // ���� ���� �� ���� 
VOID WinOnButtonUp( INT X, INT Y, UINT Keys ); // ���� ���� �� �� ����
VOID WinDoExit( VOID ); // �������� ����


#endif /* __WIN_H_ */