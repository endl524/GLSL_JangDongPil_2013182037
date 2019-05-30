/*
Copyright 2018 Lee Taek Hee (Korea Polytech University)

This program is free software: you can redistribute it and/or modify
it under the terms of the What The Hell License. Do it plz.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY.
*/

#include "stdafx.h"
#include "Dependencies\glew.h"
#include "Dependencies\freeglut.h"

#include "Renderer.h"

Renderer *g_Renderer = NULL;

int g_WindowSizeX = 600;
int g_WindowSizeY = 600;

time_point<system_clock> g_Prev_Time;
duration<float> g_Elapsed_Time(0);

int g_Prev_Cursor_Position_X = 0;
int g_Prev_Cursor_Position_Y = 0;
bool g_is_Left_Clicked = false;
bool g_is_Right_Clicked = false;

void RenderScene(void)
{
	g_Prev_Time = system_clock::now();
	
	g_Renderer->Rendering(g_Elapsed_Time.count());
	glutSwapBuffers();

	g_Elapsed_Time = system_clock::now() - g_Prev_Time;
}

void Idle(void)
{
	RenderScene();
}

void MouseInput(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON)
	{
		if (state == GLUT_DOWN && !g_is_Left_Clicked && !g_is_Right_Clicked)
		{
			// 윈도우 사이즈 변경에 대한 예외처리는 안돼있으니 유의!
			g_Prev_Cursor_Position_X = x - g_WindowSizeX * 0.5f;
			g_Prev_Cursor_Position_Y = g_WindowSizeY * 0.5f - y;
			g_is_Left_Clicked = true;
		}
		else if (state == GLUT_UP && g_is_Left_Clicked)
		{
			g_Prev_Cursor_Position_X = 0.0f;
			g_Prev_Cursor_Position_Y = 0.0f;
			g_is_Left_Clicked = false;
		}
		
	}
	if (button == GLUT_RIGHT_BUTTON)
	{
		if (state == GLUT_DOWN && !g_is_Left_Clicked && !g_is_Right_Clicked)
		{
			// 윈도우 사이즈 변경에 대한 예외처리는 안돼있으니 유의!
			g_Prev_Cursor_Position_X = x - g_WindowSizeX * 0.5f;
			g_is_Right_Clicked = true;
		}
		else if (state == GLUT_UP && g_is_Right_Clicked)
		{
			g_Prev_Cursor_Position_X = 0.0f;
			g_is_Right_Clicked = false;
		}
	}
}

void Mouse_Drag_Input(int x, int y)
{
	if (g_is_Left_Clicked)
	{
		glm::vec3 pitch_yaw_roll(
			g_Prev_Cursor_Position_X - (x - g_WindowSizeX * 0.5f),
			(g_WindowSizeY * 0.5f - y) - g_Prev_Cursor_Position_Y,
			0.0f
		);
		g_Renderer->Camera_Rotate(pitch_yaw_roll, g_Elapsed_Time.count());
		g_Prev_Cursor_Position_X = x - g_WindowSizeX * 0.5f; g_Prev_Cursor_Position_Y = g_WindowSizeY * 0.5f - y;
	}

	else if (g_is_Right_Clicked)
	{
		glm::vec3 pitch_yaw_roll( 0.0f, 0.0f, g_Prev_Cursor_Position_X - (x - g_WindowSizeX * 0.5f) );
		g_Renderer->Camera_Rotate(pitch_yaw_roll, g_Elapsed_Time.count());
		g_Prev_Cursor_Position_X = x - g_WindowSizeX * 0.5f;
	}
}

void KeyInput(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 'w': g_Renderer->Camera_Translate(glm::vec3(0.0f, 0.0f, 1.0f), g_Elapsed_Time.count()); break;
	case 's': g_Renderer->Camera_Translate(glm::vec3(0.0f, 0.0f, -1.0f), g_Elapsed_Time.count()); break;
	case 'a': g_Renderer->Camera_Translate(glm::vec3(-1.0f, 0.0f, 0.0f), g_Elapsed_Time.count()); break;
	case 'd': g_Renderer->Camera_Translate(glm::vec3(1.0f, 0.0f, 0.0f), g_Elapsed_Time.count()); break;
	case 'r': g_Renderer->Initialize_Camera(); break;
	case 27: glutLeaveMainLoop(); break;
	}
}

void SpecialKeyInput(int key, int x, int y)
{
	switch (key)
	{
	case GLUT_KEY_UP: g_Renderer->Camera_Translate(glm::vec3(0.0f, 1.0f, 0.0f), g_Elapsed_Time.count()); break;
	case GLUT_KEY_DOWN: g_Renderer->Camera_Translate(glm::vec3(0.0f, -1.0f, 0.0f), g_Elapsed_Time.count()); break;
	}
}

int main(int argc, char **argv)
{
	// Initialize GL things
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(0, 0);
	glutInitWindowSize(g_WindowSizeX, g_WindowSizeY);
	glutCreateWindow("GLSL KPU");

	glewInit();
	if (glewIsSupported("GL_VERSION_4_6"))
	{
		std::cout << " GL Version is 4.6\n ";
	}
	else
	{
		std::cout << "GLEW 4.6 not supported\n ";
	}

	// Initialize Renderer
	g_Renderer = new Renderer(g_WindowSizeX, g_WindowSizeY);

	glutDisplayFunc(RenderScene);
	glutIdleFunc(Idle);
	glutKeyboardFunc(KeyInput);
	glutSpecialFunc(SpecialKeyInput);
	glutMouseFunc(MouseInput);
	glutMotionFunc(Mouse_Drag_Input);
	

	glutMainLoop();

	delete g_Renderer;

    return 0;
}

