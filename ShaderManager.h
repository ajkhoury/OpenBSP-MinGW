/* 
 * File:   ShaderManager.h
 * Author: dude719
 *
 * Created on April 19, 2015, 6:58 AM
 */

#ifndef SHADERMANAGER_H
#define	SHADERMANAGER_H

#include <string>
#include <map>
#include <fstream>
#include <sstream>

// Forward declaration
class Shader;

class ShaderManager
{
public:
	~ShaderManager();

	// Get the unique instance of this class
	static ShaderManager*	GetInstance();

	/**
	 * Get shader object pointer
	 * @return if successful load/link, the loaded/linked shader object point is return, otherwise return the default shader
	 */
	Shader* GetShader(const char* inName, bool geomShader = false);

private:
	// Default constructor (private)
	ShaderManager();

	/**
	 * Load, compile and create a vertex/fragment/geometric shader program and link the program
	 * @return CShader object pointer if all shaders are loaded successfully, NULL otherwise
	 */
	Shader* Load(const char* inVertexFilename, const char* inFragmentFilename, const char* inGeometryFilename);

	/** Releases all resources */
	void Dispose(Shader* inShader);

	/** 
	 * Load and compile a shader
	 * @param inShaderType type of shader: GL_VERTEX_SHADER, GL_GEOMETRY_SHADER_EXT or GL_FRAGMENT_SHADER
	 * @param inFileName shader file name
	 * @param inOutShader the pointer to shader
	 * @return true if successfully loaded, false otherwise
	 */
	bool LoadShader(unsigned int inShaderType,  const std::string& inFileName, unsigned int & inOutShader);

private:
	// Map of shader name and shader obj pointer
	std::map<std::string, Shader*> m_shaderMap;

	// Default shader string
	static const char* DEFAULT_SHADER;
	// The unique instance of this class
	static ShaderManager*	s_instance;
};

#endif	/* SHADERMANAGER_H */

