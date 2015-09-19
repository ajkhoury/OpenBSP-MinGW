/* 
 * File:   Atlas.h
 * Author: dude719
 *
 * Created on April 19, 2015, 6:14 AM
 */

#ifndef ATLAS_H
#define ATLAS_H

#include <cstdlib>

#include "GLEW/glew.h"
#include "GLFW/glfw3.h"

#include "glm/glm.hpp"

#include "shareddefs.h"
#include "Font.h"

class Atlas 
{
public:	
	Atlas(Font font, int fontSize);
	~Atlas();
    
    char name[128];
    
    FT_Face face;
    bool useKerning;
    
	int width;			// width of texture in pixels
	int height;			// height of texture in pixels

	int maxcharheight;

	struct 
	{
		float ax;	// advance.x
		float ay;	// advance.y

		float bw;	// bitmap.width;
		float bh;	// bitmap.height;

		float bl;	// bitmap_left;
		float bt;	// bitmap_top;

		double tx;	// x offset of glyph in texture coordinates
		double ty;	// y offset of glyph in texture coordinates
	} c[128];		// character information

	int GetTexture() { return tex; }
	int GetSampler() { return sampler; }

private:
	unsigned int tex;		// texture object
	unsigned int sampler;   // texture sampler

	static const int MAXWIDTH;
};

#endif	/* ATLAS_H */

