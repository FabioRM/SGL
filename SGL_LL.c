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
******************************************************************************/

#include "SGL_LL.h"

void SGL_LL_Init(void)
{

}

void SGL_LL_Clear(uint32_t Color)
{
	BSP_LCD_Clear(Color);
}

uint32_t SGL_LL_GetPoint(uint32_t Xpos, uint32_t Ypos)
{
	return BSP_LCD_ReadPixel(Xpos, Ypos);
}

void SGL_LL_SetPoint(uint32_t Xpos, uint32_t Ypos, uint32_t color)
{
	BSP_LCD_DrawPixel(Xpos, Ypos, color);
}

void SGL_LL_Backlight(uint32_t val)
{

}

uint32_t SGL_LL_GetLcdXSize(void)
{
	return BSP_LCD_GetXSize();
}

uint32_t SGL_LL_GetLcdYSize(void)
{
	return BSP_LCD_GetYSize();
}

