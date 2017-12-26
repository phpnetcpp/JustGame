/* Animation module */

#include <string.h>
#include <stdio.h>
#include <windows.h>

#include "anim.h"

/* Global animation interface data */
ANIM Anim;

static VOID AnimLayoutInit( VOID ) // Инициилизирует меню
{
  Anim.LMainMenu.Background = ImageLoadBMP(Anim.hDC, "image/main_background.bmp");
  Anim.LMainMenu.ButNum = 4;
  Anim.LMainMenu.ButList = malloc(sizeof(BUTTON *) * Anim.LMainMenu.ButNum);
  Anim.LMainMenu.ButList[0] = ButtonCreate(ImageLoadBMP(Anim.hDC, "image/buttons/play.bmp"), 0.5, 0.4, ButtonCBPlay); 
  Anim.LMainMenu.ButList[1] = ButtonCreate(ImageLoadBMP(Anim.hDC, "image/buttons/settings.bmp"), 0.5, 0.55, ButtonCBSettings); 
  Anim.LMainMenu.ButList[2] = ButtonCreate(ImageLoadBMP(Anim.hDC, "image/buttons/rules.bmp"), 0.5, 0.7, ButtonCBRules);
  Anim.LMainMenu.ButList[3] = ButtonCreate(ImageLoadBMP(Anim.hDC, "image/buttons/exit.bmp"), 0.5, 0.85, ButtonCBExit);

  Anim.LRules.Background = ImageLoadBMP(Anim.hDC, "image/rules_background.bmp");
  Anim.LRules.ButNum = 1;
  Anim.LRules.ButList = malloc(sizeof(BUTTON *) * Anim.LRules.ButNum);
  Anim.LRules.ButList[0] = ButtonCreate(ImageLoadBMP(Anim.hDC, "image/buttons/exit.bmp"), 0.12, 0.13, ButtonCBMainMenu);

  Anim.LSettings.Background = ImageLoadBMP(Anim.hDC, "image/settings_background.bmp");
  Anim.LSettings.ButNum = 1;
  Anim.LSettings.ButList = malloc(sizeof(BUTTON *) * Anim.LSettings.ButNum);
  Anim.LSettings.ButList[0] = ButtonCreate(ImageLoadBMP(Anim.hDC, "image/buttons/exit.bmp"), 0.5, 0.85, ButtonCBMainMenu);

  Anim.LGame.Background = ImageLoadBMP(Anim.hDC, "image/game_background.bmp");
  Anim.LGame.ButNum = 3;
  Anim.LGame.ButList = malloc(sizeof(BUTTON *) * Anim.LGame.ButNum);
  Anim.LGame.ButList[0] = ButtonCreate(ImageLoadBMP(Anim.hDC, "image/buttons/exit.bmp"), 0.5, 0.9, ButtonCBMainMenu);
  Anim.LGame.ButList[1] = ButtonCreateEmpty(0, 0, 0, 0, ButtonCBEmpty);
  Anim.LGame.ButList[2] = ButtonCreateEmpty(0, 0, 0, 0, ButtonCBEmpty);

  Anim.LCurrent = &Anim.LMainMenu;
}

VOID AnimInit( HINSTANCE hInstance )
{ 
  WinInit(hInstance);

  AnimLayoutInit();

  Anim.IsInit = TRUE;

  Anim.First_Difference = FALSE;
  Anim.Second_Difference = FALSE;
}

VOID AnimClose( VOID )
{
  ImageFree(Anim.Frame);
}

VOID AnimResize( INT W, INT H )
{
  Anim.W = W;
  Anim.H = H;
  ImageResize(Anim.Frame, W, H);
}

/*VOID AnimCopyFrame( VOID )
{
  SwapBuffers(Anim.hDC);
} */

VOID AnimResponse( VOID )
{
  INT i;

  if (!Anim.IsInit)
    return;

  AnimInputUpdate();

  for (i = 0; i < Anim.LCurrent->ButNum && !Anim.IsLayoutChanged; i++)
    if (ButtonIsClicked(Anim.LCurrent->ButList[i]))
      Anim.LCurrent->ButList[i]->OnClick(Anim.LCurrent->ButList[i]);

  Anim.IsLayoutChanged = FALSE;
}

VOID AnimRender( HDC hDC )
{
  INT i, j;

  if (!Anim.IsInit)
    return;

  if (Anim.LCurrent->Background != NULL)
    ImageDrawToImage(Anim.LCurrent->Background, Anim.Frame, 0, 0);

  for (i = 0; i < Anim.LCurrent->ButNum; i++)
    ButtonDraw(Anim.LCurrent->ButList[i]);

  if (Anim.LCurrent == &Anim.LGame)
  {
    ImageDrawToImage(Anim.CardCurImg, Anim.Frame, 0, 0);
    ImageDrawToImage(Anim.CardMainImg, Anim.Frame, Anim.CardCurImg->W, 0);
  }

  if (Anim.First_Difference)
  {
	  for (i = 0; i < Anim.Differences[Anim.CardMain].W; i++)
		  for (j = 0; j < 3; j++)
			ImagePutPixel(Anim.Frame, Anim.Differences[Anim.CardMain].X + i, Anim.Differences[Anim.CardMain].Y + j, 0xFF0400FF);
	  for (i = 0; i < Anim.Differences[Anim.CardMain].H; i++)
		  for (j = 0; j < 3; j++)
			ImagePutPixel(Anim.Frame, Anim.Differences[Anim.CardMain].X + j, Anim.Differences[Anim.CardMain].Y + i, 0xFF0400FF);
	  for (i = 0; i < Anim.Differences[Anim.CardMain].W + 3; i++)
		  for (j = 0; j < 3; j++)
			ImagePutPixel(Anim.Frame, Anim.Differences[Anim.CardMain].X + i, Anim.Differences[Anim.CardMain].Y +
			  Anim.Differences[Anim.CardMain].H + j, 0xFF0400FF);
	  for (i = 0; i < Anim.Differences[Anim.CardMain].H + 3; i++)
		  for (j = 0; j < 3; j++)
			ImagePutPixel(Anim.Frame, Anim.Differences[Anim.CardMain].X + Anim.Differences[Anim.CardMain].W + j,
			  Anim.Differences[Anim.CardMain].Y + i, 0xFF0400FF);
  }
  
  if (Anim.Second_Difference)
  {
	  for (i = 0; i < Anim.Differences[Anim.CardCur].W; i++)
		  for (j = 0; j < 3; j++)
			ImagePutPixel(Anim.Frame, Anim.Differences[Anim.CardCur].X + i, Anim.Differences[Anim.CardCur].Y + j, 0xFF0400FF);
	  for (i = 0; i < Anim.Differences[Anim.CardCur].H; i++)
		  for (j = 0; j < 3; j++)
			ImagePutPixel(Anim.Frame, Anim.Differences[Anim.CardCur].X + j, Anim.Differences[Anim.CardCur].Y + i, 0xFF0400FF);
	  for (i = 0; i < Anim.Differences[Anim.CardCur].W + 3; i++)
		  for (j = 0; j < 3; j++)
			ImagePutPixel(Anim.Frame, Anim.Differences[Anim.CardCur].X + i, Anim.Differences[Anim.CardCur].Y +
			  Anim.Differences[Anim.CardCur].H + j, 0xFF0400FF);
	  for (i = 0; i < Anim.Differences[Anim.CardCur].H + 3; i++)
		  for (j = 0; j < 3; j++)
			ImagePutPixel(Anim.Frame, Anim.Differences[Anim.CardCur].X + Anim.Differences[Anim.CardCur].W + j,
			  Anim.Differences[Anim.CardCur].Y + i, 0xFF0400FF);
  }

  ImageDraw(Anim.Frame, hDC, 0, 0);  
}

VOID AnimInputUpdate( VOID )
{
  INT i;
  POINT pt;
  static INT OldX = 0, OldY = 0;

  /* Get keyboard state */
  memcpy(Anim.OldKeys, Anim.Keys, 256);
  GetKeyboardState(Anim.Keys);
  for (i = 0; i < 256; i++)
    Anim.Keys[i] >>= 7;
  for (i = 0; i < 256; i++)
    Anim.KeysClick[i] = Anim.Keys[i] && !Anim.OldKeys[i];

  /* Get mouse info */
  GetCursorPos(&pt);

  GetCursorPos(&pt);
  ScreenToClient(Anim.hWnd, &pt);
  Anim.MsPosX = pt.x;
  Anim.MsPosY = pt.y;

  Anim.MsDeltaX = pt.x - OldX;
  Anim.MsDeltaY = pt.x - OldY;
  OldX = pt.x;
  OldY = pt.x;
}
