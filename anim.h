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
  HWND hWnd;	//дескриптор окна. От фразы handle window
  HDC hDC;	// дескриптор контекста устройства. От фразы handle device context
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

VOID AnimInit( HINSTANCE hInstance ); // Вызывается WinInit
VOID AnimClose( VOID ); // Очищает картинку из памяти
VOID AnimResize( INT W, INT H ); // Изменяет размеры
VOID AnimResponse( VOID ); // Вся логика программы
//VOID AnimCopyFrame( VOID ); // Использует нативную буферизацию
VOID AnimRender( HDC hDC ); // Рисовка на экран
VOID AnimInputUpdate( VOID ); // Работа с клавишами и мышкой

#endif /* __ANIM_H_ */
