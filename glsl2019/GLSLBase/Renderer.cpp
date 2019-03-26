#include "stdafx.h"
#include "Renderer.h"
#include "LoadPng.h"
#include <Windows.h>
#include <cstdlib>
#include <cassert>
#include <random>

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
	m_ParticleShaderID = CompileShaders("./Shaders/Particle.vs", "./Shaders/Particle.fs");

	//Create VBOs
	//CreateVertexBufferObjects();
	//CreateParticleBuffer(1000);
	//CreateGridMesh();
	CreateParticleMoveBuffer(100);
}

void Renderer::CreateVertexBufferObjects()
{
	float size = 0.04f;

	float rect[] =
	{
		//Triangle1
		-size, -size, 0.f, 0.5f,
		-size, size, 0.f, 0.5f,
		size, size, 0.f, 0.5f,

		//Triangle2
		-size, -size, 0.f, 0.5f,
		size, size, 0.f, 0.5f,
		size, -size, 0.f, 0.5f
	};

	// id : m_VBORect
	glGenBuffers(1, &m_VBORect);
	//	m_VBORect�� ARRAY_BUFFER�� ����ϰڴٰ� �˷���
	glBindBuffer(GL_ARRAY_BUFFER, m_VBORect);
	// ����� �־��� ���� ���� ����� �־��ִ� ���� ������
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 24, rect, GL_STATIC_DRAW);

	float Color[] =
	{
		// r g b a
		1.f, 0.f, 0.f, 1.f,
		1.f, 0.f, 0.f, 1.f,
		1.f, 0.f, 0.f, 1.f,

		// r g b a
		1.f, 0.f, 0.f, 1.f,
		1.f, 0.f, 0.f, 1.f,
		1.f, 0.f, 0.f, 1.f
	};

	// id : m_VBORect
	glGenBuffers(1, &m_VBOColor);
	//	m_VBORect�� ARRAY_BUFFER�� ����ϰڴٰ� �˷���
	glBindBuffer(GL_ARRAY_BUFFER, m_VBOColor);
	// ����� �־��� ���� ���� ����� �־��ִ� ���� ������
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 24, Color, GL_STATIC_DRAW);
}

void Renderer::CreateParticleBuffer(int ParticleCount)
{
	default_random_engine dre;
	uniform_real_distribution<double> urd(-1, 1);

	// (x, y, z)������ 3��  * �簢���� �ʿ��� ���� 6��
	m_ParticleCount = ParticleCount * 3 * 6;

	m_ParticleVertex = new float[m_ParticleCount];

	for (int i = 0; i < m_ParticleCount; i += 18)
	{
		float random_x = urd(dre);
		float random_y = urd(dre);
		// ������ �ﰢ��
		m_ParticleVertex[i] = random_x;
		m_ParticleVertex[i + 1] = random_y;
		m_ParticleVertex[i + 2] = 0.f;

		m_ParticleVertex[i + 3] = random_x + 0.02;
		m_ParticleVertex[i + 4] = random_y;
		m_ParticleVertex[i + 5] = 0.f;

		m_ParticleVertex[i + 6] = random_x + 0.02;
		m_ParticleVertex[i + 7] = random_y - 0.02;
		m_ParticleVertex[i + 8] = 0.f;

		// ���� �ﰢ��
		m_ParticleVertex[i + 9] = random_x + 0.02;
		m_ParticleVertex[i + 10] = random_y - 0.02;
		m_ParticleVertex[i + 11] = 0.f;

		m_ParticleVertex[i + 12] = random_x;
		m_ParticleVertex[i + 13] = random_y - 0.02;
		m_ParticleVertex[i + 14] = 0.f;

		m_ParticleVertex[i + 15] = random_x;
		m_ParticleVertex[i + 16] = random_y;
		m_ParticleVertex[i + 17] = 0.f;
	}

	glGenBuffers(1, &m_VBOParticle);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBOParticle);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * m_ParticleCount, m_ParticleVertex, GL_STATIC_DRAW);

	delete[] m_ParticleVertex;
}

void Renderer::CreateGridMesh()
{
	float basePosX = -0.5f;
	float basePosY = -0.5f;
	float targetPosX = 0.5f;
	float targetPosY = 0.5f;

	int pointCountX = 32;
	int pointCountY = 32;

	float width = targetPosX - basePosX;
	float height = targetPosY - basePosY;

	float* point = new float[pointCountX*pointCountY * 2];
	float* vertices = new float[(pointCountX - 1)*(pointCountY - 1) * 2 * 3 * 3];
	m_Count_GridMesh = (pointCountX - 1) * (pointCountY - 1) * 2 * 3;

	//Prepare points
	for (int x = 0; x < pointCountX; x++)
	{
		for (int y = 0; y < pointCountY; y++)
		{
			point[(y*pointCountX + x) * 2 + 0] = basePosX + width * (x / (float)(pointCountX - 1));
			point[(y*pointCountX + x) * 2 + 1] = basePosY + height * (y / (float)(pointCountY - 1));
		}
	}

	//Make triangles
	int vertIndex = 0;
	for (int x = 0; x < pointCountX - 1; x++)
	{
		for (int y = 0; y < pointCountY - 1; y++)
		{
			//Triangle part 1
			vertices[vertIndex] = point[(y*pointCountX + x) * 2 + 0];
			vertIndex++;
			vertices[vertIndex] = point[(y*pointCountX + x) * 2 + 1];
			vertIndex++;
			vertices[vertIndex] = 0.f;
			vertIndex++;
			vertices[vertIndex] = point[((y + 1)*pointCountX + (x + 1)) * 2 + 0];
			vertIndex++;
			vertices[vertIndex] = point[((y + 1)*pointCountX + (x + 1)) * 2 + 1];
			vertIndex++;
			vertices[vertIndex] = 0.f;
			vertIndex++;
			vertices[vertIndex] = point[((y + 1)*pointCountX + x) * 2 + 0];
			vertIndex++;
			vertices[vertIndex] = point[((y + 1)*pointCountX + x) * 2 + 1];
			vertIndex++;
			vertices[vertIndex] = 0.f;
			vertIndex++;

			//Triangle part 2
			vertices[vertIndex] = point[(y*pointCountX + x) * 2 + 0];
			vertIndex++;
			vertices[vertIndex] = point[(y*pointCountX + x) * 2 + 1];
			vertIndex++;
			vertices[vertIndex] = 0.f;
			vertIndex++;
			vertices[vertIndex] = point[(y*pointCountX + (x + 1)) * 2 + 0];
			vertIndex++;
			vertices[vertIndex] = point[(y*pointCountX + (x + 1)) * 2 + 1];
			vertIndex++;
			vertices[vertIndex] = 0.f;
			vertIndex++;
			vertices[vertIndex] = point[((y + 1)*pointCountX + (x + 1)) * 2 + 0];
			vertIndex++;
			vertices[vertIndex] = point[((y + 1)*pointCountX + (x + 1)) * 2 + 1];
			vertIndex++;
			vertices[vertIndex] = 0.f;
			vertIndex++;
		}
	}

	glGenBuffers(1, &m_VBOGridMesh);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBOGridMesh);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * (pointCountX - 1) * (pointCountY - 1) * 2 * 3 * 3, vertices, GL_STATIC_DRAW);

	delete[] point;
	delete[] vertices;
}

void Renderer::CreateParticleMoveBuffer(int ParticleCount)
{
	random_device seed;
	default_random_engine dre(seed());
	uniform_real_distribution<double> urd(-1, 1);

	// (x, y, z) ������ + (x, y, z) ���� * 6
	m_ParticleMoveCount = ParticleCount * 6 * 6;

	m_ParticleMoveVertex = new float[m_ParticleMoveCount];

	for (int i = 0; i < m_ParticleMoveCount; i += 36)
	{
		float random_x = urd(dre);
		float random_y = urd(dre);
		float random_velocity_x = urd(dre);
		float random_velocity_y = urd(dre);

		// ������ �ﰢ��
		m_ParticleMoveVertex[i] = random_x;
		m_ParticleMoveVertex[i + 1] = random_y;
		m_ParticleMoveVertex[i + 2] = 0.f;
		m_ParticleMoveVertex[i + 3] = random_velocity_x;
		m_ParticleMoveVertex[i + 4] = random_velocity_y;
		m_ParticleMoveVertex[i + 5] = 0.f;

		m_ParticleMoveVertex[i + 6] = random_x + 0.02;
		m_ParticleMoveVertex[i + 7] = random_y;
		m_ParticleMoveVertex[i + 8] = 0.f;
		m_ParticleMoveVertex[i + 9] = random_velocity_x;
		m_ParticleMoveVertex[i + 10] = random_velocity_y;
		m_ParticleMoveVertex[i + 11] = 0.f;

		m_ParticleMoveVertex[i + 12] = random_x + 0.02;
		m_ParticleMoveVertex[i + 13] = random_y - 0.02;
		m_ParticleMoveVertex[i + 14] = 0.f;
		m_ParticleMoveVertex[i + 15] = random_velocity_x;
		m_ParticleMoveVertex[i + 16] = random_velocity_y;
		m_ParticleMoveVertex[i + 17] = 0.f;

		// ������ �ﰢ��
		m_ParticleMoveVertex[i + 18] = random_x + 0.02;
		m_ParticleMoveVertex[i + 19] = random_y - 0.02;
		m_ParticleMoveVertex[i + 20] = 0.f;
		m_ParticleMoveVertex[i + 21] = random_velocity_x;
		m_ParticleMoveVertex[i + 22] = random_velocity_y;
		m_ParticleMoveVertex[i + 23] = 0.f;

		m_ParticleMoveVertex[i + 24] = random_x;
		m_ParticleMoveVertex[i + 25] = random_y - 0.02;
		m_ParticleMoveVertex[i + 26] = 0.f;
		m_ParticleMoveVertex[i + 27] = random_velocity_x;
		m_ParticleMoveVertex[i + 28] = random_velocity_y;
		m_ParticleMoveVertex[i + 29] = 0.f;

		m_ParticleMoveVertex[i + 30] = random_x;
		m_ParticleMoveVertex[i + 31] = random_y;
		m_ParticleMoveVertex[i + 32] = 0.f;
		m_ParticleMoveVertex[i + 33] = random_velocity_x;
		m_ParticleMoveVertex[i + 34] = random_velocity_y;
		m_ParticleMoveVertex[i + 35] = 0.f;
	}

	glGenBuffers(1, &m_VBOParticleMove);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBOParticleMove);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * m_ParticleMoveCount, m_ParticleMoveVertex, GL_STATIC_DRAW);

	delete[] m_ParticleMoveVertex;
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
	if (!success) 
	{
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

float Renderer::elapsedTime = 0.f;

void Renderer::Test()
{
	elapsedTime += 0.001;
	glUseProgram(m_SolidRectShader);

	GLuint uTime = glGetUniformLocation(m_SolidRectShader, "u_Time");
	// 1f : float 1��
	glUniform1f(uTime, elapsedTime);

	int attribPosition = glGetAttribLocation(m_SolidRectShader, "a_Position");
	int attribColor = glGetAttribLocation(m_SolidRectShader, "a_Color");

	// glEnableVertexAttribArray(0) : location 0���� �ִ� a_position�� ����
	glEnableVertexAttribArray(attribPosition);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBORect);
	// Array���� 3���� ��� �ϳ��� ������ �����ض�
	// stride : sizeof(float) * 3 : ���� ������ Ÿ���� ũ�⸦ �ְ�, ������ ���ؾ� ��
	// Attribute 0�� 
	glVertexAttribPointer(attribPosition, 4, GL_FLOAT, GL_FALSE, sizeof(float) * 4, 0);

	// location 1���� ����
	glEnableVertexAttribArray(attribColor);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBOColor);
	glVertexAttribPointer(attribColor, 4, GL_FLOAT, GL_FALSE, sizeof(float) * 4, 0);

	// ���� 6���� �ﰢ������ �׸����� ���
	glDrawArrays(GL_TRIANGLES, 0, 6);

	glDisableVertexAttribArray(attribPosition);
	glDisableVertexAttribArray(attribColor);
}

void Renderer::ParticleRender()
{
	glUseProgram(m_SolidRectShader);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBOParticle);
	// Array���� 3���� ��� �ϳ��� ����
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, 0);
	// ���� ������ ��
	glDrawArrays(GL_TRIANGLES, 0, m_ParticleCount);
	glDisableVertexAttribArray(0);
}

void Renderer::GridMeshRender()
{
	glUseProgram(m_SolidRectShader);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBOGridMesh);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, 0);
	// ���� ������ ��
	glDrawArrays(GL_LINE_STRIP, 0, m_Count_GridMesh);
	
	glDisableVertexAttribArray(0);
}

void Renderer::ParticleMoveRender()
{
	glUseProgram(m_ParticleShaderID);

	GLuint uTime = glGetUniformLocation(m_ParticleShaderID, "u_Time");
	// 1f : float 1��
	glUniform1f(uTime, elapsedTime);
	elapsedTime += 0.0005;
	if (elapsedTime > 2.f)
		elapsedTime = 0.f;

	GLuint aPosition = glGetAttribLocation(m_ParticleShaderID, "a_Position");
	GLuint aVel = glGetAttribLocation(m_ParticleShaderID, "a_Vel");

	glEnableVertexAttribArray(aPosition);
	glEnableVertexAttribArray(aVel);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBOParticleMove);
	// (x, y, z ����x, ����y, ����z), ( . . . )
	glVertexAttribPointer(aPosition, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, 0);
	// aVel�� stride ���������� �ٲپ� �־���� : (GLvoid*)(sizeof(float)*3)
	glVertexAttribPointer(aVel, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, (GLvoid*)(sizeof(float)*3));


	// ���� ������ ��
	glDrawArrays(GL_TRIANGLES, 0, m_ParticleMoveCount);
	glDisableVertexAttribArray(aPosition);
	glDisableVertexAttribArray(aVel);

}