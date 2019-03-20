#pragma once

#include <string>
#include <cstdlib>
#include <fstream>
#include <iostream>

#include "Dependencies\glew.h"
#include "Dependencies\wglew.h"
#include "Dependencies\glm/glm.hpp"
#include "Dependencies\glm/gtc/matrix_transform.hpp"
#include "Dependencies\glm/gtx/euler_angles.hpp"

class Renderer
{
public:
	Renderer(int windowSizeX, int windowSizeY);
	~Renderer();

	GLuint CreatePngTexture(char * filePath);
	GLuint CreateBmpTexture(char * filePath);
	   
	void Test();
	void ParticleRender();
	void GridMeshRender();

private:
	void Initialize(int windowSizeX, int windowSizeY);
	bool ReadFile(char* filename, std::string *target);
	void AddShader(GLuint ShaderProgram, const char* pShaderText, GLenum ShaderType);
	GLuint CompileShaders(char* filenameVS, char* filenameFS);
	
	void CreateVertexBufferObjects();
	void CreateParticleBuffer();
	void CreateGridMesh();

	unsigned char* loadBMPRaw(const char * imagepath, unsigned int& outWidth, unsigned int& outHeight);

	bool m_Initialized = false;
	
	unsigned int m_WindowSizeX = 0;
	unsigned int m_WindowSizeY = 0;

	GLuint m_SolidRectShader = 0;

	GLuint m_VBORect = 0;
	GLuint m_VBOTriangle = 0;

	float* m_ParticleVertex{ nullptr };
	int m_ParticleCount = 1000;
	GLuint m_VBOParticle = 0;

	GLuint m_VBOGridMesh = 0;
	int m_Count_GridMesh = 0;
};

