#include "stdafx.h"
#include "Renderer.h"
#include "LoadPng.h"
#include <Windows.h>
#include <cstdlib>
#include <cassert>

Renderer::Renderer(int windowSizeX, int windowSizeY)
{
	Initialize(windowSizeX, windowSizeY);
}


Renderer::~Renderer()
{
}

void Renderer::Initialize(int windowSizeX, int windowSizeY)
{
	//Set window size
	m_WindowSizeX = windowSizeX;
	m_WindowSizeY = windowSizeY;

	//Load shaders
	m_SolidRectShader = CompileShaders("./Shaders/SolidRect.vs", "./Shaders/SolidRect.fs");
	m_SimpleParticleShader = CompileShaders("./Shaders/SimpleParticle.vs", "./Shaders/SimpleParticle.fs");
	m_Sin_Particle_Shader = CompileShaders("./Shaders/SinParticle.vs", "./Shaders/SinParticle.fs");

	//Random Device Setting
	Random_Device_Setting();

	//Create VBOs
	//CreateVertexBufferObjects();

	//Create Particle
	//Create_Lec4_Particle_VBO(PARTICLE_NUMS);
	//Create_Lec5_Particle_VBO(PARTICLE_NUMS);
	Create_Sin_Particle_VBO(PARTICLE_NUMS);

	//Create Proxy Geometry
	//CreateProxyGeometry();
}

void Renderer::Random_Device_Setting()
{
	mt19937_64 temp_seed(m_Random_Device());
	m_Random_Seed = temp_seed;

	uniform_real_distribution<> temp_random_pos(PARTICLE_RAND_POS_MIN, PARTICLE_RAND_POS_MAX);
	m_Random_Position = temp_random_pos;

	uniform_real_distribution<> temp_random_velo(PARTICLE_RAND_VELOCITY_MIN, PARTICLE_RAND_VELOCITY_MAX);
	m_Random_Veclocity = temp_random_velo;

	uniform_real_distribution<> temp_random_emit_time(PARTICLE_RAND_EMIT_TIME_MIN, PARTICLE_RAND_EMIT_TIME_MAX);
	m_Random_Emit_Time = temp_random_emit_time;

	uniform_real_distribution<> temp_random_life_time(PARTICLE_RAND_LIFE_TIME_MIN, PARTICLE_RAND_LIFE_TIME_MAX);
	m_Random_Life_Time = temp_random_life_time;
}

void Renderer::CreateVertexBufferObjects() // Renderer::Test()�� ���� VBO ������
{
	float size = 0.02f;
	float rect[]
		=
	{
		-size, -size, 0.0f, 0.5f,
		-size, size, 0.0f, 0.5f,
		size, size, 0.0f, 0.5f, //Triangle1 Complete
		-size, -size, 0.0f, 0.5f,
		size, size, 0.0f, 0.5f,
		size, -size, 0.0f, 0.5f //Triangle2 Complete
	};

	glGenBuffers(1, &m_VBORect); 
	// GPU���� �����͸� ����ϱ� ���� "Buffer Object"�� �����ϰ�,
	// ������ VBO�� "Object ID"�� <�ι�° ����>�� �޾ƿ´�.
	// <ù��° ����>�� ������ ������ ������ �ǹ��Ѵ�.
	// <�ι�° ����> ���� ������ ������ŭ ������ ID�� ����.

	glBindBuffer(GL_ARRAY_BUFFER, m_VBORect);
	// ������ VBO�� ID�� ����Ͽ� Bind �Ѵ�.
	// Bind ��? -> ���� �۾��� ����� ������ �ִ� ��.
	
	// GL_ARRAY_BUFFER�� "�۾���"�� �� ������� �����ϸ� �ȴ�.
	// ��, ����� GL_ARRAY_BUFFER ��� �۾��뿡 VBO�� �ϳ� �÷��� �����̴�.
	
	// [[ VBO�� ������ � �۾��� �ϱ� ������ �ݵ�� �� VBO�� Bind �ؾ��Ѵ�! ]]

	glBufferData(GL_ARRAY_BUFFER, sizeof(rect), rect, GL_STATIC_DRAW);
	// Bind�� VBO�� �����͸� �Ҵ��Ѵ�. (���� �� �����̱� ������..)
	// glBindBuffer ȣ�� �Ŀ� glBufferData�� �ٷ� ȣ���ϴ� ���� �Ǵ�.
	// �ָ��� VBO�� ������ �Ҵ��� �ϰԵ� ���� �ֱ� �����̴�. (�۾��뿡 �ٸ� VBO�� �ö��ִٴ���..)
	// ���⼭ sizeof(rect)�� �����͵��� ��ü ����� �ǹ�.


	// �Ȱ��� color�� ������ش�.
	float color[]
		=
	{
		1.0f, 0.0f, 0.0f, 1.0f,
		1.0f, 0.0f, 0.0f, 1.0f,
		1.0f, 0.0f, 0.0f, 1.0f, //Triangle1 Complete
		1.0f, 0.0f, 0.0f, 1.0f,
		1.0f, 0.0f, 0.0f, 1.0f,
		1.0f, 0.0f, 0.0f, 1.0f //Triangle2 Complete
	};
	glGenBuffers(1, &m_VBORectColor);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBORectColor);
	glBufferData(GL_ARRAY_BUFFER, sizeof(color), color, GL_STATIC_DRAW);
}

void Renderer::Create_Lec4_Particle_VBO(const int& particle_Count)
{
	float temp_Pos_X, temp_Pos_Y;
	float temp_Velocity_X, temp_Velocity_Y, temp_Velocity_Z;
	float temp_Emit_Time, temp_Life_Time;

	m_Count_of_Particle_Vertice = particle_Count * 6;
	int array_Length = m_Count_of_Particle_Vertice * 6;
	float* Particles_Vertice = new float[array_Length];

	for (int i = 0; i < array_Length; ++i)
	{
		temp_Pos_X = m_Random_Position(m_Random_Seed);
		temp_Pos_Y = m_Random_Position(m_Random_Seed);
		temp_Velocity_X = m_Random_Veclocity(m_Random_Seed);
		temp_Velocity_Y = m_Random_Veclocity(m_Random_Seed);
		temp_Velocity_Z = 0.0f;

		Particles_Vertice[i++] = temp_Pos_X - PARTICLE_HALF_SIZE; // Pos_X
		Particles_Vertice[i++] = temp_Pos_Y + PARTICLE_HALF_SIZE; // Pos_Y
		Particles_Vertice[i++] = 0.0f; // Pos_Z
		Particles_Vertice[i++] = temp_Velocity_X; // Velo_X
		Particles_Vertice[i++] = temp_Velocity_Y; // Velo_Y
		Particles_Vertice[i++] = temp_Velocity_Z; // Velo_Z

		Particles_Vertice[i++] = temp_Pos_X - PARTICLE_HALF_SIZE;
		Particles_Vertice[i++] = temp_Pos_Y - PARTICLE_HALF_SIZE;
		Particles_Vertice[i++] = 0.0f;
		Particles_Vertice[i++] = temp_Velocity_X;
		Particles_Vertice[i++] = temp_Velocity_Y;
		Particles_Vertice[i++] = temp_Velocity_Z;

		Particles_Vertice[i++] = temp_Pos_X + PARTICLE_HALF_SIZE;
		Particles_Vertice[i++] = temp_Pos_Y + PARTICLE_HALF_SIZE;
		Particles_Vertice[i++] = 0.0f;
		Particles_Vertice[i++] = temp_Velocity_X;
		Particles_Vertice[i++] = temp_Velocity_Y;
		Particles_Vertice[i++] = temp_Velocity_Z;

		Particles_Vertice[i++] = temp_Pos_X + PARTICLE_HALF_SIZE;
		Particles_Vertice[i++] = temp_Pos_Y + PARTICLE_HALF_SIZE;
		Particles_Vertice[i++] = 0.0f;
		Particles_Vertice[i++] = temp_Velocity_X;
		Particles_Vertice[i++] = temp_Velocity_Y;
		Particles_Vertice[i++] = temp_Velocity_Z;

		Particles_Vertice[i++] = temp_Pos_X - PARTICLE_HALF_SIZE;
		Particles_Vertice[i++] = temp_Pos_Y - PARTICLE_HALF_SIZE;
		Particles_Vertice[i++] = 0.0f;
		Particles_Vertice[i++] = temp_Velocity_X;
		Particles_Vertice[i++] = temp_Velocity_Y;
		Particles_Vertice[i++] = temp_Velocity_Z;

		Particles_Vertice[i++] = temp_Pos_X + PARTICLE_HALF_SIZE;
		Particles_Vertice[i++] = temp_Pos_Y - PARTICLE_HALF_SIZE;
		Particles_Vertice[i++] = 0.0f;
		Particles_Vertice[i++] = temp_Velocity_X;
		Particles_Vertice[i++] = temp_Velocity_Y;
		Particles_Vertice[i] = temp_Velocity_Z;
	}

	glGenBuffers(1, &m_VBO_Particle);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO_Particle);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * array_Length, Particles_Vertice, GL_STATIC_DRAW);

	delete[] Particles_Vertice;
}

void Renderer::Create_Lec5_Particle_VBO(const int& particle_Count)
{
	float temp_Pos_X, temp_Pos_Y;
	float temp_Velocity_X, temp_Velocity_Y, temp_Velocity_Z;
	float temp_Emit_Time, temp_Life_Time; // Start_Time, Life_Time

	// ��ƼŬ ���� ���� = particle_Count
	// ��ƼŬ �������� ���� ���� = 6
	// ������ ������ ���� = 8
	m_Count_of_Particle_Vertice = particle_Count * 6;
	int array_Length = m_Count_of_Particle_Vertice * 8;
	float* Particles_Vertice = new float[array_Length];

	for (int i = 0; i < array_Length; ++i)
	{
		temp_Pos_X = m_Random_Position(m_Random_Seed);
		temp_Pos_Y = m_Random_Position(m_Random_Seed);
		temp_Velocity_X = m_Random_Veclocity(m_Random_Seed);
		temp_Velocity_Y = m_Random_Veclocity(m_Random_Seed);
		temp_Velocity_Z = 0.0f;
		temp_Emit_Time = m_Random_Emit_Time(m_Random_Seed);
		temp_Life_Time = m_Random_Life_Time(m_Random_Seed);

		Particles_Vertice[i++] = temp_Pos_X - PARTICLE_HALF_SIZE; // Pos_X
		Particles_Vertice[i++] = temp_Pos_Y + PARTICLE_HALF_SIZE; // Pos_Y
		Particles_Vertice[i++] = 0.0f; // Pos_Z
		Particles_Vertice[i++] = temp_Velocity_X; // Velo_X
		Particles_Vertice[i++] = temp_Velocity_Y; // Velo_Y
		Particles_Vertice[i++] = temp_Velocity_Z; // Velo_Z
		Particles_Vertice[i++] = temp_Emit_Time; // Emit_Time
		Particles_Vertice[i++] = temp_Life_Time; // Life_Time

		Particles_Vertice[i++] = temp_Pos_X - PARTICLE_HALF_SIZE;
		Particles_Vertice[i++] = temp_Pos_Y - PARTICLE_HALF_SIZE;
		Particles_Vertice[i++] = 0.0f;
		Particles_Vertice[i++] = temp_Velocity_X;
		Particles_Vertice[i++] = temp_Velocity_Y;
		Particles_Vertice[i++] = temp_Velocity_Z;
		Particles_Vertice[i++] = temp_Emit_Time;
		Particles_Vertice[i++] = temp_Life_Time;

		Particles_Vertice[i++] = temp_Pos_X + PARTICLE_HALF_SIZE;
		Particles_Vertice[i++] = temp_Pos_Y + PARTICLE_HALF_SIZE;
		Particles_Vertice[i++] = 0.0f;
		Particles_Vertice[i++] = temp_Velocity_X;
		Particles_Vertice[i++] = temp_Velocity_Y;
		Particles_Vertice[i++] = temp_Velocity_Z;
		Particles_Vertice[i++] = temp_Emit_Time;
		Particles_Vertice[i++] = temp_Life_Time;

		Particles_Vertice[i++] = temp_Pos_X + PARTICLE_HALF_SIZE;
		Particles_Vertice[i++] = temp_Pos_Y + PARTICLE_HALF_SIZE;
		Particles_Vertice[i++] = 0.0f;
		Particles_Vertice[i++] = temp_Velocity_X;
		Particles_Vertice[i++] = temp_Velocity_Y;
		Particles_Vertice[i++] = temp_Velocity_Z;
		Particles_Vertice[i++] = temp_Emit_Time;
		Particles_Vertice[i++] = temp_Life_Time;

		Particles_Vertice[i++] = temp_Pos_X - PARTICLE_HALF_SIZE;
		Particles_Vertice[i++] = temp_Pos_Y - PARTICLE_HALF_SIZE;
		Particles_Vertice[i++] = 0.0f;
		Particles_Vertice[i++] = temp_Velocity_X;
		Particles_Vertice[i++] = temp_Velocity_Y;
		Particles_Vertice[i++] = temp_Velocity_Z;
		Particles_Vertice[i++] = temp_Emit_Time;
		Particles_Vertice[i++] = temp_Life_Time;

		Particles_Vertice[i++] = temp_Pos_X + PARTICLE_HALF_SIZE;
		Particles_Vertice[i++] = temp_Pos_Y - PARTICLE_HALF_SIZE;
		Particles_Vertice[i++] = 0.0f;
		Particles_Vertice[i++] = temp_Velocity_X;
		Particles_Vertice[i++] = temp_Velocity_Y;
		Particles_Vertice[i++] = temp_Velocity_Z;
		Particles_Vertice[i++] = temp_Emit_Time;
		Particles_Vertice[i] = temp_Life_Time;
	}

	glGenBuffers(1, &m_VBO_Particle);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO_Particle);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * array_Length, Particles_Vertice, GL_STATIC_DRAW);

	delete[] Particles_Vertice;
}

void Renderer::CreateProxyGeometry()
{
	float basePosX = -0.5f;
	float basePosY = -0.5f;
	float targetPosX = 0.5f;
	float targetPosY = 0.5f;

	int pointCountX = 32;
	int pointCountY = 32;

	float width = targetPosX - basePosX;
	float height = targetPosY - basePosY;

	float* point = new float[pointCountX * pointCountY * 2];
	float* vertices = new float[(pointCountX - 1) * (pointCountY - 1) * 2 * 3 * 3];
	m_Count_ProxyGeo = (pointCountX - 1) * (pointCountY - 1) * 2 * 3;

	// prepare points
	for (int x = 0; x < pointCountX; ++x)
	{
		for (int y = 0; y < pointCountY; ++y)
		{
			point[(y * pointCountX + x) * 2 + 0] = basePosX + width * (x / (float)(pointCountX - 1));
			point[(y * pointCountX + x) * 2 + 1] = basePosY + height * (y / (float)(pointCountY - 1));
		}
	}

	// Make Triangles
	int vertIndex = 0;
	for (int x = 0; x < pointCountX - 1; ++x)
	{
		for (int y = 0; y < pointCountY - 1; ++y)
		{
			// Triangle part 1
			vertices[vertIndex] = point[(y * pointCountX + x) * 2 + 0];
			++vertIndex;
			vertices[vertIndex] = point[(y * pointCountX + x) * 2 + 1];
			++vertIndex;
			vertices[vertIndex] = 0.0f;
			++vertIndex;

			vertices[vertIndex] = point[((y + 1) * pointCountX + (x + 1)) * 2 + 0];
			++vertIndex;
			vertices[vertIndex] = point[((y + 1) * pointCountX + (x + 1)) * 2 + 1];
			++vertIndex;
			vertices[vertIndex] = 0.0f;
			++vertIndex;

			vertices[vertIndex] = point[((y + 1) * pointCountX + x) * 2 + 0];
			++vertIndex;
			vertices[vertIndex] = point[((y + 1) * pointCountX + x) * 2 + 1];
			++vertIndex;
			vertices[vertIndex] = 0.0f;
			++vertIndex;

			// Triangle part 2
			vertices[vertIndex] = point[(y * pointCountX + x) * 2 + 0];
			++vertIndex;
			vertices[vertIndex] = point[(y * pointCountX + x) * 2 + 1];
			++vertIndex;
			vertices[vertIndex] = 0.0f;
			++vertIndex;

			vertices[vertIndex] = point[(y * pointCountX + (x + 1)) * 2 + 0];
			++vertIndex;
			vertices[vertIndex] = point[(y * pointCountX + (x + 1)) * 2 + 1];
			++vertIndex;
			vertices[vertIndex] = 0.0f;
			++vertIndex;

			vertices[vertIndex] = point[((y + 1) * pointCountX + (x + 1)) * 2 + 0];
			++vertIndex;
			vertices[vertIndex] = point[((y + 1) * pointCountX + (x + 1)) * 2 + 1];
			++vertIndex;
			vertices[vertIndex] = 0.0f;
			++vertIndex;
		}
	}

	glGenBuffers(1, &m_VBO_ProxyGeo);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO_ProxyGeo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * (pointCountX - 1) * (pointCountY - 1) * 2 * 3 * 3, vertices, GL_STATIC_DRAW);

	delete[] vertices;
}

void Renderer::Create_Sin_Particle_VBO(const int& particle_Count)
{
	float temp_Pos_X, temp_Pos_Y;
	float temp_Emit_Time, temp_Life_Time; // Start_Time, Life_Time

	// ��ƼŬ ���� ���� = particle_Count
	// ��ƼŬ �������� ���� ���� = 6
	// ������ ������ ���� = 8
	m_Count_of_Sin_Particle_Vertice = particle_Count * 6;
	int array_Length = m_Count_of_Sin_Particle_Vertice * 8;
	float* Particles_Vertice = new float[array_Length];

	for (int i = 0; i < array_Length; ++i)
	{
		temp_Emit_Time = m_Random_Emit_Time(m_Random_Seed);
		temp_Life_Time = m_Random_Life_Time(m_Random_Seed);

		Particles_Vertice[i++] = -PARTICLE_HALF_SIZE; // Pos_X
		Particles_Vertice[i++] = PARTICLE_HALF_SIZE; // Pos_Y
		Particles_Vertice[i++] = 0.0f; // Pos_Z
		Particles_Vertice[i++] = temp_Emit_Time; // Emit_Time
		Particles_Vertice[i++] = temp_Life_Time; // Life_Time

		Particles_Vertice[i++] = -PARTICLE_HALF_SIZE;
		Particles_Vertice[i++] = -PARTICLE_HALF_SIZE;
		Particles_Vertice[i++] = 0.0f;
		Particles_Vertice[i++] = temp_Emit_Time;
		Particles_Vertice[i++] = temp_Life_Time;

		Particles_Vertice[i++] = PARTICLE_HALF_SIZE;
		Particles_Vertice[i++] = PARTICLE_HALF_SIZE;
		Particles_Vertice[i++] = 0.0f;
		Particles_Vertice[i++] = temp_Emit_Time;
		Particles_Vertice[i++] = temp_Life_Time;

		Particles_Vertice[i++] = PARTICLE_HALF_SIZE;
		Particles_Vertice[i++] = -PARTICLE_HALF_SIZE;
		Particles_Vertice[i++] = 0.0f;
		Particles_Vertice[i++] = temp_Emit_Time;
		Particles_Vertice[i++] = temp_Life_Time;

		Particles_Vertice[i++] = PARTICLE_HALF_SIZE;
		Particles_Vertice[i++] = PARTICLE_HALF_SIZE;
		Particles_Vertice[i++] = 0.0f;
		Particles_Vertice[i++] = temp_Emit_Time;
		Particles_Vertice[i++] = temp_Life_Time;

		Particles_Vertice[i++] = -PARTICLE_HALF_SIZE;
		Particles_Vertice[i++] = -PARTICLE_HALF_SIZE;
		Particles_Vertice[i++] = 0.0f;
		Particles_Vertice[i++] = temp_Emit_Time;
		Particles_Vertice[i] = temp_Life_Time;
	}

	glGenBuffers(1, &m_VBO_Sin_Particle);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO_Sin_Particle);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * array_Length, Particles_Vertice, GL_STATIC_DRAW);

	delete[] Particles_Vertice;
}


void Renderer::AddShader(GLuint ShaderProgram, const char* pShaderText, GLenum ShaderType)
{
	//���̴� ������Ʈ ����
	GLuint ShaderObj = glCreateShader(ShaderType);

	if (ShaderObj == 0) {
		fprintf(stderr, "Error creating shader type %d\n", ShaderType);
	}

	const GLchar* p[1];
	p[0] = pShaderText;
	GLint Lengths[1];
	Lengths[0] = (GLint)strlen(pShaderText);
	//���̴� �ڵ带 ���̴� ������Ʈ�� �Ҵ�
	glShaderSource(ShaderObj, 1, p, Lengths);

	//�Ҵ�� ���̴� �ڵ带 ������
	glCompileShader(ShaderObj);

	GLint success;
	// ShaderObj �� ���������� ������ �Ǿ����� Ȯ��
	glGetShaderiv(ShaderObj, GL_COMPILE_STATUS, &success);
	if (!success) {
		GLchar InfoLog[1024];

		//OpenGL �� shader log �����͸� ������
		glGetShaderInfoLog(ShaderObj, 1024, NULL, InfoLog);
		fprintf(stderr, "Error compiling shader type %d: '%s'\n", ShaderType, InfoLog);
		printf("%s \n", pShaderText);
	}

	// ShaderProgram �� attach!!
	glAttachShader(ShaderProgram, ShaderObj);
}

bool Renderer::ReadFile(char* filename, std::string *target)
{
	std::ifstream file(filename);
	if (file.fail())
	{
		std::cout << filename << " file loading failed.. \n";
		file.close();
		return false;
	}
	std::string line;
	while (getline(file, line)) {
		target->append(line.c_str());
		target->append("\n");
	}
	return true;
}

GLuint Renderer::CompileShaders(char* filenameVS, char* filenameFS)
{
	GLuint ShaderProgram = glCreateProgram(); //�� ���̴� ���α׷� ����

	if (ShaderProgram == 0) { //���̴� ���α׷��� ����������� Ȯ��
		fprintf(stderr, "Error creating shader program\n");
	}

	std::string vs, fs;

	//shader.vs �� vs ������ �ε���
	if (!ReadFile(filenameVS, &vs)) {
		printf("Error compiling vertex shader\n");
		return -1;
	};

	//shader.fs �� fs ������ �ε���
	if (!ReadFile(filenameFS, &fs)) {
		printf("Error compiling fragment shader\n");
		return -1;
	};

	// ShaderProgram �� vs.c_str() ���ؽ� ���̴��� �������� ����� attach��
	AddShader(ShaderProgram, vs.c_str(), GL_VERTEX_SHADER);

	// ShaderProgram �� fs.c_str() �����׸�Ʈ ���̴��� �������� ����� attach��
	AddShader(ShaderProgram, fs.c_str(), GL_FRAGMENT_SHADER);

	GLint Success = 0;
	GLchar ErrorLog[1024] = { 0 };

	//Attach �Ϸ�� shaderProgram �� ��ŷ��
	glLinkProgram(ShaderProgram);

	//��ũ�� �����ߴ��� Ȯ��
	glGetProgramiv(ShaderProgram, GL_LINK_STATUS, &Success);

	if (Success == 0) {
		// shader program �α׸� �޾ƿ�
		glGetProgramInfoLog(ShaderProgram, sizeof(ErrorLog), NULL, ErrorLog);
		std::cout << filenameVS << ", " << filenameFS << " Error linking shader program\n" << ErrorLog;
		return -1;
	}

	glValidateProgram(ShaderProgram);
	glGetProgramiv(ShaderProgram, GL_VALIDATE_STATUS, &Success);
	if (!Success) {
		glGetProgramInfoLog(ShaderProgram, sizeof(ErrorLog), NULL, ErrorLog);
		std::cout << filenameVS << ", " << filenameFS << " Error validating shader program\n" << ErrorLog;
		return -1;
	}

	glUseProgram(ShaderProgram);
	std::cout << filenameVS << ", " << filenameFS << " Shader compiling is done.\n";

	return ShaderProgram;
}

unsigned char * Renderer::loadBMPRaw(const char * imagepath, unsigned int& outWidth, unsigned int& outHeight)
{
	std::cout << "Loading bmp file " << imagepath << " ... " << std::endl;
	outWidth = -1;
	outHeight = -1;
	// Data read from the header of the BMP file
	unsigned char header[54];
	unsigned int dataPos;
	unsigned int imageSize;
	// Actual RGB data
	unsigned char * data;

	// Open the file
	FILE * file = NULL;
	fopen_s(&file, imagepath, "rb");
	if (!file)
	{
		std::cout << "Image could not be opened, " << imagepath << " is missing. " << std::endl;
		return NULL;
	}

	if (fread(header, 1, 54, file) != 54)
	{
		std::cout << imagepath << " is not a correct BMP file. " << std::endl;
		return NULL;
	}

	if (header[0] != 'B' || header[1] != 'M')
	{
		std::cout << imagepath << " is not a correct BMP file. " << std::endl;
		return NULL;
	}

	if (*(int*)&(header[0x1E]) != 0)
	{
		std::cout << imagepath << " is not a correct BMP file. " << std::endl;
		return NULL;
	}

	if (*(int*)&(header[0x1C]) != 24)
	{
		std::cout << imagepath << " is not a correct BMP file. " << std::endl;
		return NULL;
	}

	dataPos = *(int*)&(header[0x0A]);
	imageSize = *(int*)&(header[0x22]);
	outWidth = *(int*)&(header[0x12]);
	outHeight = *(int*)&(header[0x16]);

	if (imageSize == 0)
		imageSize = outWidth * outHeight * 3;

	if (dataPos == 0)
		dataPos = 54;

	data = new unsigned char[imageSize];

	fread(data, 1, imageSize, file);

	fclose(file);

	std::cout << imagepath << " is succesfully loaded. " << std::endl;

	return data;
}

GLuint Renderer::CreatePngTexture(char * filePath)
{
	//Load Pngs: Load file and decode image.
	std::vector<unsigned char> image;
	unsigned width, height;
	unsigned error = lodepng::decode(image, width, height, filePath);
	if (error != 0)
	{
		lodepng_error_text(error);
		assert(error == 0);
		return -1;
	}

	GLuint temp;
	glGenTextures(1, &temp);

	glBindTexture(GL_TEXTURE_2D, temp);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, &image[0]);

	return temp;
}

GLuint Renderer::CreateBmpTexture(char * filePath)
{
	//Load Bmp: Load file and decode image.
	unsigned int width, height;
	unsigned char * bmp
		= loadBMPRaw(filePath, width, height);

	if (bmp == NULL)
	{
		std::cout << "Error while loading bmp file : " << filePath << std::endl;
		assert(bmp != NULL);
		return -1;
	}

	GLuint temp;
	glGenTextures(1, &temp);

	glBindTexture(GL_TEXTURE_2D, temp);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, bmp);

	return temp;
}

void Renderer::Test()
{
	GLuint shader_ID = m_SolidRectShader;

	glUseProgram(shader_ID);

	if (m_Scale > 1.0f) { m_Scale = 0.0f; }
	m_Scale += 0.01f;

	GLuint u_Time = glGetUniformLocation(shader_ID, "u_Time");
	glUniform1f(u_Time, m_Scale); // �ش� Uniform ������ ���� �����Ѵ�.

	GLuint a_Position = glGetAttribLocation(shader_ID, "a_Position");
	// �ش� Shader�� "a_Position" �̶�� Attribute�� ��ȣ�� �޾ƿ´�.

	glEnableVertexAttribArray(a_Position);
	// �޾ƿ� ��ȣ�� Attribute�� ����ϱ� ���� Enable ��Ű�� ��.

	glBindBuffer(GL_ARRAY_BUFFER, m_VBORect);
	// m_VBORect�� ���� ID�� �ҷ��ͼ� "�۾���"�� �ø���.

	glVertexAttribPointer(a_Position, 4, GL_FLOAT, GL_FALSE, sizeof(float) * 4, 0);
	// Draw �� �����͸� �о ������ ũ�� �� �������� �����Ѵ�. ���� => (Attribute Number, ???, ���� �ڷ���, ???, �ǳ� �� offset ũ��, ������ index)
	// (���⼭�� Float Pointer�� ����ϰ� �ִ�.)
	// ���� ���� ������ 3���� ��ġ�������� �̷�����ֱ� ������ sizeof(float) * 3�� �Ѵ�.
	// (3�� ���� ������ �����Ͱ� 3�������� �̷�����ֱ� ������, ���� ���� uv���� �߰��ȴٸ� 5�� ���ؾ��Ѵ�.)

	GLuint a_Color = glGetAttribLocation(shader_ID, "a_Color");
	glEnableVertexAttribArray(a_Color);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBORectColor);
	glVertexAttribPointer(a_Color, 4, GL_FLOAT, GL_FALSE, sizeof(float) * 4, 0);

	glDrawArrays(GL_TRIANGLES, 0, 6);
	// �������� ������ ������ ũ�⸦ ��������, �迭�� �׸���� ȣ���Ѵ�.
	// (Primitive�� ����.)
	// "0�� ����" ���� "6��"��..


	glDisableVertexAttribArray(a_Position);
	// �۾��� �Ϸ������� �ش� Attribute�� Disable ��Ų��.
	glDisableVertexAttribArray(a_Color);
}

void Renderer::Draw_Lec4_Particle()
{
	GLuint shader_ID = m_SimpleParticleShader;

	glUseProgram(shader_ID);

	GLuint u_Time = glGetUniformLocation(shader_ID, "u_Time");
	glUniform1f(u_Time, m_Time); // �ش� Uniform ������ ���� �����Ѵ�.
	m_Time += 0.01f;

	GLuint a_Position = glGetAttribLocation(shader_ID, "a_Position");
	GLuint a_Velocity = glGetAttribLocation(shader_ID, "a_Velocity");
	glEnableVertexAttribArray(a_Position);
	glEnableVertexAttribArray(a_Velocity);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO_Particle);
	glVertexAttribPointer(a_Position, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, 0);
	glVertexAttribPointer(a_Velocity, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, (GLvoid*)(sizeof(float) * 3));

	glDrawArrays(GL_TRIANGLES, 0, m_Count_of_Particle_Vertice);
	glDisableVertexAttribArray(a_Position);
	glDisableVertexAttribArray(a_Velocity);
}

void Renderer::Draw_Lec5_Particle()
{
	GLuint shader_ID = m_SimpleParticleShader;

	glUseProgram(shader_ID);

	GLuint u_Time = glGetUniformLocation(shader_ID, "u_Time");
	GLuint u_Repeat = glGetUniformLocation(shader_ID, "u_Repeat");
	glUniform1f(u_Time, m_Time);
	m_Time += 0.01f;

	GLuint a_Position = glGetAttribLocation(shader_ID, "a_Position");
	GLuint a_Velocity = glGetAttribLocation(shader_ID, "a_Velocity");
	GLuint a_Emit_and_Life_Time = glGetAttribLocation(shader_ID, "a_Emit_and_Life_Time");
	
	glEnableVertexAttribArray(a_Position);
	glEnableVertexAttribArray(a_Velocity);
	glEnableVertexAttribArray(a_Emit_and_Life_Time);

	glBindBuffer(GL_ARRAY_BUFFER, m_VBO_Particle);
	glVertexAttribPointer(a_Position, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, 0);
	glVertexAttribPointer(a_Velocity, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (GLvoid*)(sizeof(float) * 3));
	glVertexAttribPointer(a_Emit_and_Life_Time, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (GLvoid*)(sizeof(float) * 6));

	glDrawArrays(GL_TRIANGLES, 0, m_Count_of_Particle_Vertice);
	glDisableVertexAttribArray(a_Position);
	glDisableVertexAttribArray(a_Velocity);
	glDisableVertexAttribArray(a_Emit_and_Life_Time);
}

void Renderer::Draw_ProxyGeometry()
{
	GLuint shader_ID = m_SolidRectShader;

	glUseProgram(shader_ID);

	int attribPosition = glGetAttribLocation(shader_ID, "a_Position");

	glEnableVertexAttribArray(attribPosition);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO_ProxyGeo);
	glVertexAttribPointer(attribPosition, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, 0);

	glDrawArrays(GL_TRIANGLES, 0, m_Count_ProxyGeo);
	glDisableVertexAttribArray(attribPosition);
}

void Renderer::Draw_Sin_Particle()
{
	GLuint shader_ID = m_Sin_Particle_Shader;

	glUseProgram(shader_ID);

	GLuint u_Time = glGetUniformLocation(shader_ID, "u_Time");
	GLuint u_Repeat = glGetUniformLocation(shader_ID, "u_Repeat");
	glUniform1f(u_Time, m_Time);
	m_Time += 0.01f;

	GLuint a_Position = glGetAttribLocation(shader_ID, "a_Position");
	GLuint a_Emit_and_Life_Time = glGetAttribLocation(shader_ID, "a_Emit_and_Life_Time");

	glEnableVertexAttribArray(a_Position);
	glEnableVertexAttribArray(a_Emit_and_Life_Time);

	glBindBuffer(GL_ARRAY_BUFFER, m_VBO_Sin_Particle);
	glVertexAttribPointer(a_Position, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, 0);
	glVertexAttribPointer(a_Emit_and_Life_Time, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (GLvoid*)(sizeof(float) * 6));

	glDrawArrays(GL_TRIANGLES, 0, m_Count_of_Sin_Particle_Vertice);
	glDisableVertexAttribArray(a_Position);
	glDisableVertexAttribArray(a_Emit_and_Life_Time);
}




// ================================================================


// �� Proxy Geometry �����غ���. ��

// OpenGL Quick Reference Card -> ���̴� ������ ��������


// Shader Language ����ü Ÿ�� ��������

// mat4 m = mat4(2.0);
// => 2.0���� �� 0x0, 1x1, 2x2, ...�� ���ҵ��� �ʱ�ȭ�� 4x4 ���.
// 2.0f 0.0f 0.0f 0.0f
// 0.0f 2.0f 0.0f 0.0f
// 0.0f 0.0f 2.0f 0.0f
// 0.0f 0.0f 0.0f 2.0f

// => ��, ������ ��� vec3(n)�� ��ü ���Ҹ� n���� �ʱ�ȭ �Ѵ�.
// => vec3 white = vec3(1.0f);
// => white == {1.0f, 1.0f, 1.0f}

// vec4 zVec = m[2];
// => m[2]�� 3��. {0.0, 0.0f, 2.0f, 0.0f}.

// float yScale = m[1][1];
// => m[1][1]�� 2�� 2��. yScale = 2.0f.

// ** float�� �⺻���̰�, double�� �����ϰ� �ʹٸ� d�� ���λ�� ������.
// ==> vec2, vec3, mat3, mat4, .... �� float���� ���.
// ==> dvec2, dvec3, dmat3, dmat4, .... �� double���� ���.
// ** int���� ���������� 'i'��, unsigned int�� 'u'��, bool���� 'b'�� ���λ�� �Ѵ�. **