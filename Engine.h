/* 
 * File:   Engine.h
 * Author: dude719
 *
 * Created on April 19, 2015, 6:10 AM
 */

#ifndef ENGINE_H
#define	ENGINE_H

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>

// My Classes
#include "Bsp.h"
#include "Camera.h"
#include "SharedDefs.h"

// OpenGL
#include "GLFW/glfw3.h"
#include "GLEW/glew.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

class Engine
{
public:
	Engine(int width, int height, const char* title);

	void		OpenWindow(const char* title);
	void		Update();

	GLFWwindow* Window();
	Camera*		Cam();

	void		OffsetScroll(double deltaY);

    int         GetFPS();
    
	bool		IsButtonPressed(int code);

private:
	Camera*		camera;
	GLFWwindow* window;
    
	glm::vec2	mouseDelta;
	double		mouseX, mouseY, lastX, lastY;
	float		mouseSpeed, cameraSpeed, maxCameraSpeed, zoomSpeed;
	float		scrollY;
    
    float       lastTime, deltaTime, currentTime;
    int         frameCount, fps;
};

extern Engine* g_pEngine;

#endif	/* ENGINE_H */

