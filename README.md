******************************************************************************
Copyright (c) 2016 - Fabio Angeletti

e-mail: fabio.angeletti89@gmail.com


I'm not the owner of the whole code. See individual files for more info.


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
*******************************************************************************


SGL - Simple Graphical Library


A low-level agnostic library that provides basic graphical functionalities to work with graphical displays.


It consists of only three parts:

		- SGL

		- SGL_LL

		- fontLib

SGL.c / SGL.h

		provides functions to draw circles, triangles, rects, ellipses, points and text
		the header is commented and the names of the variables should be easily readable

SGL_LL.c / SGL_LL.h

		these files must be tailored to your specific application
		the header is commented and the names of the variables should be easily readable

fontLib.c / fontLib.h

		these files are generated through the python script "generateFontsLib.py".



******************************************************************************

Generating fontLib.c / fontLib.h


1) place one or more font file in the same directory as "generateFontsLib.py"


2) run the python script


supports ".ttf" and ".otf"


it is possible to customize the font generation working on the script


as example: changing font sizes





PLEASE NOTE that SGL.h contains this line:


\#define SGL_DEFAULT_FONT (FONT_TMSBLD_24)


which parameter must be modified to one of your chosen font's name.
The list of all generated fonts is in fontLib.h
