#include "stdafx.h"
#include "Renderer.h"

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
	m_FS_SandBox_Shader = CompileShaders("./Shaders/FSSandBox.vs", "./Shaders/FSSandBox.fs");
	m_FillAll_Shader = CompileShaders("./Shaders/FillAll.vs", "./Shaders/FillAll.fs");
	m_Simple_Texture_Shader = CompileShaders("./Shaders/SimpleTexture.vs", "./Shaders/SimpleTexture.fs");
	m_VS_SandBox_Shader = CompileShaders("./Shaders/VSSandBox.vs", "./Shaders/VSSandBox.fs");
	m_Simple_Cube_Shader = CompileShaders("./Shaders/SimpleCube.vs", "./Shaders/SimpleCube.fs");
	m_Texture_Rect_Shader = CompileShaders("./Shaders/TextureRect.vs", "./Shaders/TextureRect.fs");
	m_HDR_Texture_Rect_Shader = CompileShaders("./Shaders/HDRTextureRect.vs", "./Shaders/HDRTextureRect.fs");

	// Load Textures
	m_Particle_Texture_1 = CreatePngTexture("./Resources/Textures/Test_Cat.png");
	m_Particle_Texture_2 = CreatePngTexture("./Resources/Textures/Test_Leaf.png");
	m_Sample_RGB_Texture = CreatePngTexture("./Resources/Textures/Sample_RGB.png");
	m_Full_Moon_Texture = CreatePngTexture("./Resources/Textures/Full_Moon_Texture.png");
	m_Wooden_Box_Texture = CreatePngTexture("./Resources/Textures/Wooden_Box_Texture.png");
	m_Number_Texture = CreatePngTexture("./Resources/Textures/Number_Texture.png");
	m_Number_Texture_2 = CreatePngTexture("./Resources/Textures/Number_Texture_2.png");
	m_Particle_Texture_3 = CreatePngTexture("./Resources/Textures/Test_Cat_2.png");

	// Load Height Map Textures
	m_Height_Map_Texture = CreatePngTexture("./Resources/Textures/Height_Map.png");
	m_Snow_Texture = CreatePngTexture("./Resources/Textures/Snow_Texture.png");
	m_Grass_Texture = CreatePngTexture("./Resources/Textures/Grass_Texture.png");
	m_Height_Map_Texture_2 = CreatePngTexture("./Resources/Textures/Height_Map_2.png");

	// Load Sprites
	m_Sans_Sprite = CreatePngTexture("./Resources/Sprites/Sans_Sprite.png");
	m_Runner_Sprite = CreatePngTexture("./Resources/Sprites/Runner_Sprite.png");
	m_Gunner_Sprite = CreatePngTexture("./Resources/Sprites/Gunner_Sprite.png");
	m_Runner_Sprite_2 = CreatePngTexture("./Resources/Sprites/Runner_Sprite_2.png");
	m_Flame_Sprite = CreatePngTexture("./Resources/Sprites/Flame_Sprite.png");

	// Create Checkboard Texture
	glGenTextures(1, &m_Check_Texture_ID);
	glBindTexture(GL_TEXTURE_2D, m_Check_Texture_ID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 8, 8, 0, GL_RGBA, GL_UNSIGNED_BYTE, s_Checker_Board);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	// glTexImage2D()�� VRAM�� "2D �ؽ��� �̹���"�� �÷��ִ� �Լ��̴�. 
	// GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE => UV ��ǥ�� U��ǥ�� �Ѿ�� ���κ��� ������ �ȼ��� �������� �޲��ش�.
	// GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE => UV ��ǥ�� V��ǥ�� �Ѿ�� ���κ��� ������ �ȼ��� �������� �޲��ش�.

	Random_Device_Setting();
	Initialize_Camera();

	//Create VBO
	Create_Vertex_Buffer_Objects();
	Create_Lec4_Particle_VBO(PARTICLE_NUMS);
	Create_Lec5_Particle_VBO(PARTICLE_NUMS);
	Create_Sin_Particle_VBO(PARTICLE_NUMS);
	Create_FS_SandBox_VBO();
	Create_FillAll_VBO();
	Create_Simple_Texture_VBO();
	Create_VS_SandBox_VBO();
	Create_Simple_Cube_VBO();
	Create_Texture_Rect_VBO();

	//Create FBO
	for (int i = 0; i < 4; ++i) m_FBO[i] = Create_FBO(m_WindowSizeX / 2, m_WindowSizeY / 2, &m_FBO_Texture[i], false);
	m_FBO[4] = Create_FBO(m_WindowSizeX, m_WindowSizeY, &m_FBO_Texture[4], true); // Bloom FBO
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

void Renderer::Initialize_Camera()
{
	// Camera Setting
	m_World_Up_Vec3 = glm::vec3(0.0f, 0.0f, 1.0f);
	m_Camera_Pos_Vec3 = glm::vec3(0.0f, -1.0f, 0.5f);
	m_Camera_Front_Vec3 = -m_Camera_Pos_Vec3;
	Camera_Axis_Update();

	// Calculate "View Matrix"
	m_View_Mat4 = glm::lookAt(m_Camera_Pos_Vec3, m_Camera_Pos_Vec3 + m_Camera_Front_Vec3, m_World_Up_Vec3);

	// Calculate "Projection Matrix"
	//m_Projection_Mat4 = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, 0.0f, 100.0f);
	m_Projection_Mat4 = glm::perspective(PI * 0.5f, 1.0f, 0.001f, 100.0f);

	// Multiply "View and Projection Matrix"
	m_View_Proj_Mat4 = m_Projection_Mat4 * m_View_Mat4;
}

void Renderer::Create_Vertex_Buffer_Objects() // Renderer::Test()�� ���� VBO ������
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
	float temp_Pos_X, temp_Pos_Y; // �ʱ� ��ġ
	float temp_Velocity_X, temp_Velocity_Y, temp_Velocity_Z; // �ӵ�
	float temp_Start_Time, temp_Life_Time; // ���� �ð�, ���� �ֱ�
	float temp_Amplitude, temp_Ratio; // ����, ���� �ֱ�
	float temp_Value; // ���� ��
	float temp_Color_R, temp_Color_G, temp_Color_B, temp_Color_A; // ����

	// ��ƼŬ ���� ���� = particle_Count
	// ��ƼŬ �������� ���� ���� = 6
	// ������ ������ ���� = 11
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

void Renderer::Create_FS_SandBox_VBO()
{
	float temp_Pos_X, temp_Pos_Y; // �ʱ� ��ġ
	float temp_Color_R = 1.0f, temp_Color_G = 1.0f, temp_Color_B = 1.0f, temp_Color_A = 1.0f; // ����

	// ���� ���� = 6
	// ������ ������ ���� = 9
	int array_Length = 6 * 9;
	float* Vertices = new float[array_Length];

	for (int i = 0; i < array_Length; ++i)
	{
		Vertices[i++] = -SANDBOX_HALF_SIZE; // Pos_X
		Vertices[i++] = SANDBOX_HALF_SIZE; // Pos_Y
		Vertices[i++] = 0.0f; // Pos_Z
		Vertices[i++] = temp_Color_R; // Color R
		Vertices[i++] = temp_Color_G; // Color G
		Vertices[i++] = temp_Color_B; // Color B
		Vertices[i++] = temp_Color_A; // Color A
		Vertices[i++] = 0.0f;
		Vertices[i++] = 1.0f;

		Vertices[i++] = -SANDBOX_HALF_SIZE;
		Vertices[i++] = -SANDBOX_HALF_SIZE;
		Vertices[i++] = 0.0f;
		Vertices[i++] = temp_Color_R;
		Vertices[i++] = temp_Color_G;
		Vertices[i++] = temp_Color_B;
		Vertices[i++] = temp_Color_A;
		Vertices[i++] = 0.0f;
		Vertices[i++] = 0.0f;

		Vertices[i++] = SANDBOX_HALF_SIZE;
		Vertices[i++] = SANDBOX_HALF_SIZE;
		Vertices[i++] = 0.0f;
		Vertices[i++] = temp_Color_R;
		Vertices[i++] = temp_Color_G;
		Vertices[i++] = temp_Color_B;
		Vertices[i++] = temp_Color_A;
		Vertices[i++] = 1.0f;
		Vertices[i++] = 1.0f;

		Vertices[i++] = SANDBOX_HALF_SIZE;
		Vertices[i++] = -SANDBOX_HALF_SIZE;
		Vertices[i++] = 0.0f;
		Vertices[i++] = temp_Color_R;
		Vertices[i++] = temp_Color_G;
		Vertices[i++] = temp_Color_B;
		Vertices[i++] = temp_Color_A;
		Vertices[i++] = 1.0f;
		Vertices[i++] = 0.0f;

		Vertices[i++] = SANDBOX_HALF_SIZE;
		Vertices[i++] = SANDBOX_HALF_SIZE;
		Vertices[i++] = 0.0f;
		Vertices[i++] = temp_Color_R;
		Vertices[i++] = temp_Color_G;
		Vertices[i++] = temp_Color_B;
		Vertices[i++] = temp_Color_A;
		Vertices[i++] = 1.0f;
		Vertices[i++] = 1.0f;

		Vertices[i++] = -SANDBOX_HALF_SIZE;
		Vertices[i++] = -SANDBOX_HALF_SIZE;
		Vertices[i++] = 0.0f;
		Vertices[i++] = temp_Color_R;
		Vertices[i++] = temp_Color_G;
		Vertices[i++] = temp_Color_B;
		Vertices[i++] = temp_Color_A;
		Vertices[i++] = 0.0f;
		Vertices[i] = 0.0f;
	}

	glGenBuffers(1, &m_VBO_FS_SandBox);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO_FS_SandBox);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * array_Length, Vertices, GL_STATIC_DRAW);

	delete[] Vertices;
}

void Renderer::Create_FillAll_VBO()
{
	float temp_Pos_X, temp_Pos_Y; // �ʱ� ��ġ

	// ���� ���� = 6
	// ������ ������ ���� = 3
	int array_Length = 6 * 3;
	float* Vertices = new float[array_Length];

	for (int i = 0; i < array_Length; ++i)
	{
		Vertices[i++] = -FILLALL_RECT_HALF_SIZE; // Pos_X
		Vertices[i++] = FILLALL_RECT_HALF_SIZE; // Pos_Y
		Vertices[i++] = 0.0f; // Pos_Z

		Vertices[i++] = -FILLALL_RECT_HALF_SIZE;
		Vertices[i++] = -FILLALL_RECT_HALF_SIZE;
		Vertices[i++] = 0.0f;

		Vertices[i++] = FILLALL_RECT_HALF_SIZE;
		Vertices[i++] = FILLALL_RECT_HALF_SIZE;
		Vertices[i++] = 0.0f;

		Vertices[i++] = FILLALL_RECT_HALF_SIZE;
		Vertices[i++] = -FILLALL_RECT_HALF_SIZE;
		Vertices[i++] = 0.0f;

		Vertices[i++] = FILLALL_RECT_HALF_SIZE;
		Vertices[i++] = FILLALL_RECT_HALF_SIZE;
		Vertices[i++] = 0.0f;

		Vertices[i++] = -FILLALL_RECT_HALF_SIZE;
		Vertices[i++] = -FILLALL_RECT_HALF_SIZE;
		Vertices[i] = 0.0f;
	}

	glGenBuffers(1, &m_VBO_FillAll);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO_FillAll);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * array_Length, Vertices, GL_STATIC_DRAW);

	delete[] Vertices;
}

void Renderer::Create_Simple_Texture_VBO()
{
	int array_Length = 6 * 5;
	float* Vertices = new float[array_Length];

	for (int i = 0; i < array_Length; ++i)
	{
		Vertices[i++] = -SIMPLE_TEXTURE_HALF_SIZE; // Pos_X
		Vertices[i++] = SIMPLE_TEXTURE_HALF_SIZE; // Pos_Y
		Vertices[i++] = 0.0f; // Pos_Z
		Vertices[i++] = 0.0f; // UV_Pos_U
		Vertices[i++] = 1.0f; // UV_Pos_V

		Vertices[i++] = -SIMPLE_TEXTURE_HALF_SIZE;
		Vertices[i++] = -SIMPLE_TEXTURE_HALF_SIZE;
		Vertices[i++] = 0.0f;
		Vertices[i++] = 0.0f;
		Vertices[i++] = 0.0f;

		Vertices[i++] = SIMPLE_TEXTURE_HALF_SIZE;
		Vertices[i++] = SIMPLE_TEXTURE_HALF_SIZE;
		Vertices[i++] = 0.0f;
		Vertices[i++] = 1.0f;
		Vertices[i++] = 1.0f;

		Vertices[i++] = SIMPLE_TEXTURE_HALF_SIZE;
		Vertices[i++] = -SIMPLE_TEXTURE_HALF_SIZE;
		Vertices[i++] = 0.0f;
		Vertices[i++] = 1.0f;
		Vertices[i++] = 0.0f;

		Vertices[i++] = SIMPLE_TEXTURE_HALF_SIZE;
		Vertices[i++] = SIMPLE_TEXTURE_HALF_SIZE;
		Vertices[i++] = 0.0f;
		Vertices[i++] = 1.0f;
		Vertices[i++] = 1.0f;

		Vertices[i++] = -SIMPLE_TEXTURE_HALF_SIZE;
		Vertices[i++] = -SIMPLE_TEXTURE_HALF_SIZE;
		Vertices[i++] = 0.0f;
		Vertices[i++] = 0.0f;
		Vertices[i] = 0.0f;
	}

	glGenBuffers(1, &m_VBO_Simple_Texture);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO_Simple_Texture);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * array_Length, Vertices, GL_STATIC_DRAW);

	delete[] Vertices;
}

void Renderer::Create_VS_SandBox_VBO()
{
	float basePosX = -0.5f;
	float basePosY = -0.5f;
	float targetPosX = 0.5f;
	float targetPosY = 0.5f;

	int pointCountX = 50;
	int pointCountY = 50;

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

	glGenBuffers(1, &m_VBO_VS_SandBox);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO_VS_SandBox);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * (pointCountX - 1) * (pointCountY - 1) * 2 * 3 * 3, vertices, GL_STATIC_DRAW);

	delete[] point;
	delete[] vertices;
}

void Renderer::Create_Simple_Cube_VBO()
{
	float temp = 0.5f;
	int attrib_count = 10;
	int vertices_count = 36;
	int array_size = attrib_count * vertices_count;

	float cube[] = {
	-temp, -temp, -temp, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, //x, y, z, nx, ny, nz, r, g, b, a
	-temp, -temp, temp, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
	-temp, temp, temp, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,

	temp, temp, -temp, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
	-temp, -temp, -temp, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
	-temp, temp, -temp, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f,

	temp, -temp, temp, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
	-temp, -temp, -temp, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
	temp, -temp, -temp, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,

	temp, temp, -temp, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
	temp, -temp, -temp, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
	-temp, -temp, -temp, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f,

	-temp, -temp, -temp, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
	-temp, temp, temp, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
	-temp, temp, -temp, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,

	temp, -temp, temp, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
	-temp, -temp, temp, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
	-temp, -temp, -temp, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,

	-temp, temp, temp, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
	-temp, -temp, temp, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
	temp, -temp, temp, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,

	temp, temp, temp, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
	temp, -temp, -temp, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
	temp, temp, -temp, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,

	temp, -temp, -temp, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
	temp, temp, temp, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
	temp, -temp, temp, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,

	temp, temp, temp, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
	temp, temp, -temp, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
	-temp, temp, -temp, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,

	temp, temp, temp, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
	-temp, temp, -temp, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
	-temp, temp, temp, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,

	temp, temp, temp, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
	-temp, temp, temp, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
	temp, -temp, temp, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
	};

	glGenBuffers(1, &m_VBO_Simple_Cube);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO_Simple_Cube);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * array_size, cube, GL_STATIC_DRAW);
}

void Renderer::Create_Texture_Rect_VBO()
{
	float size = 0.5f;
	float texRect[]
		=
	{
		-size, -size, 0.f, 0.f, 0.f, //x, y, z, u, v
		-size, size, 0.f, 0.f, 1.f,
		size, size, 0.f, 1.f, 1.f, //Triangle1
		-size, -size, 0.f, 0.f, 0.f,
		size, size, 0.f, 1.f, 1.f,
		size, -size, 0.f, 1.f, 0.f //Triangle2
	};

	glGenBuffers(1, &m_VBO_Texture_Rect);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO_Texture_Rect);
	glBufferData(GL_ARRAY_BUFFER, sizeof(texRect), texRect, GL_STATIC_DRAW);
}




//=================================================================


// Off-Screen Buffer�� �����!
// ����, ȭ�鿡 �׷����� ��� ������Ʈ�� �׷��� �Ŀ� ó���ؾ� �ϴ� "�׸���"
// ���� �͵��� ���⿡ ���� �׷��� �����س���, ���߿� ���� ������ ���ۿ� ��� �׸���.
GLuint Renderer::Create_FBO(const int& size_x, const int& size_y, GLuint* ret_texture_id, const bool& is_HDR)
{
	// Frame Buffer Object ��ü�� �������̹Ƿ� Color Buffer�� Depth Buffer��
	// ���� �����ؼ� �ٿ���� �Ѵ�. 

	// "Gen Render Target"
	// ������ �� ����� �ؽ��� ���·� �����ϱ� ������,
	// �� �ؽ��ĸ� ���� ������ �־�� �Ѵ�.
	GLuint temp_Texture = 0;
	glGenTextures(1, &temp_Texture);
	glBindTexture(GL_TEXTURE_2D, temp_Texture);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);

	// HDR Texture / SDR Texture
	if (is_HDR) glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, size_x, size_y, 0, GL_RGBA, GL_FLOAT, 0);
	else glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, size_x, size_y, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);



	// "Gen Depth Buffer" (Render Buffer)
	// ���, Depth Buffer�� ���� ���� �ٿ����� �ʾƵ�
	// '�⺻ Depth Buffer'�� ������� �����Ѵ�.
	// �㳪, �⺻ Depth Buffer�� '����'�Ѵٴ� ���� '�ٸ� Frame Buffer'�鿡�� ������ ũ�� ��ġ�Ƿ�,
	// ���� ���� ����ϴ°��� �����ϴ�.
	glGenRenderbuffers(1, &m_DepthBuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, m_DepthBuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, size_x, size_y);
	


	// Attach "Texture and Depth Buffer" to FBO
	GLuint temp_FBO;
	glGenFramebuffers(1, &temp_FBO);
	glBindFramebuffer(GL_FRAMEBUFFER, temp_FBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, temp_Texture, 0);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_DepthBuffer);



	// Success Check
	GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (status != GL_FRAMEBUFFER_COMPLETE)
	{
		cout << "Error While Attach FBO." << endl;
		return 0;
	}



	// Save and initialize.
	glBindFramebuffer(GL_FRAMEBUFFER, 0);  // ������ Frame Buffer�� "Main Frame Buffer"�� ��ȯ.
	glBindRenderbuffer(GL_RENDERBUFFER, 0); // ������ Render Buffer�� "Main Render Buffer"�� ��ȯ.

	*ret_texture_id = temp_Texture; // ������ Color Buffer�� ID ����.

	return temp_FBO; // �ϼ��� FBO ID ��ȯ.
}


//=================================================================


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

	glDisable(GL_BLEND);
}

void Renderer::Draw_FS_SandBox()
{
	// prepare points
	GLfloat points[] = { 0.0f, 0.0f, 0.5f, 0.5f, 0.3f, 0.3f, -0.2f, -0.2f, -0.3f, -0.3f };

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// ===============================================

	GLuint shader_ID = m_FS_SandBox_Shader;
	glUseProgram(shader_ID);

	// ===============================================

	GLuint u_Time = glGetUniformLocation(shader_ID, "u_Time");
	glUniform1f(u_Time, m_Time);

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

	glBindBuffer(GL_ARRAY_BUFFER, m_VBO_FS_SandBox);
	glVertexAttribPointer(a_Position, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 9, 0);
	glVertexAttribPointer(a_Color, 4, GL_FLOAT, GL_FALSE, sizeof(float) * 9, (GLvoid*)(sizeof(float) * 3));
	glVertexAttribPointer(a_UV, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 9, (GLvoid*)(sizeof(float) * 7));

	glDrawArrays(GL_TRIANGLES, 0, 6);

	// ===============================================

	glDisableVertexAttribArray(a_Position);
	glDisableVertexAttribArray(a_Color);
	glDisableVertexAttribArray(a_UV);

	glDisable(GL_BLEND);
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

	glDisable(GL_BLEND);
}

void Renderer::Draw_Simple_Texture()
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// ===============================================

	GLuint shader_ID = m_Simple_Texture_Shader;
	glUseProgram(shader_ID);

	// ===============================================

	GLuint u_Time = glGetUniformLocation(shader_ID, "u_Time");
	glUniform1f(u_Time, m_Time);

	
	m_Number = floorf(m_Time);
	GLuint u_Number = glGetUniformLocation(shader_ID, "u_Number");
	glUniform1i(u_Number, m_Number);

	GLuint u_X_Index = glGetUniformLocation(shader_ID, "u_X_Index");
	glUniform1f(u_X_Index, m_Time * 20.0f);
	GLuint u_Resolution_X = glGetUniformLocation(shader_ID, "u_Resolution_X");
	glUniform1f(u_Resolution_X, 6.0f);
	GLuint u_Resolution_Y = glGetUniformLocation(shader_ID, "u_Resolution_Y");
	glUniform1f(u_Resolution_Y, 1.0f);
	
	int min, sec_1, sec_2;
	min = m_Time / 60.0f;
	sec_1 = (m_Time - min * 60) * 0.1f;
	sec_2 = m_Time - min * 60 - sec_1 * 10;
	int time[3] = { min, sec_1, sec_2 };
	GLuint u_Numbers = glGetUniformLocation(shader_ID, "u_Numbers");
	glUniform1iv(u_Numbers, 3, time);
	

	// Apply Textures ID
	GLuint u_Texture = glGetUniformLocation(shader_ID, "u_Texture");
	glUniform1i(u_Texture, 0);
	GLuint u_Texture_1 = glGetUniformLocation(shader_ID, "u_Texture_1");
	glUniform1i(u_Texture_1, 1);
	GLuint u_Texture_2 = glGetUniformLocation(shader_ID, "u_Texture_2");
	glUniform1i(u_Texture_2, 2);
	GLuint u_Texture_3 = glGetUniformLocation(shader_ID, "u_Texture_3");
	glUniform1i(u_Texture_3, 3);
	GLuint u_Texture_4 = glGetUniformLocation(shader_ID, "u_Texture_4");
	glUniform1i(u_Texture_4, 4);
	GLuint u_Number_Texture = glGetUniformLocation(shader_ID, "u_Number_Texture");
	glUniform1i(u_Number_Texture, 5);
	GLuint u_Number_Texture_2 = glGetUniformLocation(shader_ID, "u_Number_Texture_2");
	glUniform1i(u_Number_Texture_2, 6);
	GLuint u_Texture_1_Reversed = glGetUniformLocation(shader_ID, "u_Texture_1_Reversed");
	glUniform1i(u_Texture_1_Reversed, 7);

	// Activate Textures
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_Sample_RGB_Texture);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, m_Particle_Texture_1);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, m_Particle_Texture_2);
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, m_Full_Moon_Texture);
	glActiveTexture(GL_TEXTURE4);
	glBindTexture(GL_TEXTURE_2D, m_Wooden_Box_Texture);
	glActiveTexture(GL_TEXTURE5);
	glBindTexture(GL_TEXTURE_2D, m_Number_Texture);
	glActiveTexture(GL_TEXTURE6);
	glBindTexture(GL_TEXTURE_2D, m_Number_Texture_2);
	glActiveTexture(GL_TEXTURE7);
	glBindTexture(GL_TEXTURE_2D, m_Particle_Texture_3);


	// Apply Sprites ID
	GLuint u_Sans_Sprite = glGetUniformLocation(shader_ID, "u_Sans_Sprite");
	glUniform1i(u_Sans_Sprite, 8);
	GLuint u_Runner_Sprite = glGetUniformLocation(shader_ID, "u_Runner_Sprite");
	glUniform1i(u_Runner_Sprite, 9);
	GLuint u_Gunner_Sprite = glGetUniformLocation(shader_ID, "u_Gunner_Sprite");
	glUniform1i(u_Gunner_Sprite, 10);
	GLuint u_Runner_Sprite_Reversed = glGetUniformLocation(shader_ID, "u_Runner_Sprite_Reversed");
	glUniform1i(u_Runner_Sprite_Reversed, 11);
	GLuint u_Flame_Sprite = glGetUniformLocation(shader_ID, "u_Flame_Sprite");
	glUniform1i(u_Flame_Sprite, 12);

	// Activate Sprites
	glActiveTexture(GL_TEXTURE8);
	glBindTexture(GL_TEXTURE_2D, m_Sans_Sprite);
	glActiveTexture(GL_TEXTURE9);
	glBindTexture(GL_TEXTURE_2D, m_Runner_Sprite);
	glActiveTexture(GL_TEXTURE10);
	glBindTexture(GL_TEXTURE_2D, m_Gunner_Sprite);
	glActiveTexture(GL_TEXTURE11);
	glBindTexture(GL_TEXTURE_2D, m_Runner_Sprite_2);
	glActiveTexture(GL_TEXTURE12);
	glBindTexture(GL_TEXTURE_2D, m_Flame_Sprite);


	// GPU�� �ϳ��� Shader���� �ؽ��ĸ� �ѹ��� 80�� ���� ����� �� �ִ�.
	// glActiveTexture()�� �ش� �ؽ��� ������ Ȱ��ȭ �ؾ� ��� �� �� �ִ�.
	// �׸��� glBindTexture()�� �ϸ�, ������ Ȱ��ȭ�� �ؽ��� ���Կ� ���� �ؽ��ĸ� ���ε� �� �� �ִ�.

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

	glDisable(GL_BLEND);
}

void Renderer::Draw_VS_SandBox()
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// ===============================================

	GLuint shader_ID = m_VS_SandBox_Shader;
	glUseProgram(shader_ID);

	// ===============================================

	GLuint u_Time = glGetUniformLocation(shader_ID, "u_Time");
	glUniform1f(u_Time, m_Time);

	GLfloat points[] = { 0.0f, 0.0f, 0.5f, 0.0f, 0.3f, 0.3f, -0.5f, 0.0f, -0.3f, -0.3f };
	GLuint u_Points = glGetUniformLocation(shader_ID, "u_Points");
	glUniform2fv(u_Points, 5, points);

	GLuint u_Texture = glGetUniformLocation(shader_ID, "u_Texture");
	glUniform1i(u_Texture, 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_Sans_Sprite);

	// ===============================================

	int a_Position = glGetAttribLocation(shader_ID, "a_Position");

	glEnableVertexAttribArray(a_Position);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO_VS_SandBox);
	glVertexAttribPointer(a_Position, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, 0);

	glDrawArrays(GL_LINE_STRIP, 0, m_Count_ProxyGeo);

	// ===============================================

	glDisableVertexAttribArray(a_Position);

	glDisable(GL_BLEND);
}

//#define RENDER_CUBE
void Renderer::Draw_Simple_Cube()
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	glEnable(GL_CULL_FACE);

	// ===============================================

	GLuint shader_ID = m_Simple_Cube_Shader;
	glUseProgram(shader_ID);

	// ===============================================

	GLuint u_Time = glGetUniformLocation(shader_ID, "u_Time");
	glUniform1f(u_Time, m_Time);

	GLuint u_ViewProj_Matrix = glGetUniformLocation(shader_ID, "u_ViewProj_Matrix");
	glUniformMatrix4fv(u_ViewProj_Matrix, 1, GL_FALSE, &m_View_Proj_Mat4[0][0]);

	GLuint u_Camera_Position = glGetUniformLocation(shader_ID, "u_Camera_Position");
	glUniform3f(u_Camera_Position, m_Camera_Pos_Vec3.x, m_Camera_Pos_Vec3.y, m_Camera_Pos_Vec3.z);
	

#ifdef RENDER_CUBE
	// ** m_VBO_Simple_Cube ���� **
	// ===============================================
	
	int a_Position = glGetAttribLocation(shader_ID, "a_Position");
	int a_Normal = glGetAttribLocation(shader_ID, "a_Normal");
	int a_Color = glGetAttribLocation(shader_ID, "a_Color");

	glEnableVertexAttribArray(a_Position);
	glEnableVertexAttribArray(a_Normal);
	glEnableVertexAttribArray(a_Color);

	glBindBuffer(GL_ARRAY_BUFFER, m_VBO_Simple_Cube);

	glVertexAttribPointer(a_Position, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 10, 0);
	glVertexAttribPointer(a_Normal, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 10, (GLvoid*)(sizeof(float) * 3));
	glVertexAttribPointer(a_Color, 4, GL_FLOAT, GL_FALSE, sizeof(float) * 10, (GLvoid*)(sizeof(float) * 6));

	glDrawArrays(GL_TRIANGLES, 0, 36);
	
	// ===============================================

	glDisableVertexAttribArray(a_Position);
	glDisableVertexAttribArray(a_Normal);
	glDisableVertexAttribArray(a_Color);
	


#else
	// * m_VBO_VS_SandBox ��� *
	// ===============================================
	
	GLfloat points[] = { 0.0f, 0.0f, 0.5f, 0.0f, 0.3f, 0.3f, -0.5f, 0.0f, -0.3f, -0.3f };
	GLuint u_Points = glGetUniformLocation(shader_ID, "u_Points");
	glUniform2fv(u_Points, 5, points);

	GLuint u_Texture = glGetUniformLocation(shader_ID, "u_Texture");
	glUniform1i(u_Texture, 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_Sans_Sprite);

	GLuint u_Texture_Height_Map = glGetUniformLocation(shader_ID, "u_Texture_Height_Map");
	glUniform1i(u_Texture_Height_Map, 1);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, m_Height_Map_Texture);

	GLuint u_Texture_Snow = glGetUniformLocation(shader_ID, "u_Texture_Snow");
	glUniform1i(u_Texture_Snow, 2);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, m_Snow_Texture);

	GLuint u_Texture_Grass = glGetUniformLocation(shader_ID, "u_Texture_Grass");
	glUniform1i(u_Texture_Grass, 3);
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, m_Grass_Texture);

	GLuint u_Texture_Height_Map_2 = glGetUniformLocation(shader_ID, "u_Texture_Height_Map_2");
	glUniform1i(u_Texture_Height_Map_2, 4);
	glActiveTexture(GL_TEXTURE4);
	glBindTexture(GL_TEXTURE_2D, m_Height_Map_Texture_2);


	int a_Position = glGetAttribLocation(shader_ID, "a_Position");

	glEnableVertexAttribArray(a_Position);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO_VS_SandBox);
	glVertexAttribPointer(a_Position, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, 0);

	glDrawArrays(GL_TRIANGLES, 0, m_Count_ProxyGeo);

	glDisableVertexAttribArray(a_Position);
	
	// ===============================================

#endif

	glDisable(GL_BLEND);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
}

void Renderer::Draw_Texture_Rect(const GLuint& texture, const float& x, const float& y, const float& size_x, const float& size_y)
{
	GLuint shader_ID = m_Texture_Rect_Shader;
	glUseProgram(shader_ID);

	// ===============================================

	GLuint u_Position = glGetUniformLocation(shader_ID, "u_Position");
	glUniform2f(u_Position, x, y);

	GLuint u_Size = glGetUniformLocation(shader_ID, "u_Size");
	glUniform2f(u_Size, size_x, size_y);

	GLuint u_Texture = glGetUniformLocation(shader_ID, "u_Texture");
	glUniform1i(u_Texture, 0);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);

	// ===============================================

	GLuint a_Position = glGetAttribLocation(shader_ID, "a_Position");
	GLuint a_Texture_UV = glGetAttribLocation(shader_ID, "a_Texture_UV");

	glEnableVertexAttribArray(a_Position);
	glEnableVertexAttribArray(a_Texture_UV);

	// ===============================================

	glBindBuffer(GL_ARRAY_BUFFER, m_VBO_Texture_Rect);
	glVertexAttribPointer(a_Position, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 5, 0);
	glVertexAttribPointer(a_Texture_UV, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 5, (GLvoid*)(sizeof(float) * 3));

	glDrawArrays(GL_TRIANGLES, 0, 6);

	// ===============================================

	glDisableVertexAttribArray(a_Position);
	glDisableVertexAttribArray(a_Texture_UV);
}

void Renderer::Draw_HDR_Texture_Rect(const GLuint& texture, const float& x, const float& y, const float& size_x, const float& size_y)
{
	GLuint shader_ID = m_HDR_Texture_Rect_Shader;
	glUseProgram(shader_ID);

	// ===============================================

	GLuint u_Position = glGetUniformLocation(shader_ID, "u_Position");
	glUniform2f(u_Position, x, y);

	GLuint u_Size = glGetUniformLocation(shader_ID, "u_Size");
	glUniform2f(u_Size, size_x, size_y);

	GLuint u_Texture = glGetUniformLocation(shader_ID, "u_Texture");
	glUniform1i(u_Texture, 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);

	GLuint u_Blur_Size = glGetUniformLocation(shader_ID, "u_Blur_Size");
	glUniform1f(u_Blur_Size, 30.0f);

	GLuint u_Texel_Size = glGetUniformLocation(shader_ID, "u_Texel_Size");
	glUniform2f(u_Texel_Size, 1.0f / m_WindowSizeX, 1.0f / m_WindowSizeY);

	// ===============================================

	GLuint a_Position = glGetAttribLocation(shader_ID, "a_Position");
	GLuint a_Texture_UV = glGetAttribLocation(shader_ID, "a_Texture_UV");

	glEnableVertexAttribArray(a_Position);
	glEnableVertexAttribArray(a_Texture_UV);

	// ===============================================

	glBindBuffer(GL_ARRAY_BUFFER, m_VBO_Texture_Rect);
	glVertexAttribPointer(a_Position, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 5, 0);
	glVertexAttribPointer(a_Texture_UV, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 5, (GLvoid*)(sizeof(float) * 3));

	glDrawArrays(GL_TRIANGLES, 0, 6);

	// ===============================================

	glDisableVertexAttribArray(a_Position);
	glDisableVertexAttribArray(a_Texture_UV);
}

void Renderer::Test_FBO()
{
	// �۾� ���� ����.
	// 1. ����� Frame Buffer ���ε�
	// 2. ����Ʈ ��ġ �� ũ�� ����. (�����ָ� Default ũ��� ����ȴ�..)
	// 3. �����ӹ��� Ŭ���� (Color Buffer, Depth Buffer)
	// 4. ������! (Texture ���·� ����)
	// 5. ** 1~4 ������ �ܰ� ������ �۾��� '�������� ������ ����'�� ������ �Ͽ�
	//		�ؽ��� ���·� �����س��� ���̴�.
	//		���� ���������� ȭ�鿡 �׷��ֱ� ���� 'Default Frame Buffer'�� ���ε� �� ����,
	//		����� ���� �ؽ��ĸ� ������ �ϴ� ������� ����ؾ��Ѵ�.
	
	int window_half_size_X = m_WindowSizeX / 2;
	int window_half_size_Y = m_WindowSizeY / 2;

	glBindFramebuffer(GL_FRAMEBUFFER, m_FBO[0]);
	glViewport(0, 0, window_half_size_X, window_half_size_Y);
	Fill_All(0.2f);
	glClear(GL_DEPTH_BUFFER_BIT);
	Draw_Simple_Cube();

	glBindFramebuffer(GL_FRAMEBUFFER, m_FBO[1]);
	glViewport(0, 0, window_half_size_X, window_half_size_Y);
	glClearColor(0.2f, 0.5f, 0.2f, 1.0f);
	glClearDepth(1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	Draw_Sin_Particle();

	glBindFramebuffer(GL_FRAMEBUFFER, m_FBO[2]);
	glViewport(0, 0, window_half_size_X, window_half_size_Y);
	glClearColor(0.2f, 0.2f, 0.5f, 1.0f);
	glClearDepth(1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	Draw_VS_SandBox();

	glBindFramebuffer(GL_FRAMEBUFFER, m_FBO[3]);
	glViewport(0, 0, window_half_size_X, window_half_size_Y);
	glClearColor(0.5f, 0.2f, 0.2f, 1.0f);
	glClearDepth(1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	Draw_FS_SandBox();

	// Draw Each FrameBuffer's Texture
	glBindFramebuffer(GL_FRAMEBUFFER, 0); // Default Frame Buffer�� ��ȯ.
	glViewport(0, 0, m_WindowSizeX, m_WindowSizeY);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	Draw_Texture_Rect(m_FBO_Texture[0], -0.5f, -0.5f, 1.0f, 1.0f);
	Draw_Texture_Rect(m_FBO_Texture[1], 0.5f, -0.5f, 1.0f, 1.0f);
	Draw_Texture_Rect(m_FBO_Texture[2], -0.5f, 0.5f, 1.0f, 1.0f);
	Draw_Texture_Rect(m_FBO_Texture[3], 0.5f, 0.5f, 1.0f, 1.0f);
}

//#define RENDER_4_KIND
void Renderer::Bloom_FBO()
{
#ifdef RENDER_4_KIND

	// FBO �ϳ��� 4��� �ؼ� ���� �ٸ� 4���� �������ϱ�.
	int window_half_size_X = m_WindowSizeX / 2;
	int window_half_size_Y = m_WindowSizeY / 2;

	glEnable(GL_SCISSOR_TEST); // glClear�� �κ������� �����ϱ� ���� Scissor Test ���.

	glBindFramebuffer(GL_FRAMEBUFFER, m_FBO[4]);
	glViewport(0, window_half_size_Y, window_half_size_X, window_half_size_Y);
	glScissor(0, window_half_size_Y, window_half_size_X, window_half_size_Y);
	glClearColor(0.5f, 0.2f, 0.2f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	Draw_Sin_Particle();
	
	glViewport(window_half_size_X, window_half_size_Y, window_half_size_X, window_half_size_Y);
	glScissor(window_half_size_X, window_half_size_Y, window_half_size_X, window_half_size_Y);
	glClearColor(0.2f, 0.5f, 0.2f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	Draw_Sin_Particle();

	glViewport(0, 0, window_half_size_X, window_half_size_Y);
	glScissor(0, 0, window_half_size_X, window_half_size_Y);
	glClearColor(0.2f, 0.2f, 0.5f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	Draw_Sin_Particle();

	glViewport(window_half_size_X, 0, window_half_size_X, window_half_size_Y);
	glScissor(window_half_size_X, 0, window_half_size_X, window_half_size_Y);
	glClearColor(0.7f, 0.7f, 0.7f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	Draw_Sin_Particle();

	glDisable(GL_SCISSOR_TEST);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, m_WindowSizeX, m_WindowSizeY);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	Draw_HDR_Texture_Rect(m_FBO_Texture[4], 0.0f, 0.0f, 2.0f, 2.0f);

#else

	// �� FBO ������ ��
	glBindFramebuffer(GL_FRAMEBUFFER, m_FBO[4]);
	glViewport(0, 0, m_WindowSizeX, m_WindowSizeY);
	Fill_All(0.2f);
	glClear(GL_DEPTH_BUFFER_BIT);
	//Draw_Simple_Cube();
	Draw_Sin_Particle();

	// Draw Each FrameBuffer's Texture
	glBindFramebuffer(GL_FRAMEBUFFER, 0); // Default Frame Buffer�� ��ȯ.
	glViewport(0, 0, m_WindowSizeX, m_WindowSizeY);
	Draw_HDR_Texture_Rect(m_FBO_Texture[4], 0.0f, 0.0f, 2.0f, 2.0f);

#endif
}

void Renderer::Rendering(const float& elapsed_time)
{
	// ** Time Update **
	m_Time += elapsed_time;

	
	// ** Scene Clearing **
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_DEPTH_BUFFER_BIT); // Depth Test�� ������ �� ���� Depth Buffer�� ��� ���̰� �ȴ�. �׷��� "�� �����ٰ�."
	Fill_All(0.2f);
	

	// ** Render **
	//Test();
	//Draw_Lec4_Particle();
	//Draw_Lec5_Particle();
	//Draw_Sin_Particle();
	//Draw_FS_SandBox();
	//Draw_Simple_Texture();
	//Draw_VS_SandBox();
	Draw_Simple_Cube();
	//Test_FBO();
	//Bloom_FBO();
}



void Renderer::Camera_Axis_Update()
{
	m_Camera_Front_Vec3 = glm::normalize(m_Camera_Front_Vec3);
	m_Camera_Right_Vec3 = glm::normalize(glm::cross(m_Camera_Front_Vec3, m_World_Up_Vec3));
	m_Camera_Up_Vec3 = glm::normalize(glm::cross(m_Camera_Right_Vec3, m_Camera_Front_Vec3));
}

void Renderer::Camera_Translate(const glm::vec3& velocity)
{
	m_Camera_Pos_Vec3 += velocity.z * m_Camera_Front_Vec3;
	m_Camera_Pos_Vec3 += velocity.x * m_Camera_Right_Vec3;
	m_Camera_Pos_Vec3 += velocity.y * m_Camera_Up_Vec3;
	
	m_View_Mat4 = glm::lookAt(m_Camera_Pos_Vec3, m_Camera_Pos_Vec3 + m_Camera_Front_Vec3, m_World_Up_Vec3);
	m_View_Proj_Mat4 = m_Projection_Mat4 * m_View_Mat4;
}

#define USING_QUAT_TO_MAT4
void Renderer::Camera_Rotate(const glm::vec3& rot_value_xyz)
{ 
	glm::vec3 EulerAngles(rot_value_xyz * PI * 0.001f);
	glm::quat quat;

#ifdef USING_QUAT_TO_MAT4
	quat = glm::angleAxis(glm::degrees(EulerAngles.x), m_Camera_Right_Vec3); // ���� "Camera�� Right ����"�� ���� "Camera�� Front ����"�� ȸ��.
	m_Camera_Front_Vec3 = glm::mat4_cast(quat) * glm::vec4(m_Camera_Front_Vec3, 0.0f);
	quat = glm::angleAxis(glm::degrees(EulerAngles.y), m_Camera_Up_Vec3); // ���� "Camera�� Up ����"�� ���� "Camera�� Front ����"�� ȸ��.
	m_Camera_Front_Vec3 = glm::mat4_cast(quat) * glm::vec4(m_Camera_Front_Vec3, 0.0f);
	quat = glm::angleAxis(glm::degrees(EulerAngles.z), m_Camera_Front_Vec3); // ���� "Camera�� Front ����"�� ���� "World�� Up ����"�� ȸ��.
	m_World_Up_Vec3 = glm::mat4_cast(quat) * glm::vec4(m_World_Up_Vec3, 0.0f);
	
#else
	glm::quat v, quat_conj;
	quat = glm::angleAxis(glm::degrees(EulerAngles.x), m_Camera_Right_Vec3);
	quat_conj = glm::conjugate(quat);
	v = glm::quat(0.0f, m_Camera_Front_Vec3);
	v = quat * v * quat_conj;
	m_Camera_Front_Vec3 = glm::vec3(v.x, v.y, v.z);

	quat = glm::angleAxis(glm::degrees(EulerAngles.y), m_Camera_Up_Vec3);
	quat_conj = glm::conjugate(quat);
	v = glm::quat(0.0f, m_Camera_Front_Vec3);
	v = quat * v * quat_conj;
	m_Camera_Front_Vec3 = glm::vec3(v.x, v.y, v.z);

	quat = glm::angleAxis(glm::degrees(EulerAngles.z), m_Camera_Front_Vec3);
	quat_conj = glm::conjugate(quat);
	v = glm::quat(0.0f, m_World_Up_Vec3);
	v = quat * v * quat_conj;
	m_World_Up_Vec3 = glm::vec3(v.x, v.y, v.z);
	
#endif

	Camera_Axis_Update();

	m_View_Mat4 = glm::lookAt(m_Camera_Pos_Vec3, m_Camera_Pos_Vec3 + m_Camera_Front_Vec3, m_World_Up_Vec3);
	m_View_Proj_Mat4 = m_Projection_Mat4 * m_View_Mat4;
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