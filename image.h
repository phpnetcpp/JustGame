/* Image interactions module */ 

#ifndef __IMAGE_H_
#define __IMAGE_H_

#include <windows.h>

typedef struct tagIMAGE
{
  INT W, H;             // image size
  DWORD *Bits;          // image pixel colors (32-������ ����������� �����)
  BITMAPINFOHEADER Bih; // DIB section header	//contains information about the dimensions and color format of a DIB
  HBITMAP hBm;          // DIB section handle	//���������� ���������� �����������. �� ����� handle bitmap
  HDC hMemDC;           // Memory device context handle	//���������� ��������� ����������. �� ����� handle device context
} IMAGE;

IMAGE * ImageCreateEmpty( HDC hDC, INT W, INT H ); // ������� ������ ��������
IMAGE * ImageLoadBMP( HDC hDC, CHAR * FileName ); // ��������� BMP
IMAGE * ImageCopy( IMAGE * Src, HDC hDC ); // �������� 
VOID ImageFree( IMAGE * Img ); // �����������
VOID ImageResize( IMAGE * Img, INT W, INT H ); // ������ ���� ������� (����������� ������ � ������� ����� )
VOID ImageDraw( IMAGE * Img, HDC hDC, INT Dx, INT Dy ); // ������ ��������
VOID ImageDrawToImage( IMAGE * Src, IMAGE * Dst, INT Dx, INT Dy ); // ������ � �������� 
VOID ImagePutPixel( IMAGE * Img, INT X, INT Y, DWORD Color ); // � �������� ������ �������
DWORD ImageGetPixel( IMAGE * Img, INT X, INT Y ); // ������� � �������� �������
DWORD xRGB( BYTE R, BYTE G, BYTE B ); // �� ����� RGB ���������� � DWORD


#endif /* __IMAGE_H_ */