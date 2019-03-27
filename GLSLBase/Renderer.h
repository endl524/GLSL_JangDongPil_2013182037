#pragma once

#include <string>
#include <cstdlib>
#include <fstream>
#include <iostream>


#include <random>
#include <chrono>


#include "Dependencies\glew.h"
#include "Dependencies\wglew.h"
#include "Dependencies\glm/glm.hpp"
#include "Dependencies\glm/gtc/matrix_transform.hpp"
#include "Dependencies\glm/gtx/euler_angles.hpp"

using namespace std;

#define PARTICLE_NUMS 1000
#define PARTICLE_HALF_SIZE 0.01f
#define PARTICLE_RAND_POS_MIN -0.99f
#define PARTICLE_RAND_POS_MAX 0.99f
#define PARTICLE_RAND_VELOCITY_MIN -1.0f
#define PARTICLE_RAND_VELOCITY_MAX 1.0f
#define PARTICLE_RAND_EMIT_TIME_MIN 0.0f
#define PARTICLE_RAND_EMIT_TIME_MAX 6.0f
#define PARTICLE_RAND_LIFE_TIME_MIN 1.0f
#define PARTICLE_RAND_LIFE_TIME_MAX 3.0f

class Renderer
{
private:
	bool m_Initialized = false;

	unsigned int m_WindowSizeX = 0;
	unsigned int m_WindowSizeY = 0;

	GLuint m_VBORect = 0;
	GLuint m_VBORectColor = 0;

	// Global Variable
	float m_Scale = 0.0f;
	float m_Time = 0.0f;

	// Shader Programs
	GLuint m_SolidRectShader = 0;
	GLuint m_SimpleParticleShader = 0;

	// Random Engine
	random_device m_Random_Device;
	mt19937_64 m_Random_Seed;
	uniform_real_distribution<> m_Random_Position;
	uniform_real_distribution<> m_Random_Veclocity;
	uniform_real_distribution<> m_Random_Emit_Time;
	uniform_real_distribution<> m_Random_Life_Time;
	
	// Chrono Engine
	chrono::system_clock::time_point m_Prev_Time;
	chrono::system_clock::time_point m_Curr_Time;

	// VBO Variable
	GLuint m_VBO_Particle = 0;
	GLuint m_Count_of_Particle_Vertice = 0;
	GLuint m_VBO_ProxyGeo = 0;
	GLuint m_Count_ProxyGeo = 0;

private:
	void Initialize(int windowSizeX, int windowSizeY);
	bool ReadFile(char* filename, std::string *target);
	void AddShader(GLuint ShaderProgram, const char* pShaderText, GLenum ShaderType);
	GLuint CompileShaders(char* filenameVS, char* filenameFS);
	void CreateVertexBufferObjects(); 
	
	unsigned char * Renderer::loadBMPRaw(const char * imagepath, unsigned int& outWidth, unsigned int& outHeight);

	// =============================
	
	void Random_Device_Setting();
	
	void CreateProxyGeometry();
	void Create_Lec4_Particle_VBO(const int& particle_Count);
	void Create_Lec5_Particle_VBO(const int& particle_Count);

public:
	Renderer(int windowSizeX, int windowSizeY);
	~Renderer();

	GLuint CreatePngTexture(char * filePath);
	GLuint CreateBmpTexture(char * filePath);

	void Test();
	void Draw_ProxyGeometry();
	void Draw_Lec4_Particle();
	void Draw_Lec5_Particle();
};

