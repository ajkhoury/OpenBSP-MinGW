/* 
 * File:   TextRenderer.h
 * Author: dude719
 *
 * Created on April 19, 2015, 6:57 AM
 */

#ifndef TEXTRENDERER_H
#define	TEXTRENDERER_H

#include "GLEW/glew.h"
#include "GLFW/glfw3.h"

#include "Shader.h"
#include "Font.h"
#include "Atlas.h"

#include <map>
#include <string>

// Forward declaration
class Font;

class TextRenderer
{
public:
	TextRenderer();
    virtual ~TextRenderer();
    
	void Init();
    void Cleanup();
    
	void AddFont(const char* fontName, int fontSize);
	void SetFont(const char* fontName, int fontSize);
    void SetFont(Atlas* fontAtlas);
    
	void BeginText();
	void RenderText(float screenx, float screeny, float scale, GLfloat color[], const char *szFormatText, ...);
    void RenderText(Atlas* a, float screenx, float screeny, float scale, GLfloat color[], const char *szFormatText, ...);
	void EndText();

	/* Get the unique instance of this class */
	static TextRenderer*	GetInstance();

private:
	// current font used to render text
	Atlas* m_atlas;
	
	// Map of font name and font obj pointer
	std::map<const char*, Atlas*> m_fontAtlasMap;

	GLint	attribute_coord;
	GLint	uniform_tex;
	GLint	uniform_color;

	GLuint	vbo;
	GLuint	vao;

	Shader* m_fontShader;

	bool	bInit;

	// The unique instance of this class
	static TextRenderer*	s_instance;
    
	static const int		DEFAULT_COLOR;
	static const float		DEFAULT_SCALE;
	static const int		MAX_ATLAS_WIDTH;
};


#endif	/* TEXTRENDERER_H */

