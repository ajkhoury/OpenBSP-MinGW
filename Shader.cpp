/* 
 * File:   Shader.cpp
 * Author: dude719
 * 
 * Created on April 19, 2015, 6:44 AM
 */

#include "Shader.h"
#include <GLEW/glew.h>

Shader::Shader()
	: m_VertexShader(0),
	m_GeometricShader(0),
	m_FragmentShader(0),
	m_Program(0)
{
}

Shader::~Shader()
{
}

int Shader::GetUniformLocation(const char* inVarName)
{
	return GetVariableLocation(inVarName, true);
}

int Shader::GetAttribLocation(const char* inVarName)
{
	return GetVariableLocation(inVarName, false);
}

int Shader::GetVariableLocation(const char *inVarName, bool inIsUniform)
{
	int theResult = -1;
	if (inIsUniform) // uniform variables
		theResult = glGetUniformLocation(m_Program, inVarName);
	else // attribute variables
		theResult = glGetAttribLocation(m_Program, inVarName);
	return theResult;
}

void Shader::Use()
{
	glUseProgram(m_Program);
}
