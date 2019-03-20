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
	
	//Random Device Setting
	Random_Device_Setting();

	//Create VBOs
	CreateVertexBufferObjects();

	//Gen Quads
	//Gen_Quads(GEN_QUADS_NUMS);
	
	//Create Proxy Geometry
	CreateProxyGeometry();
}

void Renderer::Random_Device_Setting()
{
	mt19937_64 temp_seed(m_Random_Device());
	m_Random_Seed = temp_seed;

	uniform_real_distribution<> temp_random_pos(GEN_QUADS_POS_MIN, GEN_QUADS_POS_MAX);
	m_Random_Position = temp_random_pos;
}

void Renderer::CreateVertexBufferObjects() // Renderer::Test()를 위한 VBO 생성기
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

void Renderer::Create_Lecture_VBO()
{
	float temp_X = m_Random_Position(m_Random_Seed);
	float temp_Y = m_Random_Position(m_Random_Seed);

	float Quads_Vertice[]
		=
	{
		temp_X - GEN_QUADS_HALF_SIZE, temp_Y + GEN_QUADS_HALF_SIZE, 0.0f, temp_X, temp_Y,
		temp_X - GEN_QUADS_HALF_SIZE, temp_Y - GEN_QUADS_HALF_SIZE, 0.0f, temp_X, temp_Y,
		temp_X + GEN_QUADS_HALF_SIZE, temp_Y + GEN_QUADS_HALF_SIZE, 0.0f, temp_X, temp_Y,
		temp_X + GEN_QUADS_HALF_SIZE, temp_Y + GEN_QUADS_HALF_SIZE, 0.0f, temp_X, temp_Y,
		temp_X - GEN_QUADS_HALF_SIZE, temp_Y - GEN_QUADS_HALF_SIZE, 0.0f, temp_X, temp_Y,
		temp_X + GEN_QUADS_HALF_SIZE, temp_Y - GEN_QUADS_HALF_SIZE, 0.0f, temp_X, temp_Y
	};

	GLuint id = 0;
	glGenBuffers(1, &id);
	m_Gen_Quads_VBO_IDs_Vector.push_back(id);
	
	glBindBuffer(GL_ARRAY_BUFFER, id);

	glBufferData(GL_ARRAY_BUFFER, sizeof(Quads_Vertice), Quads_Vertice, GL_STATIC_DRAW);
}

void Renderer::Gen_Quads(const int& num)
{
	for (int i = 0; i < num; ++i) Create_Lecture_VBO();
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
}

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
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, bmp);

	return temp;
}

void Renderer::Test()
{
	glUseProgram(m_SolidRectShader);

	if (m_Scale > 1.0f) { m_Scale = 0.0f; }
	m_Scale += 0.01f;

	GLuint u_Time = glGetUniformLocation(m_SolidRectShader, "u_Time");
	glUniform1f(u_Time, m_Scale); // 해당 Uniform 변수의 값을 변경한다.


	GLuint a_Position = glGetAttribLocation(m_SolidRectShader, "a_Position");
	// 해당 Shader의 "a_Position" 이라는 Attribute의 번호를 받아온다.

	glEnableVertexAttribArray(a_Position);
	// 받아온 번호의 Attribute를 사용하기 위해 Enable 시키는 것.

	glBindBuffer(GL_ARRAY_BUFFER, m_VBORect);
	// m_VBORect가 가진 ID를 불러와서 "작업대"에 올린다.

	glVertexAttribPointer(a_Position, 4, GL_FLOAT, GL_FALSE, sizeof(float) * 4, 0);
	// Draw 시 데이터를 읽어갈 단위의 크기 및 시작점을 설정한다. 인자 => (Attribute Number, 읽을 변수 하나의 크기, 읽을 자료형, ???, 건너 뛸 offset 크기, ???)
	// (여기서는 Float Pointer를 사용하고 있다.)
	// 현재 정점 정보는 3개의 위치값만으로 이루어져있기 때문에 sizeof(float) * 3을 한다.
	// (3을 곱한 이유는 데이터가 3개씩으로 이루어져있기 때문에, 정점 마다 uv값이 추가된다면 5를 곱해야한다.)

	GLuint a_Color = glGetAttribLocation(m_SolidRectShader, "a_Color");
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

void Renderer::Lecture()
{
	glUseProgram(m_SolidRectShader);

	glEnableVertexAttribArray(0);

	for (auto& VBO_id : m_Gen_Quads_VBO_IDs_Vector)
	{
		glBindBuffer(GL_ARRAY_BUFFER, VBO_id);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 5, 0);
		glDrawArrays(GL_TRIANGLES, 0, 6);
	}

	glDisableVertexAttribArray(0);
}

void Renderer::Draw_ProxyGeometry()
{
	glUseProgram(m_SolidRectShader);

	int attribPosition = glGetAttribLocation(m_SolidRectShader, "a_Position");

	glEnableVertexAttribArray(attribPosition);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO_ProxyGeo);
	glVertexAttribPointer(attribPosition, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, 0);

	glDrawArrays(GL_TRIANGLES, 0, m_Count_ProxyGeo);
	glDisableVertexAttribArray(attribPosition);
}




// ================================================================


// ★ 프록시 지오메트리(?) 구현해볼것. ★

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