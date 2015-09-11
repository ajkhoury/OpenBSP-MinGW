/* 
 * File:   BspRenderer.cpp
 * Author: dude719
 * 
 * Created on May 1, 2015, 1:08 AM
 */

#include "BspRenderer.h"

#include "ShaderManager.h"
#include "TextRenderer.h"

#include "Color.h"

BspRenderer::BspRenderer(Bsp* bsp, Camera* cam) 
	: m_bsp(bsp),
	m_camera(cam),
	m_bWireframe(false),
	m_bBrushEntityNames(false),
	bInit(false)
{
	Init();
}

BspRenderer::BspRenderer(const BspRenderer& orig)
{
}

BspRenderer::~BspRenderer()
{
	glDeleteBuffers(1, &m_vbo);
	glDeleteVertexArrays(1, &m_vao);
	glDeleteShader(m_bspShader->GetVert());
	glDeleteShader(m_bspShader->GetFrag());
	glDeleteProgram(m_bspShader->GetProgram());
}

void BspRenderer::Init()
{
	if (bInit)
		return;
	
	/* Initialize basic shader */
	m_bspShader = ShaderManager::GetInstance()->GetShader("basic_shader");
	if (!m_bspShader)
	{
		printf("BspRenderer[Init]: could not create bsp shader!\n");
		return;
	}
	
	m_bspShader->Use();
	
	m_viewUniform = m_bspShader->GetUniformLocation("view");
	m_projectionUniform = m_bspShader->GetUniformLocation("projection");
	m_modelUniform = m_bspShader->GetUniformLocation("model");
	m_useUserColorUniform = m_bspShader->GetUniformLocation("useUserColor");
	m_userColorUniform = m_bspShader->GetUniformLocation("userColor");
	
	glGenVertexArrays(1, &m_vao);
	glBindVertexArray(m_vao);

	glGenBuffers(1, &m_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBufferData(GL_ARRAY_BUFFER, (sizeof(float) * 3) * m_bsp->vertices.size(), m_bsp->vertices.data(), GL_STATIC_DRAW);

	glGenBuffers(1, &m_normalVBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_normalVBO);
	glBufferData(GL_ARRAY_BUFFER, (sizeof(float) * 3) * m_bsp->mapNormals.size(), m_bsp->mapNormals.data(), GL_STATIC_DRAW);
		
	glGenBuffers(1, &m_ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * m_bsp->indices.size(), m_bsp->indices.data(), GL_STATIC_DRAW);
	
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	m_positionAttribute = m_bspShader->GetAttribLocation("position");
	glEnableVertexAttribArray(m_positionAttribute);
	glVertexAttribPointer(m_positionAttribute, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, m_normalVBO);
	m_normalPositionAttribute = m_bspShader->GetAttribLocation("nColor");
	glEnableVertexAttribArray(m_normalPositionAttribute);
	glVertexAttribPointer(m_normalPositionAttribute, 3, GL_FLOAT, GL_FALSE, 0, 0);
		
	bInit = true;
}

void BspRenderer::Begin()
{
	glBindVertexArray(m_vao);
	
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glEnableVertexAttribArray(m_positionAttribute);
	glVertexAttribPointer(m_positionAttribute, 3, GL_FLOAT, GL_FALSE, 0, 0);
    
    glBindBuffer(GL_ARRAY_BUFFER, m_normalVBO);
    glEnableVertexAttribArray(m_normalPositionAttribute);
	glVertexAttribPointer(m_normalPositionAttribute, 3, GL_FLOAT, GL_FALSE, 0, 0);

	m_bspShader->Use();
}

void BspRenderer::Render()
{
	glUniformMatrix4fv(m_viewUniform, 1, GL_FALSE, glm::value_ptr(m_camera->View()));
	glUniformMatrix4fv(m_projectionUniform, 1, GL_FALSE, glm::value_ptr(m_camera->Projection()));
	glUniformMatrix4fv(m_modelUniform, 1, GL_FALSE, glm::value_ptr(glm::mat4(1.0f)));
		
	glUniform1i(m_useUserColorUniform, GL_TRUE);
	glUniform4f(m_userColorUniform, 1.0f, 1.0f, 1.0f, 1.0f);
		
	if (!m_bWireframe)
		glDrawElements(GL_TRIANGLES, m_bsp->worldSize, GL_UNSIGNED_INT, 0);
	
	glUniform1i(m_useUserColorUniform, GL_TRUE);
		
	glEnable(GL_LINE_SMOOTH);
	glDepthFunc(GL_LEQUAL);

	glUniform4f(m_userColorUniform, 0.0f, 0.0f, 0.0f, 1.0f);
	glDrawElements(GL_LINE_LOOP, m_bsp->worldSize, GL_UNSIGNED_INT, 0);
	
	m_bsp->RenderBrushEntities(m_modelUniform, m_userColorUniform);
	//m_bsp->RenderProps(m_modelUniform, m_userColorUniform); 
	
	
	glDepthFunc(GL_LESS);
	glDisable(GL_LINE_SMOOTH);
    
	if (m_bBrushEntityNames)
		m_bsp->RenderBrushEntityNames(m_camera, Color::red);
}

void BspRenderer::End()
{
	glDisableVertexAttribArray(m_positionAttribute);
    glDisableVertexAttribArray(m_normalPositionAttribute);
}

void BspRenderer::ToggleWireframe()
{
	m_bWireframe = !m_bWireframe;
}

void BspRenderer::ToggleBrushEntityNames()
{
	m_bBrushEntityNames = !m_bBrushEntityNames;
}
