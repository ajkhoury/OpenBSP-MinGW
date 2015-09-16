/* 
 * File:   main.cpp
 * Author: dude719
 *
 * Created on April 19, 2015, 1:11 AM
 */

#include "Main.h"
#include "BspRenderer.h"

Shader* g_pBspShader = NULL;

FT_Library ft_lib;
FT_Face face;

int main()
{
	/* Create a debug console */
	Utils::CreateConsole("debug");
	
	char mapname[128];
#ifdef _DEBUG
	strcpy(mapname, "de_nuke.bsp");
#else
	printf("Please enter the name of the BSP file you want to view: ");
	//scanf("%127s", mapname);
        strcpy(mapname, "de_dust2.bsp");
#endif

	/* Initialize Engine & create new window */
	g_pEngine = new Engine(WINDOW_WIDTH, WINDOW_HEIGHT, "OpenBSP");
	glfwSetScrollCallback(g_pEngine->Window(), OnScroll);

	/* Create bsp object, load map details & generate vertex & index buffers */
	Bsp bsp(mapname);
	bsp.LoadMapDetails(g_pEngine->Cam());
	
	/* Create and initialize our BSP Renderer */
	BspRenderer* bspRenderer = new BspRenderer(&bsp, g_pEngine->Cam());
	
	/* Initialize our freetype library for font rendering */
	if(FT_Init_FreeType(&ft_lib) != 0) 
	{
		printf("Couldn't initialize FreeType library\n");
		return 1;
	}

	/* Create texture atlasses for several font sizes */
	Font fixedsys("FixedSys");
	Atlas* f48 = new Atlas(fixedsys, 48);
	Atlas* f18 = new Atlas(fixedsys, 18);
	Atlas* f16 = new Atlas(fixedsys, 16);
	Atlas* f14 = new Atlas(fixedsys, 14);
	Atlas* f12 = new Atlas(fixedsys, 12);
	
	/* Set the default text font to use */
	TextRenderer::GetInstance()->SetFont("Arial", 12);
    
	glm::vec3 pos;

	while (!glfwWindowShouldClose(g_pEngine->Window()))
	{
		glfwPollEvents();

		/* Engine Update */
		g_pEngine->Update();
		
		/* Check key presses */
		if (g_pEngine->IsButtonPressed(GLFW_KEY_F1))
			bspRenderer->ToggleWireframe();
		if (g_pEngine->IsButtonPressed(GLFW_KEY_F2))
			bspRenderer->ToggleBrushEntityNames();
		if (g_pEngine->IsButtonPressed(GLFW_KEY_ESCAPE))
			glfwSetWindowShouldClose(g_pEngine->Window(), GL_TRUE);

		/* Clear the back buffer */
		glClearColor(0.5f , 0.5f , 0.5f , 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear the buffer.

		
		/* Render BSP data */
		bspRenderer->Begin();
		bspRenderer->Render();
		bspRenderer->End();
		
		pos = g_pEngine->Cam()->Position();
		
		/* Render text */
		TextRenderer::GetInstance()->BeginText();
		
		TextRenderer::GetInstance()->RenderText(f18, 10, 10, 1.0f, Color::black, "position: %.1f, %.1f, %.1f", pos.x, pos.y, pos.z);
		TextRenderer::GetInstance()->RenderText(f18, 10, 40, 1.0f, Color::black, "yaw: %.1f  pitch: %.1f", g_pEngine->Cam()->Yaw(), g_pEngine->Cam()->Pitch());
		TextRenderer::GetInstance()->RenderText(f18, 10, 70, 1.0f, Color::black, "fov: %.1f", g_pEngine->Cam()->GetFoV());
		TextRenderer::GetInstance()->RenderText(f16, WINDOW_WIDTH - 280, WINDOW_HEIGHT - 15, 1.0f, Color::yellow, "%s version %i revision %i", mapname, bsp.header.version, bsp.header.revision);
		TextRenderer::GetInstance()->RenderText(f48, WINDOW_WIDTH - 80, 0, 1.0f, Color::green, "%i", g_pEngine->GetFPS());
		
		TextRenderer::GetInstance()->EndText();
		

		glfwSwapBuffers(g_pEngine->Window());
	}

	glfwTerminate();

	return 0;
}

void OnScroll(GLFWwindow* window, double deltaX, double deltaY) 
{
	g_pEngine->OffsetScroll(deltaY);
}

