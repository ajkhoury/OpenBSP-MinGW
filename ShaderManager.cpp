/* 
 * File:   ShaderManager.cpp
 * Author: dude719
 * 
 * Created on April 19, 2015, 6:58 AM
 */

#include "Shader.h"
#include "ShaderManager.h"
#include "GLEW/glew.h"
#include <stdlib.h>

// Maximum line length of a shader's source file
const int MAX_LINE_LENGTH = 512;

ShaderManager* ShaderManager::s_instance = NULL;
const char* ShaderManager::DEFAULT_SHADER = "DefaultShader";

ShaderManager::ShaderManager()
{
	// create default shader for unsuccessful GetShader()
	// the default Shader has program value which is 0 (default)
	Shader* theDefaultShader = new Shader();
	m_shaderMap[DEFAULT_SHADER] = theDefaultShader;
}

ShaderManager::~ShaderManager()
{
	// clean up all shaders
	std::map<std::string, Shader*>::iterator iter;
	for (iter = m_shaderMap.begin(); iter != m_shaderMap.end(); ++iter)
	{
		Dispose(iter->second);
		delete iter->second;
	}
	m_shaderMap.clear();
}

ShaderManager* ShaderManager::GetInstance()
{
	if (s_instance == NULL)
		s_instance = new ShaderManager();
	return (s_instance);
}

Shader* ShaderManager::GetShader(const char* inName, bool useGeomShader)
{
	std::string inVertFileName(inName);
	std::string inFragFileName(inName);
	std::string inGeomFileName("");

	inVertFileName += ".vert";
	inFragFileName += ".frag";

	if (useGeomShader)
	{
		inGeomFileName = inName;
		inGeomFileName += ".geom";
	}

	std::map<std::string, Shader*>::iterator it = m_shaderMap.find(inName);
	if(it != m_shaderMap.end())
	{
		return (it->second);
	}
	else
	{
		Shader* theResult = NULL;
		if (useGeomShader)
			theResult = Load(inVertFileName.c_str(), inFragFileName.c_str(), inGeomFileName.c_str());
		else
			theResult = Load(inVertFileName.c_str(), inFragFileName.c_str(), NULL);
		if(theResult != NULL)
		{
			// successful loaded and linked shader program, added to map and return the result
			m_shaderMap[inName] = theResult;
			return theResult;
		}
	}

	// if load/link unsuccessfully, return default shader which program = 0
	return m_shaderMap[DEFAULT_SHADER];
}

Shader* ShaderManager::Load(const char* inVertexFilename, const char* inFragmentFilename, const char* inGeometryFilename)
{
	unsigned int vertShader = 0, fragShader = 0, geomShader = 0, programShader = 0;
	bool loadStatus;

	// load and compile vertex. geometric and fragment sources
	loadStatus = LoadShader(GL_VERTEX_SHADER, inVertexFilename, vertShader);
	loadStatus &= LoadShader(GL_FRAGMENT_SHADER, inFragmentFilename, fragShader);
	// if geometry file is provided, load it
	if (inGeometryFilename)
		loadStatus &= LoadShader(GL_GEOMETRY_SHADER_EXT, inGeometryFilename, geomShader);

	if (loadStatus) 
	{
		// create a program
		programShader = glCreateProgram();

		// attach the vertex and fragment shader codes, and the geometric if available
		glAttachShader(programShader, vertShader);
		glAttachShader(programShader, fragShader);

		if (geomShader != 0)
			glAttachShader(programShader, geomShader);

		// link
		printf("Linking... \n");
		glLinkProgram(programShader);

		// check link status
		GLint status = GL_FALSE;
		glGetProgramiv(programShader, GL_LINK_STATUS, &status);
		if (status != GL_TRUE) 
		{
			// The link has failed, check log info
			int logLength = 1;
			glGetProgramiv(programShader, GL_INFO_LOG_LENGTH, &logLength);
		
			char* infoLog = (char*)malloc(logLength+1);
			glGetProgramInfoLog(programShader, logLength, &logLength, infoLog);
			printf("Failed to link the shader: %s\n", infoLog);
			delete[] infoLog;
		
			return NULL;
		}

		// check if the shader will run in the current OpenGL state
		glValidateProgram(programShader);
		glGetProgramiv(programShader, GL_VALIDATE_STATUS, &status);
		if (status != GL_TRUE) 
		{
			printf("Shader program will not run in this OpenGL environment!\n");
			return NULL;
		}

		// the program has been loaded/linked successfully
		Shader* res = new Shader();
		res->SetVertShader(vertShader);
		res->SetFragShader(fragShader);
		res->SetGeomShader(geomShader);
		res->SetProgram(programShader);

		return res;
	}

	return NULL;
}

void ShaderManager::Dispose(Shader* inShader)
{
	if (inShader->GetProgram() != 0) 
	{
		glDeleteProgram(inShader->GetProgram());
	}
	if (inShader->GetVert() != 0) 
	{
		glDeleteShader(inShader->GetVert());
	}
	if (inShader->GetGeom() != 0) 
	{
		glDeleteShader(inShader->GetGeom());
	}
	if (inShader->GetFrag() != 0) 
	{
		glDeleteShader(inShader->GetFrag());
	}
}

bool ShaderManager::LoadShader(unsigned int inShaderType, const std::string& inFileName, GLuint &inOutShader)
{
	// Read the shaders.
	std::ifstream ifs(inFileName.c_str());
	std::stringstream vss;
	vss << ifs.rdbuf();
	std::string vstring = vss.str();
	ifs.close();

	printf("Compiling: %s\n", inFileName.c_str());
	// Compile.
	inOutShader = glCreateShader(inShaderType);
	const char* pShader = vstring.c_str();
	glShaderSource(inOutShader, 1, &pShader, 0);
	glCompileShader(inOutShader);

	// Check for errors.
	GLint status;
	glGetShaderiv(inOutShader, GL_COMPILE_STATUS, &status);
	if (status != GL_TRUE) 
	{
		// fail to compile, check the log
		int logLength = 1;
		glGetShaderiv(inOutShader, GL_INFO_LOG_LENGTH, &logLength);

		char* infoLog = (char*)malloc(logLength + 1);
		glGetShaderInfoLog(inOutShader, logLength, &logLength, infoLog);
		printf("Failed to compile shader %s\n%s", inFileName.c_str(), infoLog);
		delete[] infoLog;

		return false;
	}
	
	return true;
}

