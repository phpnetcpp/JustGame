#ifndef __BUTTON_H_
#define __BUTTON_H_

#define CARDS_NUM 25

#include "image.h"

typedef struct tagBUTTON BUTTON;

typedef struct tagBUTTON
{
  IMAGE * Tex;
  INT W, H;
  DOUBLE PosX, PosY; /* in proportion to window sizes */
  VOID (* OnClick)( BUTTON * );

  INT Difference_Button;

} BUTTON;

VOID ButtonDraw( BUTTON * Btn ); // Рисует на экран 
BUTTON * ButtonCreate( IMAGE * Tex, DOUBLE PosX, DOUBLE PosY, VOID (* OnClick)( BUTTON * ) ); // Создает кнопку
VOID ButtonFree( BUTTON * Btn ); // Освобождает
BOOL ButtonIsClicked( BUTTON * Btn ); // Проверка нажатия
BUTTON * ButtonCreateEmpty( INT W, INT H, DOUBLE PosX, DOUBLE PosY, VOID (* OnClick)( BUTTON * ) ); // Создает пустую (нет картинки) кнопку 


// Different click callbacks

VOID ButtonCBPlay( BUTTON * Btn );
VOID ButtonCBSettings( BUTTON * Btn );
VOID ButtonCBExit( BUTTON * Btn );
VOID ButtonCBMainMenu( BUTTON * Btn );
VOID ButtonCBRules( BUTTON * Btn );
VOID ButtonCBEmpty( BUTTON * Btn );
VOID ButtonCBChangeCards( BUTTON * Btn );

VOID ButtonCreateCards( INT NumberCard, INT ButListNumber);


#endif /* __BUTTON_H_ */