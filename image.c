/* Image interactions module */ 

#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "image.h"

IMAGE * ImageCreateEmpty( HDC hDC, INT W, INT H )
{
  IMAGE * Img = malloc(sizeof(IMAGE));
  Img->W = W;
  Img->H = H;
  Img->Bits = NULL;
  Img->hBm = NULL;
  Img->hMemDC = CreateCompatibleDC(hDC);

  ImageResize(Img, W, H);

  return Img;
}

IMAGE * ImageCopy( IMAGE * Src, HDC hDC )
{
  IMAGE * Img = ImageCreateEmpty(hDC, Src->W, Src->H);
  memcpy(Img->Bits, Src->Bits, sizeof(DWORD) * Img->W * Img->H);
  return Img;
}

VOID ImageResize( IMAGE * Img, INT W, INT H )
{
  if (Img->hBm != NULL)
    DeleteObject(Img->hBm);

  Img->W = W;
  Img->H = H;

  Img->Bih.biSize = sizeof(BITMAPINFOHEADER);
  Img->Bih.biWidth = W;
  Img->Bih.biHeight = -H;
  Img->Bih.biPlanes = 1;
  Img->Bih.biBitCount = 32;
  Img->Bih.biCompression = BI_RGB;
  Img->Bih.biSizeImage = W * H * 4;
  Img->Bih.biClrImportant = 0;
  Img->Bih.biClrUsed = 0;
  Img->Bih.biXPelsPerMeter = Img->Bih.biYPelsPerMeter = 0;

  Img->hBm = CreateDIBSection(NULL, (BITMAPINFO *)&(Img->Bih), DIB_RGB_COLORS,
                             (VOID **)&(Img->Bits), NULL, 0);

  SelectObject(Img->hMemDC, Img->hBm);
}

IMAGE * ImageLoadBMP( HDC hDC, CHAR * FileName )
{
  FILE *F;
  IMAGE * Img;

  /* Open image file */
  if ((F = fopen(FileName, "rb")) == NULL)
    return NULL;

  BITMAPFILEHEADER bfh;
  BITMAPINFOHEADER bih;

  /* Check for BMP */
  if (fread(&bfh, sizeof(BITMAPFILEHEADER), 1, F) == 1 &&
      fread(&bih, sizeof(BITMAPINFOHEADER), 1, F) == 1 &&
      bfh.bfType == ('B' | ('M' << 8)) &&
      bfh.bfReserved1 == 0 &&
      bfh.bfReserved2 == 0 &&
      bih.biSize == sizeof(BITMAPINFOHEADER) &&
      bih.biPlanes == 1 &&
      (bih.biBitCount == 1 || bih.biBitCount == 4 ||
       bih.biBitCount == 8 || bih.biBitCount == 24) &&
      bih.biCompression == BI_RGB)
  {
    INT BPL, Colors, off, x, y;
    BYTE *Buf;
    static RGBQUAD Pal[256];

    /* Load palette table */
    if (bih.biClrUsed != 0)
      Colors = bih.biClrUsed;
    else
      switch (bih.biBitCount)
      {
      case 1:
        Colors = 2;
        break;
      case 4:
        Colors = 16;
        break;
      case 8:
        Colors = 256;
        break;
      default:
        Colors = 0;
        break;
      }
    if (Colors == 0 && bih.biClrImportant < 1024)
      Colors = bih.biClrImportant;
    /* Load palette */
    fread(Pal, 4, min(Colors, 256), F);

    /* Goto image color data */
    if (bfh.bfOffBits != 0)
      fseek(F, bfh.bfOffBits, SEEK_SET);
    else
      fseek(F, sizeof(BITMAPFILEHEADER) +
               sizeof(BITMAPINFOHEADER) + Colors * 4,
            SEEK_SET);

    /* Calculate bytes per line */
    switch (bih.biBitCount)
    {
    case 1:
      BPL = ((bih.biWidth + 7) / 8 + 3) / 4 * 4;
      break;
    case 4:
      BPL = ((bih.biWidth + 1) / 2 + 3) / 4 * 4;
      break;
    case 8:
      BPL = (bih.biWidth + 3) / 4 * 4;
      break;
    default:
      BPL = (bih.biWidth * 3 + 3) / 4 * 4;
      break;
    }

    /* Allocate memory for one line buffer */
    if ((Buf = malloc(BPL)) == NULL)
    {
      fclose(F);
      return NULL;
    }

    Img = ImageCreateEmpty(hDC, bih.biWidth, bih.biHeight);

    for (y = 0; y < bih.biHeight; y++)
    {
      fread(Buf, 1, BPL, F);
      off = bih.biWidth * (bih.biHeight - 1 - y);
      for (x = 0; x < bih.biWidth; x++)
      {
        int r, g, b, v;

        switch (bih.biBitCount)
        {
        case 1:
          v = (Buf[x >> 3] >> (7 - (x & 7))) & 1;
          b = Pal[v].rgbBlue;
          g = Pal[v].rgbGreen;
          r = Pal[v].rgbRed;
          break;
        case 4:
          v = (Buf[x >> 1] >> ((1 - (x & 1)) << 2)) & 0x0F;
          b = Pal[v].rgbBlue;
          g = Pal[v].rgbGreen;
          r = Pal[v].rgbRed;
          break;
        case 8:
          v = Buf[x];
          b = Pal[v].rgbBlue;
          g = Pal[v].rgbGreen;
          r = Pal[v].rgbRed;
          break;
        default:
          b = Buf[x * 3 + 0];
          g = Buf[x * 3 + 1];
          r = Buf[x * 3 + 2];
          break;
        }
        Img->Bits[off++] = xRGB(r, g, b);
      }
    }
    free(Buf);
  }

  fclose(F);

  return Img;
}

VOID ImageFree( IMAGE * Img )
{
  if (Img->hBm != NULL) 
    DeleteObject(Img->hBm);
  if (Img->hMemDC != NULL)
    DeleteDC(Img->hMemDC);
}

VOID ImageDraw( IMAGE * Img, HDC hDC, INT Dx, INT Dy )
{
  BitBlt(hDC, Dx, Dy, Img->W, Img->H, Img->hMemDC, 0, 0, SRCCOPY);
}

VOID ImageDrawToImage( IMAGE * Src, IMAGE * Dst, INT Dx, INT Dy )
{
  BitBlt(Dst->hMemDC, Dx, Dy, Src->W, Src->H, Src->hMemDC, 0, 0, SRCCOPY);
}

VOID ImagePutPixel( IMAGE * Img, INT X, INT Y, DWORD Color )
{
  if (X < 0 || Y < 0 || X >=Img->W || Y >= Img->H)
    return;
  Img->Bits[Y * Img->W + X] = Color;
}

DWORD ImageGetPixel( IMAGE * Img, INT X, INT Y )
{
  if (X < 0 || Y < 0 || X >= Img->W || Y >= Img->H)
    return 0x00000000;
  return Img->Bits[Y * Img->W + X];
}

DWORD xRGB( BYTE R, BYTE G, BYTE B )
{
  return 0xFF000000 | (R << 16) | (G << 8) | (B << 0);
}