/* Windows interaction module */

#ifndef __WIN_H_
#define __WIN_H_

#include <windows.h>

#define WND_CLASS_NAME "difference game"

VOID WinInit( HINSTANCE hInstance ); // Регистриует и создате окно
VOID WinRunGameLoop( VOID ); // Разбирает сообщения и отправляет их в функцию WinFunc

/* Windows callbacks */
LRESULT CALLBACK WinFunc( HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam ); // Функция которая реагирут на сообщения
BOOL WinOnCreate( CREATESTRUCT *CS ); // Создать окно
VOID WinOnDestroy( VOID ); // Уничтожить окно
VOID WinOnSize( UINT State, INT W, INT H ); // Изменить размер окна
BOOL WinOnEraseBkgnd( HDC hDC ); // Стереть фон окна
VOID WinOnPaint( VOID ); // Рисуем
VOID WinOnActivate( UINT Reason, HWND hWndActDeact, BOOL IsMinimized ); // Сделать активным окном
VOID WinOnTimer( INT Id ); // Перерисовка окна
VOID WinOnMouseWheel( INT X, INT Y, INT Z, UINT Keys ); // Считываем колесо мыши
VOID WinOnMouseMove( INT X, INT Y, UINT Keys ); // Считываем джвижение мыши
VOID WinOnButtonDown( BOOL IsDoubleClick, INT X, INT Y, UINT Keys ); // Ввод мыши по окну 
VOID WinOnButtonUp( INT X, INT Y, UINT Keys ); // Ввод мыши не по окну
VOID WinDoExit( VOID ); // Закрытие окон


#endif /* __WIN_H_ */