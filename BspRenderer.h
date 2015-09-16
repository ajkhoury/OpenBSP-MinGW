/* 
 * File:   BspRenderer.h
 * Author: dude719
 *
 * Created on May 1, 2015, 1:08 AM
 */

#ifndef BSPRENDERER_H
#define	BSPRENDERER_H

#include "Bsp.h"
#include "Shader.h"

class BspRenderer
{
public:
    BspRenderer(Bsp* bsp, Camera* cam);
    BspRenderer(const BspRenderer& orig);
    virtual ~BspRenderer();
    
    void Init();
    
    void Begin();
    void Render();
    void End();
    
    void ToggleWireframe();
    void ToggleBrushEntityNames();
    
private:
    Shader*  m_bspShader;
    Bsp* m_bsp;
    Camera*  m_camera;
    
    GLuint m_vao, m_vbo, m_normalVBO, m_ebo;
    
    GLint m_MVPUniform;
    GLint m_modelUniform;
    GLint m_viewUniform;
	GLint m_projectionUniform;
    
	GLint m_lightUniform;
    GLint m_lightFragUniform;
    
	GLint m_useUserColorUniform;
	GLint m_userColorUniform;
    
    GLint m_positionAttribute;
    GLint m_normalPositionAttribute;
    
    bool m_bWireframe;
    bool m_bBrushEntityNames;
    
    bool bInit;
};

#endif	/* BSPRENDERER_H */

