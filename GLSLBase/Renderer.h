#pragma once

#include <string>
#include <cstdlib>
#include <fstream>
#include <iostream>


#include <random>
#include <vector>


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

class Renderer
{
private:
	bool m_Initialized = false;

	unsigned int m_WindowSizeX = 0;
	unsigned int m_WindowSizeY = 0;

	GLuint m_VBORect = 0;
	GLuint m_VBORectColor = 0;
	float m_Scale = 0.0f;
	float m_Time = 0.0f;

	// Shader Programs
	GLuint m_SolidRectShader = 0;
	GLuint m_SimpleParticleShader = 0;

	random_device m_Random_Device;
	mt19937_64 m_Random_Seed;
	uniform_real_distribution<> m_Random_Position;
	uniform_real_distribution<> m_Random_Veclocity;

	vector<GLuint> m_Gen_Quads_VBO_IDs_Vector;


	GLuint m_VBO_Particle = 0;
	GLuint m_Size_of_Particle = 0;
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
	
	void Create_Particle_VBO(const int& particle_Count);
	void CreateProxyGeometry();

public:
	Renderer(int windowSizeX, int windowSizeY);
	~Renderer();

	GLuint CreatePngTexture(char * filePath);
	GLuint CreateBmpTexture(char * filePath);

	void Test();
	void Draw_Particle();
	void Draw_ProxyGeometry();
};

