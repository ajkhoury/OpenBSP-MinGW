/* 
 * File:   Shader.h
 * Author: dude719
 *
 * Created on April 19, 2015, 6:44 AM
 */

#ifndef SHADER_H
#define	SHADER_H

class Shader
{
public: // Methods
  Shader();
  ~Shader();

  // Getter/setters shader properties
  unsigned int GetVert() { return m_VertexShader; }
  unsigned int GetGeom() { return m_GeometricShader; }
  unsigned int GetFrag() { return m_FragmentShader; }
  unsigned int GetProgram() { return m_Program; }

  void SetVertShader(unsigned int inValue) { m_VertexShader = inValue; }
  void SetGeomShader(unsigned int inValue) { m_GeometricShader = inValue; }
  void SetFragShader(unsigned int inValue) { m_FragmentShader = inValue; }
  void SetProgram(unsigned int inValue) { m_Program = inValue; }

  // Get index of an attribute variable of this shader
  int GetAttribLocation(const char* inVarName);

  // Get index of an uniform variable of this shader
  int GetUniformLocation(const char* inVarName);

  void Use();
  
private:
  /**
   * Get index of an variable of the shader
   * @param inVarName the variable name
   * @param inIsUniform whether the variable is an uniform
   */
  int GetVariableLocation(const char* inVarName, bool inIsUniform);


private: // Fields
  unsigned int m_VertexShader;
  unsigned int m_GeometricShader;
  unsigned int m_FragmentShader;
  unsigned int m_Program;

};

#endif	/* SHADER_H */

