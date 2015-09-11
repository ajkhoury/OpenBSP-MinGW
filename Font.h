/* 
 * File:   Font.h
 * Author: dude719
 *
 * Created on April 19, 2015, 6:52 AM
 */

#ifndef FONT_H
#define	FONT_H

#include "GLEW/glew.h"
#include "GLFW/glfw3.h"

#include "shareddefs.h"

class Font
{
public:
	Font();
	Font(const char* fontName);

    const char* GetName();
    
	bool	Load();
	bool	SetFont(const char* fontName);

	FT_Face GetFace()	{ return face; }
   
private:
	char	szName[64];

	FT_Face	face;

public:
	static const char* DEFAULT_FONT;
	static int	DEFAULT_SIZE;
private:
	static const char* EXTENSION;
};

#endif	/* FONT_H */

