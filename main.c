#include "anim.h"

INT WINAPI WinMain(   HINSTANCE hInstance //идентификатор, по которому ОС будет отличать от остальных окон
					, HINSTANCE hPrevInstance //дескриптор экземпляра окна (на подобии указателя)
					, CHAR *CmdLine //если запускаем через командную строку
					, INT CmdShow ) //определяет способ показа окна
{
  Anim.IsInit = FALSE;

  AnimInit(hInstance);
  WinRunGameLoop();
  return 0;
}