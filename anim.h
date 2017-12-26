/* Animation module */

#ifndef __ANIM_H_
#define __ANIM_H_

#include "win.h"
#include "image.h"
#include "button.h"

/* Forward declaration */
typedef struct tagANIM ANIM;
typedef struct tagLAYOUT LAYOUT;

typedef struct tagLAYOUT
{
  IMAGE * Background;
  BUTTON ** ButList;
  INT ButNum;
} LAYOUT;

typedef struct tagRECTANGLE
{
  INT X, Y; // left top 
  INT W, H;
} RECTANGLE;

typedef struct tagANIM
{
  /* Render data */
  HWND hWnd;	//���������� ����. �� ����� handle window
  HDC hDC;	// ���������� ��������� ����������. �� ����� handle device context
  INT W, H;
  IMAGE * Frame;

  BOOL IsInit;
  BOOL IsLayoutChanged;

  /* Input data */
  BYTE Keys[256], OldKeys[256], KeysClick[256];

  INT MsPosX, MsPosY, MsDeltaX, MsDeltaY;

  LAYOUT * LCurrent;

  LAYOUT LMainMenu, LGame, LSettings, LRules;

  /* Game related stuff */
  INT CardMain, CardCur;
  IMAGE * CardMainImg, * CardCurImg;
  RECTANGLE * Differences;

  BOOL First_Difference;
  BOOL Second_Difference;

} ANIM;

extern ANIM Anim;

VOID AnimInit( HINSTANCE hInstance ); // ���������� WinInit
VOID AnimClose( VOID ); // ������� �������� �� ������
VOID AnimResize( INT W, INT H ); // �������� �������
VOID AnimResponse( VOID ); // ��� ������ ���������
//VOID AnimCopyFrame( VOID ); // ���������� �������� �����������
VOID AnimRender( HDC hDC ); // ������� �� �����
VOID AnimInputUpdate( VOID ); // ������ � ��������� � ������

#endif /* __ANIM_H_ */
