/* 
 * File:   TextRenderer.cpp
 * Author: dude719
 * 
 * Created on April 19, 2015, 6:57 AM
 */

#include "TextRenderer.h"
#include "ShaderManager.h"

#include <vector>
#include <glm\vec4.hpp>

TextRenderer*			TextRenderer::s_instance = NULL;
const int				TextRenderer::DEFAULT_COLOR = 0xFFFFFFFF;
const float				TextRenderer::DEFAULT_SCALE = 2.0f;
const int				TextRenderer::MAX_ATLAS_WIDTH = 1024;

TextRenderer::TextRenderer()
{
	// create default font
	Font theDefaultFont;
	Atlas* pDefaultAtlas = new Atlas(theDefaultFont, Font::DEFAULT_SIZE);
	m_fontAtlasMap[pDefaultAtlas->name] = pDefaultAtlas;
	m_atlas = pDefaultAtlas;
	bInit = false;
	Init();
}

TextRenderer::~TextRenderer()
{
	Cleanup();
}

void TextRenderer::Init()
{
	if (bInit)
		return;
	
	/* Initialize text shader */
	m_fontShader = ShaderManager::GetInstance()->GetShader("text_shader");
	m_fontShader->Use();

	// Get shader uniforms
	attribute_coord = m_fontShader->GetAttribLocation("coord");
	uniform_tex = m_fontShader->GetUniformLocation("tex");
	uniform_color = m_fontShader->GetUniformLocation("color");

	// Create the vertex buffer and vertex attribute objects
	glGenBuffers(1, &vbo);
	glGenVertexArrays(1, &vao);
	
	bInit = true;
}

void TextRenderer::BeginText()
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDisable(GL_DEPTH_TEST); // For transparency
	m_fontShader->Use();
}

void TextRenderer::RenderText(Atlas* a, float screenx, float screeny, float scale, GLfloat color[], const char *szFormatText, ...)
{
	if (szFormatText == NULL)
		return;

	//GLfloat clr[4] = { (float)(((color >> 24) & 0xFF) / 255), (float)(((color >> 16) & 0xFF) / 255), (float)(((color >> 8 ) & 0xFF) / 255), (float)(((color) & 0xFF) / 255) };
	glUniform4fv(uniform_color, 1, color);

	char szText[1024];
	va_list aParam;
	va_start(aParam, szFormatText);
	vsprintf(szText, szFormatText, aParam);
	va_end(aParam);

	float sx = scale / ((float)WINDOW_WIDTH * 0.5f);
	float sy = scale / ((float)WINDOW_HEIGHT * 0.5f);

	float x =  (((float)screenx / ((float)WINDOW_WIDTH  * 0.5f))) - 1;
	float y = -(((float)(screeny + a->maxcharheight - (a->maxcharheight / 6)) / ((float)WINDOW_HEIGHT * 0.5f))) + 1;

	std::vector<glm::vec4> coords;
	
	/* Loop through all characters */
	for (unsigned char *p = (unsigned char*)szText; *p; p++) 
	{
		/* Calculate the vertex and texture coordinates */
		float x2 =  x + a->c[*p].bl * sx;
		float y2 = -y - a->c[*p].bt * sy;
		float w = a->c[*p].bw * sx;
		float h = a->c[*p].bh * sy;
		
		x += a->c[*p].ax * sx;
		y += a->c[*p].ay * sy;
		
		/* Skip glyphs that have no pixels */
		if (!w || !h)
			continue;

		coords.push_back(glm::vec4(x2, -y2, a->c[*p].tx, a->c[*p].ty));
		coords.push_back(glm::vec4(x2 + w, -y2, a->c[*p].tx + a->c[*p].bw / a->width, a->c[*p].ty));
		coords.push_back(glm::vec4(x2, -y2 - h, a->c[*p].tx, a->c[*p].ty + a->c[*p].bh / a->height));
		coords.push_back(glm::vec4(x2 + w, -y2, a->c[*p].tx + a->c[*p].bw / a->width, a->c[*p].ty));
		coords.push_back(glm::vec4(x2, -y2 - h, a->c[*p].tx, a->c[*p].ty + a->c[*p].bh / a->height));
		coords.push_back(glm::vec4(x2 + w, -y2 - h, a->c[*p].tx + a->c[*p].bw / a->width, a->c[*p].ty + a->c[*p].bh / a->height));
	}

	/* Use the texture containing the atlas */
	glActiveTexture(GL_TEXTURE0);
	glUniform1i(uniform_tex, 0);
	glBindTexture(GL_TEXTURE_2D, a->GetTexture());
	glBindSampler(0, a->GetSampler());

	/* Set up the VBO for our vertex data */
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, coords.size() * sizeof(glm::vec4), coords.data(), GL_DYNAMIC_DRAW);

	/* Bind VAO */
	glBindVertexArray(vao);
	
	/* Coords (Position) */
	glEnableVertexAttribArray(attribute_coord);
	glVertexAttribPointer(attribute_coord, 4, GL_FLOAT, GL_FALSE, sizeof(glm::vec4), (void*)0);

	/* Draw all the character on the screen in one go */
	glDrawArrays(GL_TRIANGLE_STRIP, 0, coords.size());

	glDisableVertexAttribArray(attribute_coord);
}

void TextRenderer::RenderText(float screenx, float screeny, float scale, GLfloat color[], const char *szFormatText, ...)
{
	if (szFormatText == NULL)
		return;

	//GLfloat clr[4] = { (float)(((color >> 24) & 0xFF) / 255), (float)(((color >> 16) & 0xFF) / 255), (float)(((color >> 8 ) & 0xFF) / 255), (float)(((color) & 0xFF) / 255) };
	glUniform4fv(uniform_color, 1, color);

	char szText[1024];
	va_list aParam;
	va_start(aParam, szFormatText);
	vsprintf(szText, szFormatText, aParam);
	va_end(aParam);

	float sx = scale / ((float)WINDOW_WIDTH * 0.5f);
	float sy = scale / ((float)WINDOW_HEIGHT * 0.5f);

	float x =  (((float)screenx / ((float)WINDOW_WIDTH  * 0.5f))) - 1;
	float y = -(((float)(screeny + m_atlas->maxcharheight - (m_atlas->maxcharheight / 6)) / ((float)WINDOW_HEIGHT * 0.5f))) + 1;

	std::vector<glm::vec4> coords;
	
	//FT_UInt charIndex;
	//FT_UInt previousIndex;
	//FT_UInt _x_ = 0;
	
	/* Loop through all characters */
	for (unsigned char *p = (unsigned char*)szText; *p; p++) 
	{
		/* Calculate the vertex and texture coordinates */
		float x2 =  x + m_atlas->c[*p].bl * sx;
		float y2 = -y - m_atlas->c[*p].bt * sy;
		float w = m_atlas->c[*p].bw * sx;
		float h = m_atlas->c[*p].bh * sy;
		
		x += m_atlas->c[*p].ax * sx;
		y += m_atlas->c[*p].ay * sy;
		
		/* Skip glyphs that have no pixels */
		if (!w || !h)
			continue;

		coords.push_back(glm::vec4(x2, -y2, m_atlas->c[*p].tx, m_atlas->c[*p].ty));
		coords.push_back(glm::vec4(x2 + w, -y2, m_atlas->c[*p].tx + m_atlas->c[*p].bw / m_atlas->width, m_atlas->c[*p].ty));
		coords.push_back(glm::vec4(x2, -y2 - h, m_atlas->c[*p].tx, m_atlas->c[*p].ty + m_atlas->c[*p].bh / m_atlas->height));
		coords.push_back(glm::vec4(x2 + w, -y2, m_atlas->c[*p].tx + m_atlas->c[*p].bw / m_atlas->width, m_atlas->c[*p].ty));
		coords.push_back(glm::vec4(x2, -y2 - h, m_atlas->c[*p].tx, m_atlas->c[*p].ty + m_atlas->c[*p].bh / m_atlas->height));
		coords.push_back(glm::vec4(x2 + w, -y2 - h, m_atlas->c[*p].tx + m_atlas->c[*p].bw / m_atlas->width, m_atlas->c[*p].ty + m_atlas->c[*p].bh / m_atlas->height));
	}

	/* Use the texture containing the atlas */
	glActiveTexture(GL_TEXTURE0);
	glUniform1i(uniform_tex, 0);
	glBindTexture(GL_TEXTURE_2D, m_atlas->GetTexture());
	glBindSampler(0, m_atlas->GetSampler());

	/* Set up the VBO for our vertex data */
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, coords.size() * sizeof(glm::vec4), coords.data(), GL_DYNAMIC_DRAW);

	/* Generate VAO */
	glBindVertexArray(vao);
	
	/* Coords (Position) */
	glEnableVertexAttribArray(attribute_coord);
	glVertexAttribPointer(attribute_coord, 4, GL_FLOAT, GL_FALSE, sizeof(glm::vec4), (void*)0);

	/* Draw all the character on the screen in one go */
	glDrawArrays(GL_TRIANGLE_STRIP, 0, coords.size());

	glDisableVertexAttribArray(attribute_coord);
}

void TextRenderer::EndText()
{
	glDisableVertexAttribArray(attribute_coord);
	glEnable(GL_DEPTH_TEST); // For transparency
}

void TextRenderer::Cleanup() 
{
	glDeleteBuffers(1, &vbo);
	glDeleteVertexArrays(1, &vao);
	glDeleteShader(m_fontShader->GetVert());
	glDeleteShader(m_fontShader->GetFrag());
	glDeleteProgram(m_fontShader->GetProgram());
};

void TextRenderer::AddFont(const char* fontName, int fontSize)
{
	std::map<const char*, Atlas*>::iterator it = m_fontAtlasMap.find(fontName);
	if(it != m_fontAtlasMap.end())
		return;
	Font newFont(fontName);
	Atlas* pNewAtlas = new Atlas(newFont, fontSize);
	m_fontAtlasMap[pNewAtlas->name] = pNewAtlas;
}

void TextRenderer::SetFont(const char* fontName, int fontSize)
{
	std::map<const char*, Atlas*>::iterator it = m_fontAtlasMap.find(fontName);
	if(it != m_fontAtlasMap.end())
	{
		m_atlas = it->second;
	}
	else
	{
		Font newFont(fontName);
		Atlas* pNewAtlas = new Atlas(newFont, fontSize);
		m_fontAtlasMap[pNewAtlas->name] = pNewAtlas;
		m_atlas = pNewAtlas;
	}
}

void TextRenderer::SetFont(Atlas* fontAtlas)
{
	m_atlas = fontAtlas;
}

TextRenderer* TextRenderer::GetInstance()
{
	if (s_instance == NULL)
		s_instance = new TextRenderer();
	return s_instance;
}
