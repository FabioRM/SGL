#!/usr/bin/env python
# -*- coding: utf-8 -*-
# Needs freetype-py>=1.0




#############################################################################
# Modified by Fabio Angeletti 												#
# e-mail: fabio.angeletti89@gmail.com 										#
# For more info see:														#
# http://fabioangeletti.altervista.org 										#
#############################################################################




# For more info see:
# http://dbader.org/blog/monochrome-font-rendering-with-freetype-and-python

# The MIT License (MIT)
#
# Copyright (c) 2013 Daniel Bader (http://dbader.org)
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in
# all copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
# THE SOFTWARE.

import freetype
import glob, os
import time

class Bitmap(object):
	"""
	A 2D bitmap image represented as a list of byte values. Each byte indicates the state
	of a single pixel in the bitmap. A value of 0 indicates that the pixel is `off`
	and any other value indicates that it is `on`.
	"""
	def __init__(self, width, height, pixels=None):
		self.width = width
		self.height = height
		self.pixels = pixels or bytearray(width * height)

	def asBytes(self, w, h):
		#my mod now is correct!!!!!! yuppieeeeeeee!!!!!
		s = int(((w-1) / 8) + 1)
		bBytes = bytearray(h * s)

		#print "s size: %d" % s

		for y in range(h):
			for x in range(w):
				pos = y * s + (x/8)
				if(self.pixels[y * w + x]):
					bBytes[pos] = bBytes[pos] | (1 << (x%8))

		return bBytes

	def bitblt(self, src, x, y):
		"""Copy all pixels from `src` into this bitmap"""
		srcpixel = 0
		dstpixel = y * self.width + x
		row_offset = self.width - src.width

		for sy in range(src.height):
			for sx in range(src.width):
				# Perform an OR operation on the destination pixel and the source pixel
				# because glyph bitmaps may overlap if character kerning is applied, e.g.
				# in the string "AVA", the "A" and "V" glyphs must be rendered with
				# overlapping bounding boxes.
				self.pixels[dstpixel] = self.pixels[dstpixel] or src.pixels[srcpixel]
				srcpixel += 1
				dstpixel += 1
			dstpixel += row_offset

class Glyph(object):
	def __init__(self, pixels, width, height, top, advance_width):
		self.bitmap = Bitmap(width, height, pixels)

		# The glyph bitmap's top-side bearing, i.e. the vertical distance from the
		# baseline to the bitmap's top-most scanline.
		self.top = top

		# Ascent and descent determine how many pixels the glyph extends
		# above or below the baseline.
		self.descent = max(0, self.height - self.top)
		self.ascent = max(0, max(self.top, self.height) - self.descent)

		# The advance width determines where to place the next character horizontally,
		# that is, how many pixels we move to the right to draw the next glyph.
		self.advance_width = advance_width

	@property
	def width(self):
		return self.bitmap.width

	@property
	def height(self):
		return self.bitmap.height

	@staticmethod
	def from_glyphslot(slot):
		"""Construct and return a Glyph object from a FreeType GlyphSlot."""
		pixels = Glyph.unpack_mono_bitmap(slot.bitmap)
		width, height = slot.bitmap.width, slot.bitmap.rows
		top = slot.bitmap_top

		# The advance width is given in FreeType's 26.6 fixed point format,
		# which means that the pixel values are multiples of 64.
		advance_width = slot.advance.x / 64

		return Glyph(pixels, width, height, top, advance_width)

	@staticmethod
	def unpack_mono_bitmap(bitmap):
		"""
		Unpack a freetype FT_LOAD_TARGET_MONO glyph bitmap into a bytearray where each
		pixel is represented by a single byte.
		"""
		# Allocate a bytearray of sufficient size to hold the glyph bitmap.
		data = bytearray(bitmap.rows * bitmap.width)

		# Iterate over every byte in the glyph bitmap. Note that we're not
		# iterating over every pixel in the resulting unpacked bitmap --
		# we're iterating over the packed bytes in the input bitmap.
		for y in range(bitmap.rows):
			for byte_index in range(bitmap.pitch):

				# Read the byte that contains the packed pixel data.
				byte_value = bitmap.buffer[y * bitmap.pitch + byte_index]

				# We've processed this many bits (=pixels) so far. This determines
				# where we'll read the next batch of pixels from.
				num_bits_done = byte_index * 8

				# Pre-compute where to write the pixels that we're going
				# to unpack from the current byte in the glyph bitmap.
				rowstart = y * bitmap.width + byte_index * 8

				# Iterate over every bit (=pixel) that's still a part of the
				# output bitmap. Sometimes we're only unpacking a fraction of a byte
				# because glyphs may not always fit on a byte boundary. So we make sure
				# to stop if we unpack past the current row of pixels.
				for bit_index in range(min(8, bitmap.width - num_bits_done)):

					# Unpack the next pixel from the current glyph byte.
					bit = byte_value & (1 << (7 - bit_index))

					# Write the pixel to the output bytearray. We ensure that `off`
					# pixels have a value of 0 and `on` pixels have a value of 1.
					data[rowstart + bit_index] = 1 if bit else 0

		return data


class Font(object):
	def __init__(self, filename, size):
		self.face = freetype.Face(filename)
		self.face.set_pixel_sizes(0, size)

	def glyph_for_character(self, char):
		# Let FreeType load the glyph for the given character and tell it to render
		# a monochromatic bitmap representation.
		self.face.load_char(char, freetype.FT_LOAD_RENDER | freetype.FT_LOAD_TARGET_MONO)
		return Glyph.from_glyphslot(self.face.glyph)

	def render_character(self, char):
		glyph = self.glyph_for_character(char)
		return glyph.bitmap

	def kerning_offset(self, previous_char, char):
		"""
		Return the horizontal kerning offset in pixels when rendering `char`
		after `previous_char`.

		Use the resulting offset to adjust the glyph's drawing position to
		reduces extra diagonal whitespace, for example in the string "AV" the
		bitmaps for "A" and "V" may overlap slightly with some fonts. In this
		case the glyph for "V" has a negative horizontal kerning offset as it is
		moved slightly towards the "A".
		"""
		kerning = self.face.get_kerning(previous_char, char)

		# The kerning offset is given in FreeType's 26.6 fixed point format,
		# which means that the pixel values are multiples of 64.
		return kerning.x / 64

	def text_dimensions(self, text):
		"""Return (width, height, baseline) of `text` rendered in the current font."""
		width = 0
		max_ascent = 0
		max_descent = 0
		previous_char = None

		# For each character in the text string we get the glyph
		# and update the overall dimensions of the resulting bitmap.
		for char in text:
			glyph = self.glyph_for_character(char)
			max_ascent = max(max_ascent, glyph.ascent)
			max_descent = max(max_descent, glyph.descent)
			kerning_x = self.kerning_offset(previous_char, char)

			# With kerning, the advance width may be less than the width of the glyph's bitmap.
			# Make sure we compute the total width so that all of the glyph's pixels
			# fit into the returned dimensions.
			width += max(glyph.advance_width + kerning_x, glyph.width + kerning_x)

			previous_char = char

		height = max_ascent + max_descent
		return (width, height, max_descent)

	def render_text(self, text, width=None, height=None, baseline=None):
		"""
		Render the given `text` into a Bitmap and return it.

		If `width`, `height`, and `baseline` are not specified they are computed using
		the `text_dimensions' method.
		"""
		if None in (width, height, baseline):
			width, height, baseline = self.text_dimensions(text)

		x = 0
		previous_char = None
		outbuffer = Bitmap(width, height)

		for char in text:
			glyph = self.glyph_for_character(char)

			# Take kerning information into account before we render the
			# glyph to the output bitmap.
			x += self.kerning_offset(previous_char, char)

			# The vertical drawing position should place the glyph
			# on the baseline as intended.
			y = height - glyph.ascent - baseline

			outbuffer.bitblt(glyph.bitmap, x, y)

			x += glyph.advance_width
			previous_char = char

		return outbuffer


def genCharTable(fontFilename, fontName, fontSize):
	chars = ""
	for index in range(32,126):
		chars = chars + "%c" % index

	maxWidth = 0
	maxHeight = 0

	f = Font(fontFilename, fontSize)

	_, maxHeight, baseline = f.text_dimensions(chars)

	for c in chars:
		cWidth, _, _ = f.text_dimensions(c)
		if(cWidth > maxWidth):
			maxWidth = cWidth

	#maxWidth = maxWidth + 1

	tableName = "%s_TABLE" % (fontName)
	s = 'const uint8_t %s[] =\n{\n' % tableName
	for c in chars:
		buff = (f.render_text(c,maxWidth,maxHeight,baseline)).asBytes(maxWidth, maxHeight)
		s = s + '\t'
		for b in buff:
			s = s + "0x%02X," % b
		s = s + "\n"

	s = s + '};\n\n'

	return s, maxWidth, maxHeight

def genCharStruct(fontName, width, height):
	s = "FONT_t %s =\n{\n" % fontName
	s = s + "\t%s_TABLE,\n\t%d,\n\t%d,\n};\n\n" % (fontName, width, height)
	return s

def processFont(fontFilename, fontSize):
	fontName = "FONT_" + fontFilename.replace(' ', '').replace('-', '_').replace('.ttf', '').replace('.otf','').replace('.TTF', '').replace('.OTF','').upper() + "_%d" % fontSize
	data, maxWidth, maxHeight = genCharTable(fontFilename, fontName, fontSize) 
	cStr = data + genCharStruct(fontName, maxWidth, maxHeight)
	hStr = "extern FONT_t %s;\n" % fontName
	return cStr, hStr

def getHeaders():
	hHead = """#ifndef __FONTLIB_H__\n#define __FONTLIB_H__\n\n#ifdef __cplusplus\nextern "C" {\n#endif\n\n/* Includes ------------------------------------------------------------------*/\n#include <stdint.h>\n#include "string.h"\n\n/* Exported types ------------------------------------------------------------*/\ntypedef struct tFont\n{\n\tconst uint8_t* table;\n\tuint8_t Width;\n\tuint8_t Height;\n}FONT_t;\n\n"""
	cHead = """/* Includes ------------------------------------------------------------------*/\n#include "fontLib.h"\n\n"""
	return cHead, hHead

def getFooters():
	hFoot = """\n/* Private function prototypes -----------------------------------------------*/\nvoid GetASCIICode(uint8_t* pBuffer, uint8_t ASCII, struct tFont FONTx);\n\n#ifdef __cplusplus\n}\n#endif\n\n#endif\n\n"""
	cFoot = """void GetASCIICode(uint8_t* pBuffer, uint8_t ASCII, FONT_t FONTx)\n{\n\tuint16_t pos, index, stepSize;\n\tpos = ASCII - 32;\n\tstepSize = FONTx.Height * (((FONTx.Width - 1) / 8) + 1);\n\n\tfor(index = 0; index < stepSize; index++)\n\t\tpBuffer[index] = FONTx.table[pos * stepSize + index];\n}\n"""
	return cFoot, hFoot

def getFonts():
	files = glob.glob('*.ttf')
	files.extend(glob.glob('*.otf'))
	files.extend(glob.glob('*.TTF'))
	files.extend(glob.glob('*.OTF'))
	return files

if __name__ == '__main__':

	files = getFonts()

	if(len(files) == 1):
		print "Found only 1 file to process"
	elif(len(files) == 0):
		print "No fonts in the current directory"
	else:
		print "Found %d files to process" % len(files)

	if(len(files) != 0):

		cLibFile = open('fontLib.c', 'w+')
		hLibFile = open('fontLib.h', 'w+')

		cStr, hStr = getHeaders()
		cLibFile.write(cStr)
		hLibFile.write(hStr)

		sTime = time.time()

		for file in files:
			print "\tProcessing %s..." % file

			#suggested 8,12,16,24,32,48,64
			#compact 8,12,16,24
			for size in (8,12,16,24):
				cStr, hStr = processFont(file, size)
				cLibFile.write(cStr)
				hLibFile.write(hStr)

		if((time.time()-sTime) < 1):
			print "DONE in less than a second"
		else:
			print "DONE in %d seconds" % (time.time()-sTime)

		cStr, hStr = getFooters()
		cLibFile.write(cStr)
		hLibFile.write(hStr)
		
		cLibFile.close()
		hLibFile.close()