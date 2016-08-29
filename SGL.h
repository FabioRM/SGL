/*****************************************************************************
Copyright (c) 2016 - Fabio Angeletti
e-mail: fabio.angeletti89@gmail.com

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

* Redistributions of source code must retain the above copyright notice, this
  list of conditions and the following disclaimer.

* Redistributions in binary form must reproduce the above copyright notice,
  this list of conditions and the following disclaimer in the documentation
  and/or other materials provided with the distribution.

* Neither the name of SGL nor the names of its contributors may be used
  to endorse or promote products derived from this software without
  specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

 Part of this code is an adaptation from the souce code provided by
 WaveShare - http://www.waveshare.net

 Part of this code is an adaptation from the souce code written by
 Michael Margolis - https://code.google.com/p/glcd-arduino/

 Part of this code is an adaptation from souce code found on 
 AvrFreaks - http://www.avrfreaks.net/sites/default/files/triangles.c

******************************************************************************/
 
#ifndef __SGL_H_
#define __SGL_H_

/* Includes ------------------------------------------------------------------*/
#include <math.h>
#include "fontLib.h"
#include "SGL_LL.h"

/* Macros --------------------------------------------------------------------*/
#define SGL_ASSEMBLE_ARGB(A,R,G,B)	(uint32_t)(A << 24 | R << 16 | G << 8 | B)
#define SGL_ASSEMBLE_RGB565(R,G,B)  (uint16_t)(((R & 0xF8) << 8) | ((G & 0xFC) << 3) | ((B & 0xF8) >> 3))
#define SGL_ASSEMBLE_RGB332(R,G,B)  (uint8_t)((((R & 0xE0) >> 5) | (((G & 0xE0) >> 5) | (((B & 0xC0) >> 6))
#define SGL_ASSEMBLE_MONO(R,G,B)	(uint8_t)(((R > 0) | (G > 0) | (B > 0)) ? (0xFF) : (0x00))

#define SGL_COLOR_BLUE          		(0xFF0000FF)
#define SGL_COLOR_GREEN         		(0xFF00FF00)
#define SGL_COLOR_RED           		(0xFFFF0000)
#define SGL_COLOR_CYAN          		(0xFF00FFFF)
#define SGL_COLOR_MAGENTA       		(0xFFFF00FF)
#define SGL_COLOR_YELLOW        		(0xFFFFFF00)
#define SGL_COLOR_LIGHTBLUE     		(0xFF8080FF)
#define SGL_COLOR_LIGHTGREEN    		(0xFF80FF80)
#define SGL_COLOR_LIGHTRED      		(0xFFFF8080)
#define SGL_COLOR_LIGHTCYAN     		(0xFF80FFFF)
#define SGL_COLOR_LIGHTMAGENTA  		(0xFFFF80FF)
#define SGL_COLOR_LIGHTYELLOW   		(0xFFFFFF80)
#define SGL_COLOR_DARKBLUE      		(0xFF000080)
#define SGL_COLOR_DARKGREEN     		(0xFF008000)
#define SGL_COLOR_DARKRED       		(0xFF800000)
#define SGL_COLOR_DARKCYAN      		(0xFF008080)
#define SGL_COLOR_DARKMAGENTA   		(0xFF800080)
#define SGL_COLOR_DARKYELLOW    		(0xFF808000)
#define SGL_COLOR_LIGHTGRAY     		(0xFFD3D3D3)
#define SGL_COLOR_GRAY          		(0xFF808080)
#define SGL_COLOR_DARKGRAY      		(0xFF404040)
#define SGL_COLOR_BROWN         		(0xFFA52A2A)
#define SGL_COLOR_ORANGE        		(0xFFFFA500)

#define SGL_COLOR_TRANSPARENT   		(0xFF000000)
#define SGL_COLOR_WHITE         		(0xFFFFFFFF)
#define SGL_COLOR_BLACK         		(0xFF000000)

#define SGL_DEFAULT_FONT						(FONT_TMSBLD_24)

/* Exported Functions --------------------------------------------------------*/
void SGL_Init(void);
void SGL_SetFont(FONT_t font);
void SGL_SetFrontColor(uint32_t color);
void SGL_SetBackColor(uint32_t color);

// Clear screen
void SGL_ClearScreen(void);
// writes a character with background
void SGL_PutChar(uint32_t Xpos, uint32_t Ypos, uint8_t ASCII);
// writes a character without background using default font
void SGL_CleanPutChar(uint32_t Xpos, uint32_t Ypos, uint8_t ASCII);
// writes a string with background using default font
void SGL_Text(uint32_t Xpos, uint32_t Ypos, uint8_t *str);
// writes a string without background using default font
void SGL_CleanText(uint32_t Xpos, uint32_t Ypos, uint8_t *str);

// draws a picture saved as array
void SGL_DrawPicture(uint32_t x, uint32_t y, uint32_t w, uint32_t h, const uint32_t *pic);
// draws a picture saved as 2x8bit array
void SGL_DrawPicture8bit(uint32_t x, uint32_t y, uint32_t w, uint32_t h, const uint8_t *pic);

// draws a point at x, y
void SGL_DrawPoint(uint32_t x, uint32_t y);
// draws a line starting at x0,y0 and ending at x1,y1
void SGL_DrawLine(uint32_t x0, uint32_t y0, uint32_t x1, uint32_t y1);
// draws a circle with center x0,y0 radius r
void SGL_DrawCircle(uint32_t x0, uint32_t y0, uint32_t r);
// draws a filled circle with center x0,y0 radius r
void SGL_FillCircle(uint32_t x0, uint32_t y0, uint32_t r);
// draws a rectangle starting at x,y with width w and height h
void SGL_DrawRect(uint32_t x, uint32_t y, uint32_t w, uint32_t h);
// draws a filled rectangle starting at x,y with width w and height h
void SGL_FillRect(uint32_t x, uint32_t y, uint32_t w, uint32_t h);
// draws a rounded rectangle starting at x,y with width w and height h with rounded radius r
void SGL_DrawRoundRect(int16_t x, int16_t y, int16_t w, int16_t h, int16_t r);
// draws a filled and rounded rectangle starting at x,y with width w and height h with rounded radius r
void SGL_FillRoundRect(int16_t x, int16_t y, int16_t w, int16_t h, int16_t r);
// draws a triangle with coordinates x0,y0 x1,y1 x2,y2
void SGL_DrawTriangle(uint32_t x0, uint32_t y0, uint32_t x1, uint32_t y1, uint32_t x2, uint32_t y2);
// draws a filled triangle with coordinates x0,y0 x1,y1 x2,y2
void SGL_FillTriangle(int16_t x1, int16_t y1, int16_t x2, int16_t y2, int16_t x3, int16_t y3);
// draws an ellipse centered in x0,y0 with vertical length rx and horizontal length ry
void SGL_DrawEllipse(int16_t x0, int16_t y0, int16_t rx, int16_t ry);
// draws a filled ellipse centered in x0,y0 with vertical length rx and horizontal length ry
void SGL_FillEllipse(int16_t x0, int16_t y0, int16_t rx, int16_t ry);
#endif
