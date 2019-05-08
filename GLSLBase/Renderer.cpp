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
	m_SandBox_Shader = CompileShaders("./Shaders/SandBox.vs", "./Shaders/SandBox.fs");
	m_FillAll_Shader = CompileShaders("./Shaders/FillAll.vs", "./Shaders/FillAll.fs");
	m_Simple_Texture_Shader = CompileShaders("./Shaders/SimpleTexture.vs", "./Shaders/SimpleTexture.fs");

	// Load Textures
	m_Particle_Texture_1 = CreatePngTexture("./Resources/Textures/Test_Cat.png");
	m_Particle_Texture_2 = CreatePngTexture("./Resources/Textures/Test_Leaf.png");
	m_Sample_RGB_Texture = CreatePngTexture("./Resources/Textures/Sample_RGB.png");
	m_Steel_Floor_Texture = CreatePngTexture("./Resources/Textures/Steel_Floor_Texture.png");
	m_Wooden_Box_Texture = CreatePngTexture("./Resources/Textures/Wooden_Box_Texture.png");

	//Random Device Setting
	Random_Device_Setting();

	//Create VBO
	Create_Vertex_Buffer_Objects();
	Create_Proxy_Geometry();
	Create_Lec4_Particle_VBO(PARTICLE_NUMS);
	Create_Lec5_Particle_VBO(PARTICLE_NUMS);
	Create_Sin_Particle_VBO(PARTICLE_NUMS);
	Create_SandBox_VBO();
	Create_FillAll_VBO();
	Create_Simple_Texture_VBO();
}

void Renderer::Random_Device_Setting()
{
	mt19937_64 temp_seed(m_Random_Device());
	m_Random_Seed = temp_seed;

	uniform_real_distribution<> temp_random_pos(PARTICLE_RAND_POS_MIN, PARTICLE_RAND_POS_MAX);
	m_Random_Position = temp_random_pos;

	uniform_real_distribution<> temp_random_velo(PARTICLE_RAND_VELOCITY_MIN, PARTICLE_RAND_VELOCITY_MAX);
	m_Random_Veclocity = temp_random_velo;

	uniform_real_distribution<> temp_random_start_time(PARTICLE_RAND_START_TIME_MIN, PARTICLE_RAND_START_TIME_MAX);
	m_Random_Start_Time = temp_random_start_time;

	uniform_real_distribution<> temp_random_life_time(PARTICLE_RAND_LIFE_TIME_MIN, PARTICLE_RAND_LIFE_TIME_MAX);
	m_Random_Life_Time = temp_random_life_time;

	uniform_real_distribution<> temp_random_ratio(PARTICLE_RAND_RATIO_MIN, PARTICLE_RAND_RATIO_MAX);
	m_Random_Ratio = temp_random_ratio;

	uniform_real_distribution<> temp_random_amplitude(PARTICLE_RAND_AMPLITUDE_MIN, PARTICLE_RAND_AMPLITUDE_MAX);
	m_Random_Amplitude = temp_random_amplitude;

	uniform_real_distribution<> temp_random_value(PARTICLE_RAND_VALUE_MIN, PARTICLE_RAND_VALUE_MAX);
	m_Random_Value = temp_random_value;

	uniform_real_distribution<> temp_random_color(PARTICLE_RAND_COLOR_MIN, PARTICLE_RAND_COLOR_MAX);
	m_Random_Color = temp_random_color;
}

void Renderer::Create_Vertex_Buffer_Objects() // Renderer::Test()를 위한 VBO 생성기
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
	// GPU에서 데이터를 사용하기 위해 "Buffer Object"를 생성하고,
	// 생성된 VBO의 "Object ID"를 <두번째 인자>에 받아온다.
	// <첫번째 인자>는 생성할 버퍼의 개수를 의미한다.
	// <두번째 인자> 에는 버퍼의 개수만큼 각각의 ID가 들어간다.

	glBindBuffer(GL_ARRAY_BUFFER, m_VBORect);
	// 생성된 VBO의 ID를 사용하여 Bind 한다.
	// Bind 란? -> 실제 작업할 대상을 선정해 주는 것.
	
	// GL_ARRAY_BUFFER는 "작업대"의 한 종류라고 생각하면 된다.
	// 즉, 현재는 GL_ARRAY_BUFFER 라는 작업대에 VBO를 하나 올려둔 상태이다.
	
	// [[ VBO를 가지고 어떤 작업을 하기 전에는 반드시 그 VBO를 Bind 해야한다! ]]

	glBufferData(GL_ARRAY_BUFFER, sizeof(rect), rect, GL_STATIC_DRAW);
	// Bind된 VBO에 데이터를 할당한다. (아직 빈 버퍼이기 때문에..)
	// glBindBuffer 호출 후에 glBufferData를 바로 호출하는 것이 옳다.
	// 애멎은 VBO에 데이터 할당을 하게될 수도 있기 때문이다. (작업대에 다른 VBO가 올라가있다던가..)
	// 여기서 sizeof(rect)는 데이터들의 전체 사이즈를 의미.


	// 똑같이 color도 만들어준다.
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

void Renderer::Create_Proxy_Geometry()
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

void Renderer::Create_Lec4_Particle_VBO(const int& particle_Count)
{
	float temp_Pos_X, temp_Pos_Y;
	float temp_Velocity_X, temp_Velocity_Y, temp_Velocity_Z;

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
	float temp_Start_Time, temp_Life_Time; // Start_Time, Life_Time

	// 파티클 조각 개수 = particle_Count
	// 파티클 한조각의 정점 개수 = 6
	// 정점당 데이터 개수 = 8
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
		temp_Start_Time = m_Random_Start_Time(m_Random_Seed);
		temp_Life_Time = m_Random_Life_Time(m_Random_Seed);

		Particles_Vertice[i++] = temp_Pos_X - PARTICLE_HALF_SIZE; // Pos_X
		Particles_Vertice[i++] = temp_Pos_Y + PARTICLE_HALF_SIZE; // Pos_Y
		Particles_Vertice[i++] = 0.0f; // Pos_Z
		Particles_Vertice[i++] = temp_Velocity_X; // Velo_X
		Particles_Vertice[i++] = temp_Velocity_Y; // Velo_Y
		Particles_Vertice[i++] = temp_Velocity_Z; // Velo_Z
		Particles_Vertice[i++] = temp_Start_Time; // Emit_Time
		Particles_Vertice[i++] = temp_Life_Time; // Life_Time

		Particles_Vertice[i++] = temp_Pos_X - PARTICLE_HALF_SIZE;
		Particles_Vertice[i++] = temp_Pos_Y - PARTICLE_HALF_SIZE;
		Particles_Vertice[i++] = 0.0f;
		Particles_Vertice[i++] = temp_Velocity_X;
		Particles_Vertice[i++] = temp_Velocity_Y;
		Particles_Vertice[i++] = temp_Velocity_Z;
		Particles_Vertice[i++] = temp_Start_Time;
		Particles_Vertice[i++] = temp_Life_Time;

		Particles_Vertice[i++] = temp_Pos_X + PARTICLE_HALF_SIZE;
		Particles_Vertice[i++] = temp_Pos_Y + PARTICLE_HALF_SIZE;
		Particles_Vertice[i++] = 0.0f;
		Particles_Vertice[i++] = temp_Velocity_X;
		Particles_Vertice[i++] = temp_Velocity_Y;
		Particles_Vertice[i++] = temp_Velocity_Z;
		Particles_Vertice[i++] = temp_Start_Time;
		Particles_Vertice[i++] = temp_Life_Time;

		Particles_Vertice[i++] = temp_Pos_X + PARTICLE_HALF_SIZE;
		Particles_Vertice[i++] = temp_Pos_Y + PARTICLE_HALF_SIZE;
		Particles_Vertice[i++] = 0.0f;
		Particles_Vertice[i++] = temp_Velocity_X;
		Particles_Vertice[i++] = temp_Velocity_Y;
		Particles_Vertice[i++] = temp_Velocity_Z;
		Particles_Vertice[i++] = temp_Start_Time;
		Particles_Vertice[i++] = temp_Life_Time;

		Particles_Vertice[i++] = temp_Pos_X - PARTICLE_HALF_SIZE;
		Particles_Vertice[i++] = temp_Pos_Y - PARTICLE_HALF_SIZE;
		Particles_Vertice[i++] = 0.0f;
		Particles_Vertice[i++] = temp_Velocity_X;
		Particles_Vertice[i++] = temp_Velocity_Y;
		Particles_Vertice[i++] = temp_Velocity_Z;
		Particles_Vertice[i++] = temp_Start_Time;
		Particles_Vertice[i++] = temp_Life_Time;

		Particles_Vertice[i++] = temp_Pos_X + PARTICLE_HALF_SIZE;
		Particles_Vertice[i++] = temp_Pos_Y - PARTICLE_HALF_SIZE;
		Particles_Vertice[i++] = 0.0f;
		Particles_Vertice[i++] = temp_Velocity_X;
		Particles_Vertice[i++] = temp_Velocity_Y;
		Particles_Vertice[i++] = temp_Velocity_Z;
		Particles_Vertice[i++] = temp_Start_Time;
		Particles_Vertice[i] = temp_Life_Time;
	}

	glGenBuffers(1, &m_VBO_Particle);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO_Particle);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * array_Length, Particles_Vertice, GL_STATIC_DRAW);

	delete[] Particles_Vertice;
}

void Renderer::Create_Sin_Particle_VBO(const int& particle_Count)
{
	float temp_Pos_X, temp_Pos_Y; // 초기 위치
	float temp_Velocity_X, temp_Velocity_Y, temp_Velocity_Z; // 속도
	float temp_Start_Time, temp_Life_Time; // 시작 시간, 생명 주기
	float temp_Amplitude, temp_Ratio; // 진폭, 진동 주기
	float temp_Value; // 고유 값
	float temp_Color_R, temp_Color_G, temp_Color_B, temp_Color_A; // 색상

	// 파티클 조각 개수 = particle_Count
	// 파티클 한조각의 정점 개수 = 6
	// 정점당 데이터 개수 = 11
	m_Count_of_Sin_Particle_Vertice = particle_Count * 6;
	int array_Length = m_Count_of_Sin_Particle_Vertice * 15;
	float* Particles_Vertice = new float[array_Length];

	for (int i = 0; i < array_Length; ++i)
	{
		temp_Velocity_X = m_Random_Veclocity(m_Random_Seed);
		temp_Velocity_Y = m_Random_Veclocity(m_Random_Seed);
		temp_Velocity_Z = m_Random_Veclocity(m_Random_Seed);
		temp_Start_Time = m_Random_Start_Time(m_Random_Seed);
		temp_Life_Time = m_Random_Life_Time(m_Random_Seed);
		temp_Ratio = m_Random_Ratio(m_Random_Seed);
		temp_Amplitude = m_Random_Amplitude(m_Random_Seed);
		temp_Value = m_Random_Value(m_Random_Seed);
		temp_Color_R = m_Random_Color(m_Random_Seed);
		temp_Color_G = m_Random_Color(m_Random_Seed);
		temp_Color_B = m_Random_Color(m_Random_Seed);
		temp_Color_A = m_Random_Color(m_Random_Seed);


		Particles_Vertice[i++] = -PARTICLE_HALF_SIZE; // Pos_X
		Particles_Vertice[i++] = PARTICLE_HALF_SIZE; // Pos_Y
		Particles_Vertice[i++] = 0.0f; // Pos_Z
		Particles_Vertice[i++] = temp_Velocity_X; // Velocity_X
		Particles_Vertice[i++] = temp_Velocity_Y; // Velocity_Y
		Particles_Vertice[i++] = temp_Velocity_Z; // Velocity_Z
		Particles_Vertice[i++] = temp_Start_Time; // Emit_Time
		Particles_Vertice[i++] = temp_Life_Time; // Life_Time
		Particles_Vertice[i++] = temp_Ratio; // Ratio
		Particles_Vertice[i++] = temp_Amplitude; // Amplitude
		Particles_Vertice[i++] = temp_Value; // Value
		Particles_Vertice[i++] = temp_Color_R; // Color R
		Particles_Vertice[i++] = temp_Color_G; // Color G
		Particles_Vertice[i++] = temp_Color_B; // Color B
		Particles_Vertice[i++] = temp_Color_A; // Color A

		Particles_Vertice[i++] = -PARTICLE_HALF_SIZE;
		Particles_Vertice[i++] = -PARTICLE_HALF_SIZE;
		Particles_Vertice[i++] = 0.0f;
		Particles_Vertice[i++] = temp_Velocity_X;
		Particles_Vertice[i++] = temp_Velocity_Y;
		Particles_Vertice[i++] = temp_Velocity_Z;
		Particles_Vertice[i++] = temp_Start_Time;
		Particles_Vertice[i++] = temp_Life_Time;
		Particles_Vertice[i++] = temp_Ratio;
		Particles_Vertice[i++] = temp_Amplitude;
		Particles_Vertice[i++] = temp_Value;
		Particles_Vertice[i++] = temp_Color_R;
		Particles_Vertice[i++] = temp_Color_G;
		Particles_Vertice[i++] = temp_Color_B;
		Particles_Vertice[i++] = temp_Color_A;

		Particles_Vertice[i++] = PARTICLE_HALF_SIZE;
		Particles_Vertice[i++] = PARTICLE_HALF_SIZE;
		Particles_Vertice[i++] = 0.0f;
		Particles_Vertice[i++] = temp_Velocity_X;
		Particles_Vertice[i++] = temp_Velocity_Y;
		Particles_Vertice[i++] = temp_Velocity_Z;
		Particles_Vertice[i++] = temp_Start_Time;
		Particles_Vertice[i++] = temp_Life_Time;
		Particles_Vertice[i++] = temp_Ratio;
		Particles_Vertice[i++] = temp_Amplitude;
		Particles_Vertice[i++] = temp_Value;
		Particles_Vertice[i++] = temp_Color_R;
		Particles_Vertice[i++] = temp_Color_G;
		Particles_Vertice[i++] = temp_Color_B;
		Particles_Vertice[i++] = temp_Color_A;

		Particles_Vertice[i++] = PARTICLE_HALF_SIZE;
		Particles_Vertice[i++] = -PARTICLE_HALF_SIZE;
		Particles_Vertice[i++] = 0.0f;
		Particles_Vertice[i++] = temp_Velocity_X;
		Particles_Vertice[i++] = temp_Velocity_Y;
		Particles_Vertice[i++] = temp_Velocity_Z;
		Particles_Vertice[i++] = temp_Start_Time;
		Particles_Vertice[i++] = temp_Life_Time;
		Particles_Vertice[i++] = temp_Ratio;
		Particles_Vertice[i++] = temp_Amplitude;
		Particles_Vertice[i++] = temp_Value;
		Particles_Vertice[i++] = temp_Color_R;
		Particles_Vertice[i++] = temp_Color_G;
		Particles_Vertice[i++] = temp_Color_B;
		Particles_Vertice[i++] = temp_Color_A;

		Particles_Vertice[i++] = PARTICLE_HALF_SIZE;
		Particles_Vertice[i++] = PARTICLE_HALF_SIZE;
		Particles_Vertice[i++] = 0.0f;
		Particles_Vertice[i++] = temp_Velocity_X;
		Particles_Vertice[i++] = temp_Velocity_Y;
		Particles_Vertice[i++] = temp_Velocity_Z;
		Particles_Vertice[i++] = temp_Start_Time;
		Particles_Vertice[i++] = temp_Life_Time;
		Particles_Vertice[i++] = temp_Ratio;
		Particles_Vertice[i++] = temp_Amplitude;
		Particles_Vertice[i++] = temp_Value;
		Particles_Vertice[i++] = temp_Color_R;
		Particles_Vertice[i++] = temp_Color_G;
		Particles_Vertice[i++] = temp_Color_B;
		Particles_Vertice[i++] = temp_Color_A;

		Particles_Vertice[i++] = -PARTICLE_HALF_SIZE;
		Particles_Vertice[i++] = -PARTICLE_HALF_SIZE;
		Particles_Vertice[i++] = 0.0f;
		Particles_Vertice[i++] = temp_Velocity_X;
		Particles_Vertice[i++] = temp_Velocity_Y;
		Particles_Vertice[i++] = temp_Velocity_Z;
		Particles_Vertice[i++] = temp_Start_Time;
		Particles_Vertice[i++] = temp_Life_Time;
		Particles_Vertice[i++] = temp_Ratio;
		Particles_Vertice[i++] = temp_Amplitude;
		Particles_Vertice[i++] = temp_Value;
		Particles_Vertice[i++] = temp_Color_R;
		Particles_Vertice[i++] = temp_Color_G;
		Particles_Vertice[i++] = temp_Color_B;
		Particles_Vertice[i] = temp_Color_A;
	}

	glGenBuffers(1, &m_VBO_Sin_Particle);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO_Sin_Particle);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * array_Length, Particles_Vertice, GL_STATIC_DRAW);

	delete[] Particles_Vertice;
}

void Renderer::Create_SandBox_VBO()
{
	float temp_Pos_X, temp_Pos_Y; // 초기 위치
	float temp_Color_R = 1.0f, temp_Color_G = 1.0f, temp_Color_B = 1.0f, temp_Color_A = 1.0f; // 색상

	// 파티클 조각 개수 = particle_Count
	// 파티클 한조각의 정점 개수 = 6
	// 정점당 데이터 개수 = 9
	int array_Length = 6 * 9;
	float* Particles_Vertice = new float[array_Length];

	for (int i = 0; i < array_Length; ++i)
	{
		Particles_Vertice[i++] = -SANDBOX_HALF_SIZE; // Pos_X
		Particles_Vertice[i++] = SANDBOX_HALF_SIZE; // Pos_Y
		Particles_Vertice[i++] = 0.0f; // Pos_Z
		Particles_Vertice[i++] = temp_Color_R; // Color R
		Particles_Vertice[i++] = temp_Color_G; // Color G
		Particles_Vertice[i++] = temp_Color_B; // Color B
		Particles_Vertice[i++] = temp_Color_A; // Color A
		Particles_Vertice[i++] = 0.0f;
		Particles_Vertice[i++] = 1.0f;

		Particles_Vertice[i++] = -SANDBOX_HALF_SIZE;
		Particles_Vertice[i++] = -SANDBOX_HALF_SIZE;
		Particles_Vertice[i++] = 0.0f;
		Particles_Vertice[i++] = temp_Color_R;
		Particles_Vertice[i++] = temp_Color_G;
		Particles_Vertice[i++] = temp_Color_B;
		Particles_Vertice[i++] = temp_Color_A;
		Particles_Vertice[i++] = 0.0f;
		Particles_Vertice[i++] = 0.0f;

		Particles_Vertice[i++] = SANDBOX_HALF_SIZE;
		Particles_Vertice[i++] = SANDBOX_HALF_SIZE;
		Particles_Vertice[i++] = 0.0f;
		Particles_Vertice[i++] = temp_Color_R;
		Particles_Vertice[i++] = temp_Color_G;
		Particles_Vertice[i++] = temp_Color_B;
		Particles_Vertice[i++] = temp_Color_A;
		Particles_Vertice[i++] = 1.0f;
		Particles_Vertice[i++] = 1.0f;

		Particles_Vertice[i++] = SANDBOX_HALF_SIZE;
		Particles_Vertice[i++] = -SANDBOX_HALF_SIZE;
		Particles_Vertice[i++] = 0.0f;
		Particles_Vertice[i++] = temp_Color_R;
		Particles_Vertice[i++] = temp_Color_G;
		Particles_Vertice[i++] = temp_Color_B;
		Particles_Vertice[i++] = temp_Color_A;
		Particles_Vertice[i++] = 1.0f;
		Particles_Vertice[i++] = 0.0f;

		Particles_Vertice[i++] = SANDBOX_HALF_SIZE;
		Particles_Vertice[i++] = SANDBOX_HALF_SIZE;
		Particles_Vertice[i++] = 0.0f;
		Particles_Vertice[i++] = temp_Color_R;
		Particles_Vertice[i++] = temp_Color_G;
		Particles_Vertice[i++] = temp_Color_B;
		Particles_Vertice[i++] = temp_Color_A;
		Particles_Vertice[i++] = 1.0f;
		Particles_Vertice[i++] = 1.0f;

		Particles_Vertice[i++] = -SANDBOX_HALF_SIZE;
		Particles_Vertice[i++] = -SANDBOX_HALF_SIZE;
		Particles_Vertice[i++] = 0.0f;
		Particles_Vertice[i++] = temp_Color_R;
		Particles_Vertice[i++] = temp_Color_G;
		Particles_Vertice[i++] = temp_Color_B;
		Particles_Vertice[i++] = temp_Color_A;
		Particles_Vertice[i++] = 0.0f;
		Particles_Vertice[i] = 0.0f;
	}

	glGenBuffers(1, &m_VBO_SandBox);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO_SandBox);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * array_Length, Particles_Vertice, GL_STATIC_DRAW);

	delete[] Particles_Vertice;
}

void Renderer::Create_FillAll_VBO()
{
	float temp_Pos_X, temp_Pos_Y; // 초기 위치

	// 파티클 조각 개수 = particle_Count
	// 파티클 한조각의 정점 개수 = 6
	// 정점당 데이터 개수 = 3
	int array_Length = 6 * 3;
	float* Particles_Vertice = new float[array_Length];

	for (int i = 0; i < array_Length; ++i)
	{
		Particles_Vertice[i++] = -FILLALL_RECT_HALF_SIZE; // Pos_X
		Particles_Vertice[i++] = FILLALL_RECT_HALF_SIZE; // Pos_Y
		Particles_Vertice[i++] = 0.0f; // Pos_Z

		Particles_Vertice[i++] = -FILLALL_RECT_HALF_SIZE;
		Particles_Vertice[i++] = -FILLALL_RECT_HALF_SIZE;
		Particles_Vertice[i++] = 0.0f;

		Particles_Vertice[i++] = FILLALL_RECT_HALF_SIZE;
		Particles_Vertice[i++] = FILLALL_RECT_HALF_SIZE;
		Particles_Vertice[i++] = 0.0f;

		Particles_Vertice[i++] = FILLALL_RECT_HALF_SIZE;
		Particles_Vertice[i++] = -FILLALL_RECT_HALF_SIZE;
		Particles_Vertice[i++] = 0.0f;

		Particles_Vertice[i++] = FILLALL_RECT_HALF_SIZE;
		Particles_Vertice[i++] = FILLALL_RECT_HALF_SIZE;
		Particles_Vertice[i++] = 0.0f;

		Particles_Vertice[i++] = -FILLALL_RECT_HALF_SIZE;
		Particles_Vertice[i++] = -FILLALL_RECT_HALF_SIZE;
		Particles_Vertice[i] = 0.0f;
	}

	glGenBuffers(1, &m_VBO_FillAll);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO_FillAll);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * array_Length, Particles_Vertice, GL_STATIC_DRAW);

	delete[] Particles_Vertice;
}

void Renderer::Create_Simple_Texture_VBO()
{
	//float temp_Pos_X, temp_Pos_Y; // 초기 위치

	// 파티클 조각 개수 = particle_Count
	// 파티클 한조각의 정점 개수 = 6
	// 정점당 데이터 개수 = 5
	int array_Length = 6 * 5;
	float* Particles_Vertice = new float[array_Length];

	for (int i = 0; i < array_Length; ++i)
	{
		Particles_Vertice[i++] = -SIMPLE_TEXTURE_HALF_SIZE; // Pos_X
		Particles_Vertice[i++] = SIMPLE_TEXTURE_HALF_SIZE; // Pos_Y
		Particles_Vertice[i++] = 0.0f; // Pos_Z
		Particles_Vertice[i++] = 0.0f; // UV_Pos_U
		Particles_Vertice[i++] = 1.0f; // UV_Pos_V

		Particles_Vertice[i++] = -SIMPLE_TEXTURE_HALF_SIZE;
		Particles_Vertice[i++] = -SIMPLE_TEXTURE_HALF_SIZE;
		Particles_Vertice[i++] = 0.0f;
		Particles_Vertice[i++] = 0.0f;
		Particles_Vertice[i++] = 0.0f;

		Particles_Vertice[i++] = SIMPLE_TEXTURE_HALF_SIZE;
		Particles_Vertice[i++] = SIMPLE_TEXTURE_HALF_SIZE;
		Particles_Vertice[i++] = 0.0f;
		Particles_Vertice[i++] = 1.0f;
		Particles_Vertice[i++] = 1.0f;

		Particles_Vertice[i++] = SIMPLE_TEXTURE_HALF_SIZE;
		Particles_Vertice[i++] = -SIMPLE_TEXTURE_HALF_SIZE;
		Particles_Vertice[i++] = 0.0f;
		Particles_Vertice[i++] = 1.0f;
		Particles_Vertice[i++] = 0.0f;

		Particles_Vertice[i++] = SIMPLE_TEXTURE_HALF_SIZE;
		Particles_Vertice[i++] = SIMPLE_TEXTURE_HALF_SIZE;
		Particles_Vertice[i++] = 0.0f;
		Particles_Vertice[i++] = 1.0f;
		Particles_Vertice[i++] = 1.0f;

		Particles_Vertice[i++] = -SIMPLE_TEXTURE_HALF_SIZE;
		Particles_Vertice[i++] = -SIMPLE_TEXTURE_HALF_SIZE;
		Particles_Vertice[i++] = 0.0f;
		Particles_Vertice[i++] = 0.0f;
		Particles_Vertice[i] = 0.0f;
	}

	glGenBuffers(1, &m_VBO_Simple_Texture);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO_Simple_Texture);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * array_Length, Particles_Vertice, GL_STATIC_DRAW);

	delete[] Particles_Vertice;

	// 체크보드 텍스쳐 생성
	glGenTextures(1, &m_Check_Texture_ID);
	glBindTexture(GL_TEXTURE_2D, m_Check_Texture_ID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 8, 8, 0, GL_RGBA, GL_UNSIGNED_BYTE, s_Checker_Board);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	// glTexImage2D()는 VRAM에 "2D 텍스쳐 이미지"를 올려주는 함수이다. 
	// GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE => UV 좌표의 U좌표를 넘어가는 끝부분을 마지막 픽셀의 색상으로 메꿔준다.
	// GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE => UV 좌표의 V좌표를 넘어가는 끝부분을 마지막 픽셀의 색상으로 메꿔준다.
}

//=================================================================


void Renderer::AddShader(GLuint ShaderProgram, const char* pShaderText, GLenum ShaderType)
{
	//쉐이더 오브젝트 생성
	GLuint ShaderObj = glCreateShader(ShaderType);

	if (ShaderObj == 0) {
		fprintf(stderr, "Error creating shader type %d\n", ShaderType);
	}

	const GLchar* p[1];
	p[0] = pShaderText;
	GLint Lengths[1];
	Lengths[0] = (GLint)strlen(pShaderText);
	//쉐이더 코드를 쉐이더 오브젝트에 할당
	glShaderSource(ShaderObj, 1, p, Lengths);

	//할당된 쉐이더 코드를 컴파일
	glCompileShader(ShaderObj);

	GLint success;
	// ShaderObj 가 성공적으로 컴파일 되었는지 확인
	glGetShaderiv(ShaderObj, GL_COMPILE_STATUS, &success);
	if (!success) {
		GLchar InfoLog[1024];

		//OpenGL 의 shader log 데이터를 가져옴
		glGetShaderInfoLog(ShaderObj, 1024, NULL, InfoLog);
		fprintf(stderr, "Error compiling shader type %d: '%s'\n", ShaderType, InfoLog);
		printf("%s \n", pShaderText);
	}

	// ShaderProgram 에 attach!!
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
	GLuint ShaderProgram = glCreateProgram(); //빈 쉐이더 프로그램 생성

	if (ShaderProgram == 0) { //쉐이더 프로그램이 만들어졌는지 확인
		fprintf(stderr, "Error creating shader program\n");
	}

	std::string vs, fs;

	//shader.vs 가 vs 안으로 로딩됨
	if (!ReadFile(filenameVS, &vs)) {
		printf("Error compiling vertex shader\n");
		return -1;
	};

	//shader.fs 가 fs 안으로 로딩됨
	if (!ReadFile(filenameFS, &fs)) {
		printf("Error compiling fragment shader\n");
		return -1;
	};

	// ShaderProgram 에 vs.c_str() 버텍스 쉐이더를 컴파일한 결과를 attach함
	AddShader(ShaderProgram, vs.c_str(), GL_VERTEX_SHADER);

	// ShaderProgram 에 fs.c_str() 프레그먼트 쉐이더를 컴파일한 결과를 attach함
	AddShader(ShaderProgram, fs.c_str(), GL_FRAGMENT_SHADER);

	GLint Success = 0;
	GLchar ErrorLog[1024] = { 0 };

	//Attach 완료된 shaderProgram 을 링킹함
	glLinkProgram(ShaderProgram);

	//링크가 성공했는지 확인
	glGetProgramiv(ShaderProgram, GL_LINK_STATUS, &Success);

	if (Success == 0) {
		// shader program 로그를 받아옴
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
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, bmp);

	return temp;
}


//=================================================================


void Renderer::Test()
{
	GLuint shader_ID = m_SolidRectShader;

	glUseProgram(shader_ID);

	if (m_Scale > 1.0f) { m_Scale = 0.0f; }
	m_Scale += 0.01f;

	GLuint u_Time = glGetUniformLocation(shader_ID, "u_Time");
	glUniform1f(u_Time, m_Scale); // 해당 Uniform 변수의 값을 변경한다.

	GLuint a_Position = glGetAttribLocation(shader_ID, "a_Position");
	// 해당 Shader의 "a_Position" 이라는 Attribute의 번호를 받아온다.

	glEnableVertexAttribArray(a_Position);
	// 받아온 번호의 Attribute를 사용하기 위해 Enable 시키는 것.

	glBindBuffer(GL_ARRAY_BUFFER, m_VBORect);
	// m_VBORect가 가진 ID를 불러와서 "작업대"에 올린다.

	glVertexAttribPointer(a_Position, 4, GL_FLOAT, GL_FALSE, sizeof(float) * 4, 0);
	// Draw 시 데이터를 읽어갈 단위의 크기 및 시작점을 설정한다. 인자 => (Attribute Number, ???, 읽을 자료형, ???, 건너 뛸 offset 크기, 시작점 index)
	// (여기서는 Float Pointer를 사용하고 있다.)
	// 현재 정점 정보는 3개의 위치값만으로 이루어져있기 때문에 sizeof(float) * 3을 한다.
	// (3을 곱한 이유는 데이터가 3개씩으로 이루어져있기 때문에, 정점 마다 uv값이 추가된다면 5를 곱해야한다.)

	GLuint a_Color = glGetAttribLocation(shader_ID, "a_Color");
	glEnableVertexAttribArray(a_Color);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBORectColor);
	glVertexAttribPointer(a_Color, 4, GL_FLOAT, GL_FALSE, sizeof(float) * 4, 0);

	glDrawArrays(GL_TRIANGLES, 0, 6);
	// 시작점과 데이터 단위의 크기를 정했으니, 배열을 그리라고 호출한다.
	// (Primitive를 결정.)
	// "0번 정점" 부터 "6개"를..


	glDisableVertexAttribArray(a_Position);
	// 작업을 완료했으니 해당 Attribute를 Disable 시킨다.
	glDisableVertexAttribArray(a_Color);
}

void Renderer::Draw_Lec4_Particle()
{
	GLuint shader_ID = m_SimpleParticleShader;

	glUseProgram(shader_ID);

	GLuint u_Time = glGetUniformLocation(shader_ID, "u_Time");
	glUniform1f(u_Time, m_Time); // 해당 Uniform 변수의 값을 변경한다.
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

void Renderer::Draw_Proxy_Geometry()
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
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// ===============================================

	GLuint shader_ID = m_Sin_Particle_Shader;
	glUseProgram(shader_ID);

	// ===============================================

	GLuint u_Time = glGetUniformLocation(shader_ID, "u_Time");
	glUniform1f(u_Time, m_Time);
	m_Time += 0.01f;
	
	GLuint u_Texture = glGetUniformLocation(shader_ID, "u_Texture");
	glUniform1i(u_Texture, 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_Particle_Texture_2);


	// ===============================================

	GLuint a_Position = glGetAttribLocation(shader_ID, "a_Position");
	GLuint a_Velocity = glGetAttribLocation(shader_ID, "a_Velocity");
	GLuint a_Start_and_Life_Time = glGetAttribLocation(shader_ID, "a_Start_and_Life_Time");
	GLuint a_Ratio_and_Amplitude = glGetAttribLocation(shader_ID, "a_Ratio_and_Amplitude");
	GLuint a_Value = glGetAttribLocation(shader_ID, "a_Value");
	GLuint a_Color = glGetAttribLocation(shader_ID, "a_Color");

	glEnableVertexAttribArray(a_Position);
	glEnableVertexAttribArray(a_Velocity);
	glEnableVertexAttribArray(a_Start_and_Life_Time);
	glEnableVertexAttribArray(a_Ratio_and_Amplitude);
	glEnableVertexAttribArray(a_Value);
	glEnableVertexAttribArray(a_Color);

	// ===============================================

	glBindBuffer(GL_ARRAY_BUFFER, m_VBO_Sin_Particle);
	glVertexAttribPointer(a_Position, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 15, 0);
	glVertexAttribPointer(a_Velocity, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 15, (GLvoid*)(sizeof(float) * 3));
	glVertexAttribPointer(a_Start_and_Life_Time, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 15, (GLvoid*)(sizeof(float) * 6));
	glVertexAttribPointer(a_Ratio_and_Amplitude, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 15, (GLvoid*)(sizeof(float) * 8));
	glVertexAttribPointer(a_Value, 1, GL_FLOAT, GL_FALSE, sizeof(float) * 15, (GLvoid*)(sizeof(float) * 10));
	glVertexAttribPointer(a_Color, 4, GL_FLOAT, GL_FALSE, sizeof(float) * 15, (GLvoid*)(sizeof(float) * 11));

	glDrawArrays(GL_TRIANGLES, 0, m_Count_of_Sin_Particle_Vertice);

	// ===============================================

	glDisableVertexAttribArray(a_Position);
	glDisableVertexAttribArray(a_Velocity);
	glDisableVertexAttribArray(a_Start_and_Life_Time);
	glDisableVertexAttribArray(a_Ratio_and_Amplitude);
	glDisableVertexAttribArray(a_Value);
	glDisableVertexAttribArray(a_Color);
}

void Renderer::Draw_SandBox()
{
	// prepare points
	GLfloat points[] = { 0.0f, 0.0f, 0.5f, 0.5f, 0.3f, 0.3f, -0.2f, -0.2f, -0.3f, -0.3f };

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// ===============================================

	GLuint shader_ID = m_SandBox_Shader;
	glUseProgram(shader_ID);

	// ===============================================

	GLuint u_Time = glGetUniformLocation(shader_ID, "u_Time");
	glUniform1f(u_Time, m_Time);
	m_Time += 0.01f;

	GLuint u_Points = glGetUniformLocation(shader_ID, "u_Points");
	glUniform2fv(u_Points, 5, points);

	// ===============================================

	GLuint a_Position = glGetAttribLocation(shader_ID, "a_Position");
	GLuint a_Color = glGetAttribLocation(shader_ID, "a_Color");
	GLuint a_UV = glGetAttribLocation(shader_ID, "a_UV");

	glEnableVertexAttribArray(a_Position);
	glEnableVertexAttribArray(a_Color);
	glEnableVertexAttribArray(a_UV);

	// ===============================================

	glBindBuffer(GL_ARRAY_BUFFER, m_VBO_SandBox);
	glVertexAttribPointer(a_Position, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 9, 0);
	glVertexAttribPointer(a_Color, 4, GL_FLOAT, GL_FALSE, sizeof(float) * 9, (GLvoid*)(sizeof(float) * 3));
	glVertexAttribPointer(a_UV, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 9, (GLvoid*)(sizeof(float) * 7));

	glDrawArrays(GL_TRIANGLES, 0, 6);

	// ===============================================

	glDisableVertexAttribArray(a_Position);
	glDisableVertexAttribArray(a_Color);
	glDisableVertexAttribArray(a_UV);
}

void Renderer::Fill_All(const float& alpha)
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// ===============================================

	GLuint shader_ID = m_FillAll_Shader;
	glUseProgram(shader_ID);

	// ===============================================

	GLuint a_Position = glGetAttribLocation(shader_ID, "a_Position");

	glEnableVertexAttribArray(a_Position);

	// ===============================================

	glBindBuffer(GL_ARRAY_BUFFER, m_VBO_FillAll);
	glVertexAttribPointer(a_Position, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, 0);
	glDrawArrays(GL_TRIANGLES, 0, 6);

	// ===============================================

	glDisableVertexAttribArray(a_Position);
}

void Renderer::Draw_Simple_Texture(const GLuint& tex)
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// ===============================================

	GLuint shader_ID = m_Simple_Texture_Shader;
	glUseProgram(shader_ID);

	// ===============================================

	GLuint u_Time = glGetUniformLocation(shader_ID, "u_Time");
	glUniform1f(u_Time, m_Time);
	m_Time += 0.01f;

	//int time_sec = floorf(m_Time);
	GLuint u_Texture = glGetUniformLocation(shader_ID, "u_Texture");
	glUniform1i(u_Texture, 1);
	GLuint u_Texture_2 = glGetUniformLocation(shader_ID, "u_Texture_2");
	glUniform1i(u_Texture_2, 2);
	GLuint u_Texture_3 = glGetUniformLocation(shader_ID, "u_Texture_3");
	glUniform1i(u_Texture_3, 3);
	GLuint u_Texture_4 = glGetUniformLocation(shader_ID, "u_Texture_4");
	glUniform1i(u_Texture_4, 4);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_Sample_RGB_Texture);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, m_Particle_Texture_1);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, m_Particle_Texture_2);
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, m_Steel_Floor_Texture);
	glActiveTexture(GL_TEXTURE4);
	glBindTexture(GL_TEXTURE_2D, m_Wooden_Box_Texture);

	// GPU는 하나의 Shader에서 텍스쳐를 한번에 80개 정도 사용할 수 있다.
	// glActiveTexture()로 해당 텍스쳐 슬롯을 활성화 해야 사용 할 수 있다.
	// 그리고 glBindTexture()를 하면, 직전에 활성화한 텍스쳐 슬롯에 가진 텍스쳐를 바인드 할 수 있다.

	// ===============================================

	GLuint a_Position = glGetAttribLocation(shader_ID, "a_Position");
	GLuint a_Texture_UV = glGetAttribLocation(shader_ID, "a_Texture_UV");

	glEnableVertexAttribArray(a_Position);
	glEnableVertexAttribArray(a_Texture_UV);

	// ===============================================

	glBindBuffer(GL_ARRAY_BUFFER, m_VBO_Simple_Texture);
	glVertexAttribPointer(a_Position, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 5, 0);
	glVertexAttribPointer(a_Texture_UV, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 5, (GLvoid*)(sizeof(float) * 3));

	glDrawArrays(GL_TRIANGLES, 0, 6);

	// ===============================================

	glDisableVertexAttribArray(a_Position);
	glDisableVertexAttribArray(a_Texture_UV);
}



// ================================================================


// ★ Proxy Geometry 구현해볼것. ★

// OpenGL Quick Reference Card -> 셰이더 랭귀지 참조문서


// Shader Language 집합체 타입 연습문제

// mat4 m = mat4(2.0);
// => 2.0으로 각 0x0, 1x1, 2x2, ...의 원소들이 초기화된 4x4 행렬.
// 2.0f 0.0f 0.0f 0.0f
// 0.0f 2.0f 0.0f 0.0f
// 0.0f 0.0f 2.0f 0.0f
// 0.0f 0.0f 0.0f 2.0f

// => 단, 벡터의 경우 vec3(n)은 전체 원소를 n으로 초기화 한다.
// => vec3 white = vec3(1.0f);
// => white == {1.0f, 1.0f, 1.0f}

// vec4 zVec = m[2];
// => m[2]는 3열. {0.0, 0.0f, 2.0f, 0.0f}.

// float yScale = m[1][1];
// => m[1][1]은 2행 2열. yScale = 2.0f.

// ** float이 기본형이고, double로 구성하고 싶다면 d를 접두사로 붙힌다.
// ==> vec2, vec3, mat3, mat4, .... 는 float형을 사용.
// ==> dvec2, dvec3, dmat3, dmat4, .... 는 double형을 사용.
// ** int형은 마찬가지로 'i'를, unsigned int는 'u'를, bool형은 'b'를 접두사로 한다. **