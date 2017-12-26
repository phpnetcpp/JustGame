/* Image interactions module */ 

#ifndef __IMAGE_H_
#define __IMAGE_H_

#include <windows.h>

typedef struct tagIMAGE
{
  INT W, H;             // image size
  DWORD *Bits;          // image pixel colors (32-битное беззнаковое целое)
  BITMAPINFOHEADER Bih; // DIB section header	//contains information about the dimensions and color format of a DIB
  HBITMAP hBm;          // DIB section handle	//дескриптор растрового изображения. От фразы handle bitmap
  HDC hMemDC;           // Memory device context handle	//дескриптор контекста устройства. От фразы handle device context
} IMAGE;

IMAGE * ImageCreateEmpty( HDC hDC, INT W, INT H ); // Создаем пустую картинку
IMAGE * ImageLoadBMP( HDC hDC, CHAR * FileName ); // Загружает BMP
IMAGE * ImageCopy( IMAGE * Src, HDC hDC ); // Копирует 
VOID ImageFree( IMAGE * Img ); // Освобождает
VOID ImageResize( IMAGE * Img, INT W, INT H ); // Задаем свои размеры (освобождает старую и создает новую )
VOID ImageDraw( IMAGE * Img, HDC hDC, INT Dx, INT Dy ); // Рисуем контекст
VOID ImageDrawToImage( IMAGE * Src, IMAGE * Dst, INT Dx, INT Dy ); // Рисует в картинку 
VOID ImagePutPixel( IMAGE * Img, INT X, INT Y, DWORD Color ); // В картинку рисует пиксель
DWORD ImageGetPixel( IMAGE * Img, INT X, INT Y ); // ДОстает с картинки пиксель
DWORD xRGB( BYTE R, BYTE G, BYTE B ); // Из чисел RGB возвращает в DWORD


#endif /* __IMAGE_H_ */