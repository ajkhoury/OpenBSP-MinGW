/* 
 * File:   Font.cpp
 * Author: dude719
 * 
 * Created on April 19, 2015, 6:52 AM
 */

#include "Font.h"

const char* Font::DEFAULT_FONT = "Arial";
const char* Font::EXTENSION = ".ttf";
int			Font::DEFAULT_SIZE = 12;

Font::Font()
{
	strcpy(szName, DEFAULT_FONT);
	Load();
}

Font::Font(const char* fontName)
{
	strcpy(szName, fontName);
	Load();
}

const char* Font::GetName()
{
	return szName;
}

bool Font::SetFont(const char* fontName)
{
	if (face)
		FT_Done_Face(face);
	strcpy(szName, fontName);
	return Load();
}

bool Font::Load()
{
	char* szFile = (char*)malloc(strlen(szName) + strlen(EXTENSION) + 1);
	strcpy(szFile, szName);
	strcat(szFile, EXTENSION);

	printf("Font file: %s\n", szFile);

	if(FT_New_Face(ft_lib, szFile, 0, &face) != 0) 
	{
		printf("Unable to load %s\n", szFile);
		FT_Done_Face(face);
		delete[] szFile;

		return false;
	}
	FT_Set_Pixel_Sizes(face, 0, DEFAULT_SIZE);
	//FT_Set_Char_Size(face, 50, 50, WINDOW_WIDTH, WINDOW_HEIGHT);

	delete[] szFile;

	return true;
}

