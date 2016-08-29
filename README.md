#SGL - Simple Graphical Library


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
