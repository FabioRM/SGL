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

 Part of this code is an adaptation from the souce code found on 
 AvrFreaks - http://www.avrfreaks.net/sites/default/files/triangles.c

 Part of this code is an adaptation from the souce code provided by the user
 "colinday" on stackoverflow (Midpoint circle algorithm for filled circles)

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

#define SGL_DEFAULT_FONT				(FONT_TMSBLD_24)

/* Exported Functions --------------------------------------------------------*/

/**
 * @brief      Initilize the library
 *
 * @param[in]  width   The screen width
 * @param[in]  height  The screen height
 */
void SGL_Init(uint32_t width, uint32_t height);

/**
 * @brief      Changes the Font
 *
 * @param[in]  font  The font to use
 */
void SGL_SetFont(FONT_t font);

/**
 * @brief      Changes the drawing color
 *
 * @param[in]  color  The color
 */
void SGL_SetFrontColor(uint32_t color);

/**
 * @brief      Changes the background color
 *
 * @param[in]  color  The color
 */
void SGL_SetBackColor(uint32_t color);

/**
 * @brief      Clears the whole screen with the background color
 */
void SGL_ClearScreen(void);

/**
 * @brief      Draws a point to the given position, if the position lies outside
 *             the screen area, it does nothing
 *
 * @param[in]  x     The x position
 * @param[in]  y     The y position
 */
void SGL_DrawPoint(int32_t x, int32_t y);

/**
 * @brief      Draws a line from position (x0, y0) to position (x1, y1)
 *
 * @param[in]  x0    The position x0
 * @param[in]  y0    The position y0
 * @param[in]  x1    The position x1
 * @param[in]  y1    The position y1
 */
void SGL_DrawLine(int32_t x0, int32_t y0, int32_t x1, int32_t y1);

/**
 * @brief      Puts a single char to the given position
 *
 * @param[in]  xPos   The x position
 * @param[in]  yPos   The y position
 * @param[in]  ASCII  The character to print
 */
void SGL_PutChar(uint32_t xPos, uint32_t yPos, uint8_t ASCII);

/**
 * @brief      Write a null-terminated string starting from the given position
 *
 * @param[in]  xPos  The x position
 * @param[in]  yPos  The y position
 * @param      str   The string to write
 */
void SGL_Text(uint32_t xPos, uint32_t yPos, uint8_t *str);

/**
 * @brief      Puts a single char to the given position with background
 *
 * @param[in]  xPos   The x position
 * @param[in]  yPos   The y position
 * @param[in]  ASCII  The character to print
 */
void SGL_PutCharBg(uint32_t xPos, uint32_t yPos, uint8_t ASCII);

/**
 * @brief      Write a null-terminated string starting from the given position
 *             with background
 *
 * @param[in]  xPos  The x position
 * @param[in]  yPos  The y position
 * @param      str   The string to write
 */
void SGL_TextBg(uint32_t xPos, uint32_t yPos, uint8_t *str);

/**
 * @brief      Draws a circle centered at position (x0, y0) with radius of r
 *             pixels
 *
 * @param[in]  x0    The x0 position
 * @param[in]  y0    The y0 position
 * @param[in]  r     The radius of the circle in pixels
 */
void SGL_DrawCircle(uint32_t x0, uint32_t y0, uint32_t r);

/**
 * @brief      Draws a filled circle centered at position (x0, y0) with radius r
 *
 * @param[in]  x0    The x0 position
 * @param[in]  y0    The y0 position
 * @param[in]  r     The radius of the circle
 */
void SGL_FillCircle(uint32_t x0, uint32_t y0, uint32_t r);

/**
 * @brief      Draws a rectangle starting from position (x, y) with width w and
 *             height h
 *
 * @param[in]  x     The x position
 * @param[in]  y     The y position
 * @param[in]  w     The width of the rectangle
 * @param[in]  h     The height of the rectangle
 */
void SGL_DrawRect(uint32_t x, uint32_t y, uint32_t w, uint32_t h);

/**
 * @brief      Draws a filled rectangle starting from position (x, y) with width
 *             w and height h
 *
 * @param[in]  x     The x position
 * @param[in]  y     The y position
 * @param[in]  w     The width of the rectangle
 * @param[in]  h     The height of the rectangle
 */
void SGL_FillRect(uint32_t x, uint32_t y, uint32_t w, uint32_t h);

/**
 * @brief      Draws a rounded rectangle starting from position (x, y) with
 *             width w, height h and corner radius r
 *
 * @param[in]  x     The x position
 * @param[in]  y     The y position
 * @param[in]  w     The width of the rectangle
 * @param[in]  h     The height of the rectangle
 * @param[in]  r     The corner radius r
 */
void SGL_DrawRoundRect(int16_t x, int16_t y, int16_t w, int16_t h, int16_t r);

/**
 * @brief      Draws a rounded and filled rectangle starting from position
 *             (x, y) with width w, height h and corner radius r
 *
 * @param[in]  x     The x position
 * @param[in]  y     The y position
 * @param[in]  w     The width of the rectangle
 * @param[in]  h     The height of the rectangle
 * @param[in]  r     The corner radius r
 */
void SGL_FillRoundRect(int16_t x, int16_t y, int16_t w, int16_t h, int16_t r);

/**
 * @brief      Draws a triangle given three vertices (x0, y0), (x1, y1), (x2,
 *             y2)
 *
 * @param[in]  x0    The position x0
 * @param[in]  y0    The position y0
 * @param[in]  x1    The position x1
 * @param[in]  y1    The position y1
 * @param[in]  x2    The position x2
 * @param[in]  y2    The position y2
 */
void SGL_DrawTriangle(uint32_t x0, uint32_t y0, uint32_t x1, uint32_t y1, uint32_t x2, uint32_t y2);

/**
 * @brief      Draws a filled triangle given three vertices (x0, y0), (x1, y1),
 *             (x2, y2)
 *
 * @param[in]  x0    The position x0
 * @param[in]  y0    The position y0
 * @param[in]  x1    The position x1
 * @param[in]  y1    The position y1
 * @param[in]  x2    The position x2
 * @param[in]  y2    The position y2
 */
void SGL_FillTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2);

/**
 * @brief      Draws an ellipse given the center position (x, y), horizontal
 *             radius rx and vertical radius ry
 *
 * @param[in]  x0    The position x0
 * @param[in]  y0    The position y0
 * @param[in]  rx    The horizontal radius
 * @param[in]  ry    The vertical radius
 */
void SGL_DrawEllipse(int16_t x0, int16_t y0, int16_t rx, int16_t ry);

/**
 * @brief      Draws a filled ellipse given the center position (x, y),
 *             horizontal radius rx and vertical radius ry
 *
 * @param[in]  x0    The position x0
 * @param[in]  y0    The position y0
 * @param[in]  rx    The horizontal radius
 * @param[in]  ry    The vertical radius
 */
void SGL_FillEllipse(int16_t x0, int16_t y0, int16_t rx, int16_t ry);

/**
 * @brief      Draw a polygon of p vertices within a circle of radius centered
 *             in (x0, y0)
 *
 * @param[in]  x0    The midopoint position x0
 * @param[in]  y0    The midopoint position y0
 * @param[in]  r     The circle radius
 * @param[in]  p     The number of polygon vertices
 * @param[in]  d     The initial rotation of the polygon
 */
void SGL_DrawPolygonMidpoint(int16_t x0, int16_t y0, uint16_t r, uint16_t p, int16_t d);

/**
 * @brief      Draw a filled polygon of p vertices within a circle of radius
 *             centered in (x0, y0)
 *
 * @param[in]  x0    The midopoint position x0
 * @param[in]  y0    The midopoint position y0
 * @param[in]  r     The circle radius
 * @param[in]  p     The number of polygon vertices
 * @param[in]  d     The initial rotation of the polygon
 */
void SGL_FillPolygonMidpoint(int16_t x0, int16_t y0, uint16_t r, uint16_t p, int16_t d);

#endif
