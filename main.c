#include "anim.h"

INT WINAPI WinMain(   HINSTANCE hInstance //�������������, �� �������� �� ����� �������� �� ��������� ����
					, HINSTANCE hPrevInstance //���������� ���������� ���� (�� ������� ���������)
					, CHAR *CmdLine //���� ��������� ����� ��������� ������
					, INT CmdShow ) //���������� ������ ������ ����
{
  Anim.IsInit = FALSE;

  AnimInit(hInstance);
  WinRunGameLoop();
  return 0;
}