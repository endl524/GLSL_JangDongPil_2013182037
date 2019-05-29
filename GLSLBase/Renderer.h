#pragma once

#include <string>
#include <cstdlib>
#include <fstream>
#include <iostream>


#include <random>


#include "Dependencies\glew.h"
#include "Dependencies\wglew.h"
#include "Dependencies\glm/glm.hpp"
#include "Dependencies\glm/gtc/matrix_transform.hpp"
#include "Dependencies\glm/gtx/euler_angles.hpp"

using namespace std;

// [Initial] ============================
#define PARTICLE_NUMS 100
#define PARTICLE_HALF_SIZE 0.1f
#define SANDBOX_HALF_SIZE 1.0f
#define FILLALL_RECT_HALF_SIZE 1.0f
#define SIMPLE_TEXTURE_HALF_SIZE 0.5f
#define PLANE_WAVE_WIDTH 1.0f
#define PLANE_WAVE_HEIGHT 1.0f

// [Random] =============================
#define PARTICLE_RAND_POS_MIN -0.99f
#define PARTICLE_RAND_POS_MAX 0.99f
#define PARTICLE_RAND_VELOCITY_MIN -1.0f
#define PARTICLE_RAND_VELOCITY_MAX 1.0f
#define PARTICLE_RAND_START_TIME_MIN 0.0f
#define PARTICLE_RAND_START_TIME_MAX 2.0f
#define PARTICLE_RAND_LIFE_TIME_MIN 0.5f
#define PARTICLE_RAND_LIFE_TIME_MAX 1.0f

#define PARTICLE_RAND_RATIO_MIN 2.0f
#define PARTICLE_RAND_RATIO_MAX 4.0f
#define PARTICLE_RAND_AMPLITUDE_MIN -0.1f
#define PARTICLE_RAND_AMPLITUDE_MAX 0.2f
#define PARTICLE_RAND_VALUE_MIN 0.0f
#define PARTICLE_RAND_VALUE_MAX 1.0f

#define PARTICLE_RAND_COLOR_MIN 0.0f
#define PARTICLE_RAND_COLOR_MAX 1.0f



// [end] ===================================================


static const GLulong s_Checker_Board[] =
{
0xFFFFFFFF, 0x00000000, 0xFFFFFFFF, 0x00000000, 0xFFFFFFFF, 0x00000000, 0xFFFFFFFF, 0x00000000,
0x00000000, 0xFFFFFFFF, 0x00000000, 0xFFFFFFFF, 0x00000000, 0xFFFFFFFF, 0x00000000, 0xFFFFFFFF,
0xFFFFFFFF, 0x00000000, 0xFFFFFFFF, 0x00000000, 0xFFFFFFFF, 0x00000000, 0xFFFFFFFF, 0x00000000,
0x00000000, 0xFFFFFFFF, 0x00000000, 0xFFFFFFFF, 0x00000000, 0xFFFFFFFF, 0x00000000, 0xFFFFFFFF,
0xFFFFFFFF, 0x00000000, 0xFFFFFFFF, 0x00000000, 0xFFFFFFFF, 0x00000000, 0xFFFFFFFF, 0x00000000,
0x00000000, 0xFFFFFFFF, 0x00000000, 0xFFFFFFFF, 0x00000000, 0xFFFFFFFF, 0x00000000, 0xFFFFFFFF,
0xFFFFFFFF, 0x00000000, 0xFFFFFFFF, 0x00000000, 0xFFFFFFFF, 0x00000000, 0xFFFFFFFF, 0x00000000,
0x00000000, 0xFFFFFFFF, 0x00000000, 0xFFFFFFFF, 0x00000000, 0xFFFFFFFF, 0x00000000, 0xFFFFFFFF
};


class Renderer
{
private:
	
	bool m_Initialized = false;

	// Window Size
	unsigned int m_WindowSizeX = 0;
	unsigned int m_WindowSizeY = 0;


	// Global Variable
	float m_Scale = 0.0f;
	float m_Time = 0.0f;
	int m_Number = 0;


	// Shader Programs
	GLuint m_SolidRectShader = 0;
	GLuint m_SimpleParticleShader = 0;
	GLuint m_Sin_Particle_Shader = 0;
	GLuint m_SandBox_Shader = 0;
	GLuint m_FillAll_Shader = 0;
	GLuint m_Simple_Texture_Shader = 0;
	GLuint m_VS_SandBox_Shader = 0;
	GLuint m_Simple_Cube_Shader = 0;


	// Random Engine
	random_device m_Random_Device;
	mt19937_64 m_Random_Seed;
	uniform_real_distribution<> m_Random_Position;
	uniform_real_distribution<> m_Random_Veclocity;
	uniform_real_distribution<> m_Random_Start_Time;
	uniform_real_distribution<> m_Random_Life_Time;
	uniform_real_distribution<> m_Random_Ratio;
	uniform_real_distribution<> m_Random_Amplitude;
	uniform_real_distribution<> m_Random_Value;
	uniform_real_distribution<> m_Random_Color;


	// VBO Variable
	GLuint m_VBORect = 0;
	GLuint m_VBORectColor = 0;

	GLuint m_VBO_Particle = 0;
	GLuint m_Count_of_Particle_Vertice = 0;

	GLuint m_VBO_Sin_Particle = 0;
	GLuint m_Count_of_Sin_Particle_Vertice = 0;

	GLuint m_VBO_SandBox = 0;
	
	GLuint m_VBO_FillAll = 0;

	GLuint m_VBO_Simple_Texture = 0;

	GLuint m_VBO_VS_SandBox = 0;
	GLuint m_Count_ProxyGeo = 0;

	GLuint m_VBO_Simple_Cube = 0;


	// Textures
	GLuint m_Particle_Texture_1 = 0;
	GLuint m_Particle_Texture_2 = 0;
	GLuint m_Particle_Texture_3 = 0;
	
	GLuint m_Check_Texture_ID = 0;
	
	GLuint m_Sample_RGB_Texture = 0;

	GLuint m_Full_Moon_Texture = 0;
	GLuint m_Wooden_Box_Texture = 0;
	GLuint m_Number_Texture = 0;
	GLuint m_Number_Texture_2 = 0;


	// Sprites
	GLuint m_Sans_Sprite = 0;
	GLuint m_Runner_Sprite = 0;
	GLuint m_Gunner_Sprite = 0;
	GLuint m_Runner_Sprite_2 = 0;
	GLuint m_Flame_Sprite = 0;


	// Matrices
	glm::mat4 m_View_Mat4;
	glm::mat4 m_Ortho_Proj_Mat4;
	glm::mat4 m_View_Proj_Mat4;
	glm::vec3 m_Camera_Pos_Vec3;
	glm::vec3 m_Camera_Up_Vec3;
	glm::vec3 m_Camera_Lookat_Vec3;


	// [end] ===================================================



private:
	
	// Initializer
	void Initialize(int windowSizeX, int windowSizeY);
	void Random_Device_Setting();
	void Matrices_Setting();

	// Utilities
	bool ReadFile(char* filename, std::string *target);
	void AddShader(GLuint ShaderProgram, const char* pShaderText, GLenum ShaderType);
	GLuint CompileShaders(char* filenameVS, char* filenameFS);
	unsigned char * Renderer::loadBMPRaw(const char * imagepath, unsigned int& outWidth, unsigned int& outHeight);


	// VBO Create
	void Create_Vertex_Buffer_Objects();
	void Create_Lec4_Particle_VBO(const int& particle_Count);
	void Create_Lec5_Particle_VBO(const int& particle_Count);
	void Create_Sin_Particle_VBO(const int& particle_Count);
	void Create_SandBox_VBO();
	void Create_FillAll_VBO();
	void Create_Simple_Texture_VBO();
	void Creat_Simple_Cube_VBO();
	void Create_VS_SandBox_VBO();


	// Draw Methods
	void Test();
	void Draw_VS_SandBox();
	void Draw_Lec4_Particle();
	void Draw_Lec5_Particle();
	void Draw_Sin_Particle();
	void Draw_SandBox();
	void Fill_All(const float& alpha);
	void Draw_Simple_Texture();
	void Draw_Simple_Cube();
	

	// [end] ===================================================



public:

	// Class Create & Destroy
	Renderer(int windowSizeX, int windowSizeY);
	~Renderer();


	// Texture Create Method
	GLuint CreatePngTexture(char* filePath);
	GLuint CreateBmpTexture(char* filePath);


	// Rendering
	void Rendering(const float& elapsed_time);


	// [end] ===================================================
};

