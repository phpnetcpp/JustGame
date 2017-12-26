#include <math.h>
#include <stdio.h>

#include "button.h"
#include "anim.h"


VOID ButtonDraw( BUTTON * Btn )
{
  if (Btn->Tex != NULL)
    ImageDrawToImage(Btn->Tex, Anim.Frame, (INT)(Anim.W * Btn->PosX - Btn->Tex->W / 2), (INT)(Anim.H * Btn->PosY - Btn->Tex->H / 2));
}

BUTTON * ButtonCreate( IMAGE * Tex, DOUBLE PosX, DOUBLE PosY, VOID (* OnClick)( BUTTON * ) )
{
  BUTTON * nbutton = malloc(sizeof(BUTTON));
  nbutton->Tex = Tex;
  nbutton->PosX = PosX;
  nbutton->PosY = PosY;
  nbutton->OnClick = OnClick;
  nbutton->W = Tex->W;
  nbutton->H = Tex->H;

  return nbutton;
}

BUTTON * ButtonCreateEmpty( INT W, INT H, DOUBLE PosX, DOUBLE PosY, VOID (* OnClick)( BUTTON * ) )
{
  BUTTON * nbutton = malloc(sizeof(BUTTON));
  nbutton->Tex = NULL;
  nbutton->PosX = PosX;
  nbutton->PosY = PosY;
  nbutton->OnClick = OnClick;
  nbutton->W = W;
  nbutton->H = H;

  return nbutton;
}

BOOL ButtonIsClicked( BUTTON * Btn )
{
  if (!Anim.KeysClick[VK_LBUTTON] || Btn == NULL)
    return FALSE;

  if (abs(Anim.MsPosX - (INT)(Anim.W * Btn->PosX)) <= Btn->W / 2 &&
      abs(Anim.MsPosY - (INT)(Anim.H * Btn->PosY)) <= Btn->H / 2)
    return TRUE;
  return FALSE;
}

VOID ButtonFree( BUTTON * Btn )
{
  if (Btn->Tex != NULL)
    ImageFree(Btn->Tex);
  free(Btn);
}

VOID ButtonCBExit( BUTTON * Btn )
{
  PostQuitMessage(0);
}

VOID ButtonCBPlay( BUTTON * Btn )
{
  CHAR s[100]; 
  INT i, buf, iter=0;
  FILE * DifFile;
 

  Anim.CardCur = rand() % CARDS_NUM;
  Anim.CardMain = rand() % CARDS_NUM;
  while (Anim.CardMain == Anim.CardCur)
	Anim.CardMain = rand() % CARDS_NUM;

  sprintf(s, "image/boloto/b%d.bmp", Anim.CardMain + 1);
  Anim.CardMainImg = ImageLoadBMP(Anim.hDC, s);

  sprintf(s, "image/boloto/b%d.bmp", Anim.CardCur + 1);
  Anim.CardCurImg = ImageLoadBMP(Anim.hDC, s);

  DifFile = fopen("differences.txt", "r");
  Anim.Differences = malloc(sizeof(RECTANGLE) * CARDS_NUM);

  for (i = 0; i < CARDS_NUM; i++)
    fscanf(DifFile, "%d%d%d%d%d", &buf, &Anim.Differences[i].X, &Anim.Differences[i].Y, 
                                        &Anim.Differences[i].W, &Anim.Differences[i].H);

  ButtonCreateCards(Anim.CardMain, 1);
  ButtonCreateCards(Anim.CardCur, 2);

  Anim.LCurrent = &Anim.LGame;
  Anim.IsLayoutChanged = TRUE;
}

VOID ButtonCBSettings( BUTTON * Btn )
{
	Anim.First_Difference = FALSE;
	Anim.Second_Difference = FALSE;
	Anim.LCurrent = &Anim.LSettings;
	Anim.IsLayoutChanged = TRUE;
}

VOID ButtonCBChangeCards( BUTTON * Btn )
{
  INT bufw, bufh, bufx, bufy, animw, animh; 
  CHAR s[100];

  if (Btn->Difference_Button == 1)
  {
	  Anim.First_Difference = TRUE;
  }

  if (Btn->Difference_Button == 2)
  {
	  Anim.Second_Difference = TRUE;
  }

	if (Anim.First_Difference && Anim.Second_Difference)
	{

	  MessageBox(NULL, //If this parameter is NULL, the message box has no owner window.
				L"Отличия были найдены, для смены карт нажмите ОК",
				L"Хорошая работа", 
				MB_ICONASTERISK | MB_OK);

	  //DrawText();

	  Anim.CardMain = Anim.CardCur;
	  Anim.CardMainImg = Anim.CardCurImg;
	  Anim.CardCur = rand() % CARDS_NUM;
	  while (Anim.CardMain == Anim.CardCur)
		Anim.CardCur = rand() % CARDS_NUM;

	  sprintf(s, "image/boloto/b%d.bmp", Anim.CardCur + 1);
	  Anim.CardCurImg = ImageLoadBMP(Anim.hDC, s);

	  ButtonCreateCards(Anim.CardMain, 1);
	  ButtonCreateCards(Anim.CardCur, 2);

	  Anim.IsLayoutChanged = TRUE;

	  Anim.First_Difference = FALSE;
	  Anim.Second_Difference = FALSE;
	}
}

VOID ButtonCBMainMenu( BUTTON * Btn )
{
	Anim.First_Difference = FALSE;
	Anim.Second_Difference = FALSE;
    Anim.LCurrent = &Anim.LMainMenu;  
    Anim.IsLayoutChanged = TRUE;
}

VOID ButtonCBRules( BUTTON * Btn )
{
	Anim.First_Difference = FALSE;
	Anim.Second_Difference = FALSE;
    Anim.LCurrent = &Anim.LRules;
    Anim.IsLayoutChanged = TRUE;
}  

VOID ButtonCBEmpty( BUTTON * Btn )
{
}

VOID ButtonCreateCards(INT NumberCard, INT ButListNumber)
{
	INT bufw, bufh, bufx, bufy, animw, animh;

	bufw = Anim.Differences[NumberCard].W;
	bufh = Anim.Differences[NumberCard].H;
	bufx = Anim.Differences[NumberCard].X;
	bufy = Anim.Differences[NumberCard].Y;
	animw = Anim.Frame->W;
	animh = Anim.Frame->H;

	Anim.LGame.ButList[ButListNumber] = ButtonCreateEmpty(bufw, bufh, (DOUBLE)(bufx + bufw / 2 + Anim.CardCurImg->W) / animw, (DOUBLE)(bufy + bufh / 2) / animh, ButtonCBChangeCards);

	Anim.LGame.ButList[ButListNumber]->Difference_Button = ButListNumber;
}