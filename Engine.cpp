/* 
 * File:   Engine.cpp
 * Author: dude719
 * 
 * Created on April 19, 2015, 6:10 AM
 */

#include "Engine.h"

Engine* g_pEngine;

Engine::Engine(int width, int height, const char* title) 
	: mouseX(0.0f), 
	mouseY(0.0f), 
	mouseSpeed(0.1f), 
	cameraSpeed(500.0f), 
	zoomSpeed(-1.0f), 
	scrollY(0.0f),
	lastTime(glfwGetTime()),
	deltaTime(0.0f),
	currentTime(0.0f),
	frameCount(0),
	fps(0)
{
	OpenWindow(title);
	camera = new Camera(width, height);
}

void Engine::OpenWindow(const char* title)
{   
	glfwInit();
        
        glfwWindowHint(GLFW_SAMPLES, 4);
        glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_API);
        
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_ANY_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
        

	window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, title, NULL, NULL);
	glfwMakeContextCurrent(window);
        
        glewExperimental = GL_TRUE; // experimental
	GLenum glew_status = glewInit();
	if (GLEW_OK != glew_status)
		printf("Error: %s\n", glewGetErrorString(glew_status));

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	
	glEnable(GL_PRIMITIVE_RESTART);
	glPrimitiveRestartIndex(-1);
		
	glEnable(GL_CULL_FACE);
	glCullFace(GL_FRONT);

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

void Engine::Update()
{
	deltaTime = glfwGetTime() - currentTime;
	currentTime = glfwGetTime();
        frameCount++;
	if (currentTime - lastTime >= 1.0f)
	{
            fps = frameCount;
            frameCount = 0;
            lastTime += 1.0f;
        }
	
	float speed = (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) ? (cameraSpeed * 2.0f) : cameraSpeed;
	
	if(glfwGetKey(window, GLFW_KEY_S))
		camera->OffsetPosition(deltaTime * speed * -camera->Forward());
	else if(glfwGetKey(window, GLFW_KEY_W))
		camera->OffsetPosition(deltaTime * speed * camera->Forward());

	if(glfwGetKey(window, GLFW_KEY_A))
		camera->OffsetPosition(deltaTime * speed * -camera->Right());
	else if(glfwGetKey(window, GLFW_KEY_D))
		camera->OffsetPosition(deltaTime * speed * camera->Right());

	if(glfwGetKey(window, GLFW_KEY_LEFT_CONTROL))
		camera->OffsetPosition(deltaTime * speed * -glm::vec3(0,0,1));
	else if(glfwGetKey(window, GLFW_KEY_SPACE))
		camera->OffsetPosition(deltaTime * speed * glm::vec3(0,0,1));

	lastX = mouseX; lastY = mouseY;
	glfwGetCursorPos(window, &mouseX, &mouseY);
	camera->OffsetOrientation(mouseSpeed * (mouseY - lastY), mouseSpeed * (mouseX - lastX));

	float fieldOfView = camera->GetFoV() + zoomSpeed * (float)scrollY;
	if(fieldOfView < 5.0f)
		fieldOfView = 5.0f;
	if(fieldOfView > 100.0f)
		fieldOfView = 100.0f;
	camera->SetFoV(fieldOfView);
	
	scrollY = 0.0f;
}

void Engine::OffsetScroll(double deltaY) 
{
	scrollY += deltaY;
}

Camera* Engine::Cam()
{
	return camera;
}

GLFWwindow* Engine::Window()
{
	return window;
}

int Engine::GetFPS()
{
	return fps;
}

bool Engine::IsButtonPressed(int code)
{
	static float buttonPressedTime = 0.0f;
	float time = glfwGetTime();
	if ((glfwGetKey(window, code) == GLFW_PRESS) && (time - buttonPressedTime > 0.25))
	{
		buttonPressedTime = time;
		return true;
	}
	return false;
}