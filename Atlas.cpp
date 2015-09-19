/* 
 * File:   Atlas.cpp
 * Author: dude719
 *
 * Created on April 19, 2015, 6:14 AM
 */

#include "Atlas.h"

const int Atlas::MAXWIDTH = 1024;

Atlas::Atlas(Font font, int fontSize) 
{
	FT_Face fontFace = font.GetFace();
	FT_Set_Pixel_Sizes(fontFace, 0, fontSize);
	FT_GlyphSlot g = fontFace->glyph;

	strcpy(name, font.GetName());
	char szFontSize[32];
	itoa(fontSize, szFontSize, 10);
	strcat(name, szFontSize);
	
	printf("Atlas: %s\n", name);
	
	face = fontFace;
	useKerning = face->face_flags & FT_FACE_FLAG_KERNING;
	
	width = 0;
	height = 0;

	maxcharheight = 0;

	memset(c, 0, sizeof(c));

	int roww = 0;
	int rowh = 0;

	/* Find minimum size for a texture holding all visible ASCII characters */
	for (int i = 32; i < 128; i++) 
	{
		if (FT_Load_Char(fontFace, i, FT_LOAD_RENDER)) 
		{
			printf("Loading character %c failed!\n", i);
			continue;
		}

		if (roww + g->bitmap.width + 1 >= MAXWIDTH) 
		{
			width = width > roww ? width : roww;
			height += rowh;
			roww = 0;
			rowh = 0;
		}

		roww += g->bitmap.width + 1;
		rowh = rowh > g->bitmap.rows + 1 ? rowh : g->bitmap.rows + 1;
		maxcharheight = maxcharheight > g->bitmap.rows + 1 ? maxcharheight : g->bitmap.rows + 1;
	}

	printf("maxcharheight: %i\n", maxcharheight);

	width = width > roww ? width : roww;
	height += rowh;

	/* Create a texture that will be used to hold all ASCII glyphs */
	glActiveTexture(GL_TEXTURE0);
	glGenTextures(1, &tex);
	glBindTexture(GL_TEXTURE_2D, tex);
	//glUniform1i(uniform_tex, 0);

	glGenSamplers(1, &sampler);
	glSamplerParameteri(sampler, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glSamplerParameteri(sampler, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glSamplerParameteri(sampler, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glSamplerParameteri(sampler, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindSampler(0, sampler);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, width, height, 0, GL_RED, GL_UNSIGNED_BYTE, 0);

	/* We require 1 byte alignment when uploading texture data */
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glPixelStorei(GL_PACK_ALIGNMENT, 1);
    
	/* Paste all glyph bitmaps into the texture, remembering the offset */
	int ox = 0;
	int oy = 0;

	rowh = 0;

	for (int i = 32; i < 128; i++) 
	{
		if (FT_Load_Char(fontFace, i, FT_LOAD_RENDER)) 
		{
			printf("Loading character %c failed!\n", i);
			continue;
		}

		if (ox + g->bitmap.width + 1 >= MAXWIDTH) 
		{
			oy += rowh;
			rowh = 0;
			ox = 0;
		}

		glTexSubImage2D(GL_TEXTURE_2D, 0, ox, oy, g->bitmap.width, g->bitmap.rows, GL_RED, GL_UNSIGNED_BYTE, g->bitmap.buffer);
		c[i].ax = (float)(g->advance.x >> 6);
		c[i].ay = (float)(g->advance.y >> 6);

		c[i].bw = g->bitmap.width;
		c[i].bh = g->bitmap.rows;

		c[i].bl = g->bitmap_left;
		c[i].bt = g->bitmap_top;

		c[i].tx = ox / (float)width;
		c[i].ty = oy / (float)height;

		rowh = rowh > g->bitmap.rows + 1 ? rowh : g->bitmap.rows + 1;
		ox += g->bitmap.width + 1;
	}

	printf("Generated a %d x %d (%d kb) texture atlas\n", width, height, width * height / 1024);
}

Atlas::~Atlas()
{
	glDeleteTextures(1, &tex);
}

