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

#include "SGL.h"

/* Absolute value */
#define ABS(x)   ((x) > 0 ? (x) : -(x))

/* Private helper functions --------------------------------------------------*/
void SGL_FillCircleHelper(int16_t x0, int16_t y0, int16_t r, uint8_t cornername, int16_t delta);
void SGL_DrawCircleHelper(int16_t x0, int16_t y0, int16_t r, uint8_t cornername);
void swap(int16_t* a, int16_t* b);

/* Private variables ---------------------------------------------------------*/
uint32_t current_X, current_Y;
uint32_t MAX_X, MAX_Y;
uint32_t currentFrontColor, currentBackColor;
FONT_t currentFont;

/* Functions -----------------------------------------------------------------*/
void SGL_Init(void)
{
	current_X = 0;
	current_Y = 0;
	currentFont = SGL_DEFAULT_FONT;
	currentFrontColor = SGL_COLOR_WHITE;
	currentBackColor = SGL_COLOR_BLACK;
	MAX_X = SGL_LL_GetLcdXSize();
	MAX_Y = SGL_LL_GetLcdYSize();
	SGL_LL_Init();
}

void SGL_SetFont(FONT_t font)
{
	currentFont = font;
}

void SGL_SetFrontColor(uint32_t color)
{
	currentFrontColor = color;
}

void SGL_SetBackColor(uint32_t color)
{
	currentBackColor = color;
}

void SGL_DrawPoint(uint32_t x, uint32_t y)
{
	SGL_LL_SetPoint(x, y, currentFrontColor);
}

void SGL_ClearScreen(void)
{
	SGL_LL_Clear(currentBackColor);
}

void SGL_DrawLine(uint32_t x0, uint32_t y0, uint32_t x1, uint32_t y1)
{
	// Bresenham's line algorithm
	// from rosettacode.org
	int dx = ABS(x1 - x0), sx = x0 < x1 ? 1 : -1;
	int dy = ABS(y1 - y0), sy = y0 < y1 ? 1 : -1;
	int err = (dx > dy ? dx : -dy) / 2, e2;

	for (;;) {
		SGL_LL_SetPoint(x0, y0, currentFrontColor);
		if (x0 == x1 && y0 == y1)
			break;
		e2 = err;
		if (e2 > -dx) {
			err -= dy;
			x0 += sx;
		}
		if (e2 < dy) {
			err += dx;
			y0 += sy;
		}
	}
}

void SGL_PutChar(uint32_t Xpos, uint32_t Ypos, uint8_t ASCII)
{
	uint16_t line, column, bytesPerLine, bufferPosition;
	uint8_t buffer[1024];

	GetASCIICode(buffer, ASCII, currentFont);

	bytesPerLine = ((currentFont.Width - 1) / 8) + 1;

	for(line = 0; line < currentFont.Height; line++)
	{
		for(column = 0; column < currentFont.Width; column++)
		{
			bufferPosition = line * bytesPerLine + (column / 8);
			if (((buffer[bufferPosition] >> (column % 8)) & 0x01) > 0)
			{
				SGL_LL_SetPoint(Xpos + column, Ypos + line, currentFrontColor);
			}
			else
			{
				SGL_LL_SetPoint(Xpos + column, Ypos + line, currentBackColor);
			}

		}
	}
}

void SGL_CleanPutChar(uint32_t Xpos, uint32_t Ypos, uint8_t ASCII)
{
	uint16_t line, column, bytesPerLine, bufferPosition;
	uint8_t buffer[1024];

	GetASCIICode(buffer, ASCII, currentFont);

	bytesPerLine = ((currentFont.Width - 1) / 8) + 1;

	for(line = 0; line < currentFont.Height; line++)
	{
		for(column = 0; column < currentFont.Width; column++)
		{
			bufferPosition = line * bytesPerLine + (column / 8);
			if (((buffer[bufferPosition] >> (column % 8)) & 0x01) > 0)
			{
				SGL_DrawPoint(Xpos + column, Ypos + line);
			}
		}
	}
}

void SGL_Text(uint32_t Xpos, uint32_t Ypos, uint8_t *str)
{
	uint8_t TempChar;

	do {
		TempChar = *str++;

		if (TempChar == 13 || TempChar == 10) {
			Xpos = 0;
			Ypos += currentFont.Height;
		} else if (TempChar == 9) {
			Xpos += 4 * currentFont.Width;
		} else {
			SGL_PutChar(Xpos, Ypos, TempChar);
			if (Xpos < MAX_X - (currentFont.Width + (currentFont.Width / 2))) {
				Xpos += currentFont.Width;
			} else if (Ypos < MAX_Y - currentFont.Height) {
				Xpos = 0;
				Ypos += currentFont.Height;
			} else {
				Xpos = 0;
				Ypos = 0;
			}
		}
	} while (*str != 0);
}

void SGL_CleanText(uint32_t Xpos, uint32_t Ypos, uint8_t *str)
{
	uint8_t TempChar;

	do {
		TempChar = *str++;

		if (TempChar == 13 || TempChar == 10) {
			Xpos = 0;
			Ypos += currentFont.Height;
		} else if (TempChar == 9) {
			Xpos += 4 * currentFont.Width;
		} else {
			SGL_CleanPutChar(Xpos, Ypos, TempChar);
			if (Xpos < MAX_X - (currentFont.Width + (currentFont.Width / 2))) {
				Xpos += currentFont.Width;
			} else if (Ypos < MAX_Y - currentFont.Height) {
				Xpos = 0;
				Ypos += currentFont.Height;
			} else {
				Xpos = 0;
				Ypos = 0;
			}
		}
	} while (*str != 0);
}

void SGL_DrawPicture(uint32_t x, uint32_t y, uint32_t w, uint32_t h,
		const uint32_t *pic) {
	uint32_t x_index, y_index;
	uint32_t pic_index = 0;

	//for(y_index = y; y_index < (y+h); y_index++)
	//if wrong orientation
	for (y_index = (y + h); y_index > 0; y_index--) {
		for (x_index = x; x_index < (x + w); x_index++) {
			SGL_LL_SetPoint(x_index, y_index, pic[pic_index]);
			pic_index++;
		}
	}
}

void SGL_DrawPicture8bit(uint32_t x, uint32_t y, uint32_t w, uint32_t h,
		const uint8_t *pic) {
	uint32_t color;
	uint32_t x_index, y_index;
	uint32_t pic_index = 0;

	//for(y_index = y; y_index < (y+h); y_index++)
	//if wrong orientation
	for (y_index = (y + h); y_index > 0; y_index--) {
		for (x_index = x; x_index < (x + w); x_index++) {
			color = (pic[pic_index + 1] << 8) | (pic[pic_index]);
			SGL_LL_SetPoint(x_index, y_index, color);
			pic_index += 2;
		}
	}
}

void SGL_DrawCircle(uint32_t x0, uint32_t y0, uint32_t r)
{
	int16_t f = 1 - r;
	int16_t ddF_x = 1;
	int16_t ddF_y = -2 * (int16_t)r;
	int16_t x = 0;
	int16_t y = r;

	SGL_LL_SetPoint(x0, y0 + r, currentFrontColor);
	SGL_LL_SetPoint(x0, y0 - r, currentFrontColor);
	SGL_LL_SetPoint(x0 + r, y0, currentFrontColor);
	SGL_LL_SetPoint(x0 - r, y0, currentFrontColor);

	while (x < y) {
		if (f >= 0) {
			y--;
			ddF_y += 2;
			f += ddF_y;
		}
		x++;
		ddF_x += 2;
		f += ddF_x;

		SGL_LL_SetPoint(x0 + x, y0 + y, currentFrontColor);
		SGL_LL_SetPoint(x0 - x, y0 + y, currentFrontColor);
		SGL_LL_SetPoint(x0 + x, y0 - y, currentFrontColor);
		SGL_LL_SetPoint(x0 - x, y0 - y, currentFrontColor);

		SGL_LL_SetPoint(x0 + y, y0 + x, currentFrontColor);
		SGL_LL_SetPoint(x0 - y, y0 + x, currentFrontColor);
		SGL_LL_SetPoint(x0 + y, y0 - x, currentFrontColor);
		SGL_LL_SetPoint(x0 - y, y0 - x, currentFrontColor);
	}
}

void SGL_FillCircle(uint32_t x0, uint32_t y0, uint32_t r)
{
	int16_t f = 1 - r;
	int16_t ddF_x = 1;
	int16_t ddF_y = -2 * (int16_t)r;
	int16_t x = 0;
	int16_t y = r;

	int16_t i;

	for (i = y0 - r; i <= y0 + r; i++)
		SGL_LL_SetPoint(x0, i, currentFrontColor);

	while (x < y) {
		if (f >= 0) {
			y--;
			ddF_y += 2;
			f += ddF_y;
		}
		x++;
		ddF_x += 2;
		f += ddF_x;

		for (i = y0 - y; i <= y0 + y; i++) {
			SGL_LL_SetPoint(x0 + x, i, currentFrontColor);
			SGL_LL_SetPoint(x0 - x, i, currentFrontColor);
		}
		for (i = y0 - x; i <= y0 + x; i++) {
			SGL_LL_SetPoint(x0 + y, i, currentFrontColor);
			SGL_LL_SetPoint(x0 - y, i, currentFrontColor);
		}
	}
}

void SGL_DrawRect(uint32_t x, uint32_t y, uint32_t w, uint32_t h)
{
	if (x > MAX_X)
		x = MAX_X;
	if (y > MAX_Y)
		y = MAX_Y;

	if ((x + w) > MAX_X)
		w = MAX_X - x;

	if ((y + h) > MAX_Y)
		h = MAX_Y - y;

	SGL_DrawLine(x, y, x, y + h);
	SGL_DrawLine(x, y, x + w, y);
	SGL_DrawLine(x + w, y + h, x, y + h);
	SGL_DrawLine(x + w, y + h, x + w, y);
}

void SGL_FillRect(uint32_t x, uint32_t y, uint32_t w, uint32_t h)
{
	uint32_t x_index, y_index;

	if (x > MAX_X)
		x = MAX_X;
	if (y > MAX_Y)
		y = MAX_Y;

	if ((x + w) > MAX_X)
		w = MAX_X - x;

	if ((y + h) > MAX_Y)
		h = MAX_Y - y;

	for (x_index = x; x_index <= x + w; x_index++) {
		for (y_index = y; y_index <= y + h; y_index++) {
			SGL_LL_SetPoint(x_index, y_index, currentFrontColor);
		}
	}

}

void SGL_DrawTriangle(uint32_t x0, uint32_t y0, uint32_t x1, uint32_t y1, uint32_t x2, uint32_t y2)
{
	if (x0 > MAX_X)
		x0 = MAX_X;
	if (y0 > MAX_Y)
		y0 = MAX_Y;
	if (x1 > MAX_X)
		x1 = MAX_X;
	if (y1 > MAX_Y)
		y1 = MAX_Y;
	if (x2 > MAX_X)
		x2 = MAX_X;
	if (y2 > MAX_Y)
		y2 = MAX_Y;

	SGL_DrawLine(x0, y0, x1, y1);
	SGL_DrawLine(x0, y0, x2, y2);
	SGL_DrawLine(x2, y2, x1, y1);
}

void SGL_DrawEllipse(int16_t x0, int16_t y0, int16_t rx, int16_t ry)
{
	if (rx < 2)
		return;
	if (ry < 2)
		return;
	int16_t x, y;
	int32_t rx2 = rx * rx;
	int32_t ry2 = ry * ry;
	int32_t fx2 = 4 * rx2;
	int32_t fy2 = 4 * ry2;
	int32_t s;

	for (x = 0, y = ry, s = 2 * ry2 + rx2 * (1 - 2 * ry); ry2 * x <= rx2 * y;
			x++) {
		SGL_LL_SetPoint(x0 + x, y0 + y, currentFrontColor);
		SGL_LL_SetPoint(x0 - x, y0 + y, currentFrontColor);
		SGL_LL_SetPoint(x0 + x, y0 - y, currentFrontColor);
		SGL_LL_SetPoint(x0 - x, y0 - y, currentFrontColor);
		if (s >= 0) {
			s += fx2 * (1 - y);
			y--;
		}
		s += ry2 * ((4 * x) + 6);
	}

	for (x = rx, y = 0, s = 2 * rx2 + ry2 * (1 - 2 * rx); rx2 * y <= ry2 * x;
			y++) {
		SGL_LL_SetPoint(x0 + x, y0 + y, currentFrontColor);
		SGL_LL_SetPoint(x0 - x, y0 + y, currentFrontColor);
		SGL_LL_SetPoint(x0 + x, y0 - y, currentFrontColor);
		SGL_LL_SetPoint(x0 - x, y0 - y, currentFrontColor);
		if (s >= 0) {
			s += fy2 * (1 - x);
			x--;
		}
		s += rx2 * ((4 * y) + 6);
	}
}

void SGL_FillEllipse(int16_t x0, int16_t y0, int16_t rx, int16_t ry)
{
	if (rx < 2)
		return;
	if (ry < 2)
		return;
	int16_t x, y;
	int32_t rx2 = rx * rx;
	int32_t ry2 = ry * ry;
	int32_t fx2 = 4 * rx2;
	int32_t fy2 = 4 * ry2;
	int32_t s;

	for (x = 0, y = ry, s = 2 * ry2 + rx2 * (1 - 2 * ry); ry2 * x <= rx2 * y;
			x++) {
		SGL_DrawLine(x0 - x, y0 - y, x0 + x + 1, y0 - y);
		SGL_DrawLine(x0 - x, y0 + y, x0 + x + 1, y0 + y);

		if (s >= 0) {
			s += fx2 * (1 - y);
			y--;
		}
		s += ry2 * ((4 * x) + 6);
	}

	for (x = rx, y = 0, s = 2 * rx2 + ry2 * (1 - 2 * rx); rx2 * y <= ry2 * x;
			y++) {
		SGL_DrawLine(x0 - x, y0 - y, x0 + x + 1, y0 - y);
		SGL_DrawLine(x0 - x, y0 + y, x0 + x + 1, y0 + y);

		if (s >= 0) {
			s += fy2 * (1 - x);
			x--;
		}
		s += rx2 * ((4 * y) + 6);
	}

}

void SGL_DrawCircleHelper(int16_t x0, int16_t y0, int16_t r, uint8_t cornername)
{
	int16_t f = 1 - r;
	int16_t ddF_x = 1;
	int16_t ddF_y = -r - r;
	int16_t x = 0;

	while (x < r) {
		if (f >= 0) {
			r--;
			ddF_y += 2;
			f += ddF_y;
		}
		x++;
		ddF_x += 2;
		f += ddF_x;
		if (cornername & 0x4) {
			SGL_LL_SetPoint(x0 + x, y0 + r, currentFrontColor);
			SGL_LL_SetPoint(x0 + r, y0 + x, currentFrontColor);
		}
		if (cornername & 0x2) {
			SGL_LL_SetPoint(x0 + x, y0 - r, currentFrontColor);
			SGL_LL_SetPoint(x0 + r, y0 - x, currentFrontColor);
		}
		if (cornername & 0x8) {
			SGL_LL_SetPoint(x0 - r, y0 + x, currentFrontColor);
			SGL_LL_SetPoint(x0 - x, y0 + r, currentFrontColor);
		}
		if (cornername & 0x1) {
			SGL_LL_SetPoint(x0 - r, y0 - x, currentFrontColor);
			SGL_LL_SetPoint(x0 - x, y0 - r, currentFrontColor);
		}
	}
}

void SGL_FillCircleHelper(int16_t x0, int16_t y0, int16_t r, uint8_t cornername, int16_t delta)
{
	int16_t f = 1 - r;
	int16_t ddF_x = 1;
	int16_t ddF_y = -r - r;
	int16_t x = 0;

	delta++;
	while (x < r) {
		if (f >= 0) {
			r--;
			ddF_y += 2;
			f += ddF_y;
		}
		x++;
		ddF_x += 2;
		f += ddF_x;

		if (cornername & 0x1) {
			SGL_DrawLine(x0 + x, y0 - r, x0 + x, y0 + r + delta);
			SGL_DrawLine(x0 + r, y0 - x, x0 + r, y0 + x + delta);
		}
		if (cornername & 0x2) {
			SGL_DrawLine(x0 - x, y0 - r, x0 - x, y0 + r + delta);
			SGL_DrawLine(x0 - r, y0 - x, x0 - r, y0 + x + delta);
		}
	}
}

void SGL_DrawRoundRect(int16_t x, int16_t y, int16_t w, int16_t h, int16_t r)
{
	if (w < 2 * r || h < 2 * r)
		return; // Stop iteration problems on corners
	// smarter version
	SGL_DrawLine(x + r, y, x + w - r, y); // Top
	SGL_DrawLine(x + r, y + h - 1, x + w - r, y + h - 1); // Bottom
	SGL_DrawLine(x, y + r, x, y + h - r); // Left
	SGL_DrawLine(x + w - 1, y + r, x + w - 1, y + h - r); // Right
	// draw four corners
	SGL_DrawCircleHelper(x + r, y + r, r, 1);
	SGL_DrawCircleHelper(x + w - r - 1, y + r, r, 2);
	SGL_DrawCircleHelper(x + w - r - 1, y + h - r - 1, r, 4);
	SGL_DrawCircleHelper(x + r, y + h - r - 1, r, 8);
}

void SGL_FillRoundRect(int16_t x, int16_t y, int16_t w, int16_t h, int16_t r)
{
	if (w < 2 * r || h < 2 * r)
		return; // Stop iteration problems on corners

	// smarter version
	SGL_FillRect(x + r, y, w - r - r, h);

	// draw four corners
	SGL_FillCircleHelper(x + w - r - 1, y + r, r, 1, h - r - r - 1);
	SGL_FillCircleHelper(x + r, y + r, r, 2, h - r - r - 1);
}

void SGL_FillTriangle(int16_t x1, int16_t y1, int16_t x2, int16_t y2, int16_t x3, int16_t y3)
{
	int16_t t1x, t2x, y, minx, maxx, t1xp, t2xp;
	uint8_t changed1 = 0x00;
	uint8_t changed2 = 0x00;
	int16_t signx1, signx2, dx1, dy1, dx2, dy2;
	uint32_t e1, e2;
	// Sort vertices
	if (y1 > y2) {
		swap(&y1, &y2);
		swap(&x1, &x2);
	}
	if (y1 > y3) {
		swap(&y1, &y3);
		swap(&x1, &x3);
	}
	if (y2 > y3) {
		swap(&y2, &y3);
		swap(&x2, &x3);
	}

	t1x = t2x = x1;
	y = y1;   // Starting points

	dx1 = x2 - x1;
	if (dx1 < 0) {
		dx1 = -dx1;
		signx1 = -1;
	} else
		signx1 = 1;
	dy1 = y2 - y1;

	dx2 = x3 - x1;
	if (dx2 < 0) {
		dx2 = -dx2;
		signx2 = -1;
	} else
		signx2 = 1;
	dy2 = y3 - y1;

	if (dy1 > dx1) {   // swap values
		swap(&dx1, &dy1);
		changed1 = 0x01;
	}
	if (dy2 > dx2) {   // swap values
		swap(&dy2, &dx2);
		changed2 = 0x01;
	}

	e2 = dx2 >> 1;
	// Flat top, just process the second half
	if (y1 == y2)
		goto next;
	e1 = dx1 >> 1;

	for (uint32_t i = 0; i < dx1;) {
		t1xp = 0;
		t2xp = 0;
		if (t1x < t2x) {
			minx = t1x;
			maxx = t2x;
		} else {
			minx = t2x;
			maxx = t1x;
		}
		// process first line until y value is about to change
		while (i < dx1) {
			i++;
			e1 += dy1;
			while (e1 >= dx1) {
				e1 -= dx1;
				if (changed1)
					t1xp = signx1;   //t1x += signx1;
				else
					goto next1;
			}
			if (changed1)
				break;
			else
				t1x += signx1;
		}
		// Move line
		next1:
		// process second line until y value is about to change
		while (1) {
			e2 += dy2;
			while (e2 >= dx2) {
				e2 -= dx2;
				if (changed2)
					t2xp = signx2;   //t2x += signx2;
				else
					goto next2;
			}
			if (changed2)
				break;
			else
				t2x += signx2;
		}
		next2: if (minx > t1x)
			minx = t1x;
		if (minx > t2x)
			minx = t2x;
		if (maxx < t1x)
			maxx = t1x;
		if (maxx < t2x)
			maxx = t2x;
		SGL_DrawLine(minx, y, maxx, y); // Draw line from min to max points found on the y
		// Now increase y
		if (!changed1)
			t1x += signx1;
		t1x += t1xp;
		if (!changed2)
			t2x += signx2;
		t2x += t2xp;
		y += 1;
		if (y == y2)
			break;

	}
	next:
	// Second half
	dx1 = x3 - x2;
	if (dx1 < 0) {
		dx1 = -dx1;
		signx1 = -1;
	} else
		signx1 = 1;
	dy1 = y3 - y2;
	t1x = x2;

	if (dy1 > dx1) {   // swap values
		swap(&dy1, &dx1);
		changed1 = 0x01;
	} else
		changed1 = 0x00;

	e1 = dx1 >> 1;

	for (uint32_t i = 0; i <= dx1; i++) {
		t1xp = 0;
		t2xp = 0;
		if (t1x < t2x) {
			minx = t1x;
			maxx = t2x;
		} else {
			minx = t2x;
			maxx = t1x;
		}
		// process first line until y value is about to change
		while (i < dx1) {
			e1 += dy1;
			while (e1 >= dx1) {
				e1 -= dx1;
				if (changed1) {
					t1xp = signx1;
					break;
				}   //t1x += signx1;
				else
					goto next3;
			}
			if (changed1)
				break;
			else
				t1x += signx1;
			if (i < dx1)
				i++;
		}
		next3:
		// process second line until y value is about to change
		while (t2x != x3) {
			e2 += dy2;
			while (e2 >= dx2) {
				e2 -= dx2;
				if (changed2)
					t2xp = signx2;
				else
					goto next4;
			}
			if (changed2)
				break;
			else
				t2x += signx2;
		}
		next4:

		if (minx > t1x)
			minx = t1x;
		if (minx > t2x)
			minx = t2x;
		if (maxx < t1x)
			maxx = t1x;
		if (maxx < t2x)
			maxx = t2x;
		SGL_DrawLine(minx, y, maxx, y); // Draw line from min to max points found on the y
		// Now increase y
		if (!changed1)
			t1x += signx1;
		t1x += t1xp;
		if (!changed2)
			t2x += signx2;
		t2x += t2xp;
		y += 1;
		if (y > y3)
			return;
	}
}

void swap(int16_t* a, int16_t* b) {
	int16_t temp = *a;
	*a = *b;
	*b = temp;
}
