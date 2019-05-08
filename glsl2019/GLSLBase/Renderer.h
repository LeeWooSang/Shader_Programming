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

	// 텍스처를 생성해주는 함수
	// => PNG : 투명도가 있는 텍스처
	// => BMP : 투명도가 없는 텍스처
	GLuint CreatePngTexture(char * filePath);
	GLuint CreateBmpTexture(char * filePath);
	   
	void Test();
	void ParticleRender();
	void GridMeshRender();
	void ParticleMoveRender();
	void ParticleStartLifeTimeRender();
	void ParticleFlyRender();
	void ParticleRandomColorRender();
	void RaderRender();
	void FillAll(float);
	void TextureRectRender();
	void TextureCheckerBoardRender();

private:
	void Initialize(int windowSizeX, int windowSizeY);
	bool ReadFile(char* filename, std::string *target);
	void AddShader(GLuint ShaderProgram, const char* pShaderText, GLenum ShaderType);
	GLuint CompileShaders(char* filenameVS, char* filenameFS);
	
	void CreateVertexBufferObjects();
	void CreateParticleBuffer(int);
	void CreateParticleMoveBuffer(int);
	void CreateParticleStartLifeTimeBuffer(int);
	void CreateParticleFlyBuffer(int);
	void CreateParticleRandomColor(int);
	void CreateGridMesh();
	void CreateRader(int);
	void CreateTextureMesh(int);
	void CreateTextureCheckerBoard(int);

	unsigned char* loadBMPRaw(const char * imagepath, unsigned int& outWidth, unsigned int& outHeight);

	bool m_Initialized = false;
	
	unsigned int m_WindowSizeX = 0;
	unsigned int m_WindowSizeY = 0;

	// 셰이더 ID
	GLuint m_SolidRectShader = 0;
	GLuint m_ParticleShaderID = 0;
	GLuint m_ParticleFlyShaderID = 0;
	GLuint m_ParticleRandomColorShaderID = 0;
	GLuint m_RaderShaderID = 0;
	GLuint m_FillAllShaderID = 0;
	GLuint m_TextureRectShaderID = 0;
	GLuint m_TextureCheckerBoardShaderID = 0;

	GLuint m_VBORect = 0;
	GLuint m_VBOColor = 0;
	GLuint m_VBOTriangle = 0;
	GLuint m_VBOParticle = 0;
	GLuint m_VBOGridMesh = 0;
	GLuint m_VBOParticleMove = 0;
	GLuint m_VBOParticleFly = 0;
	GLuint m_VBOParticleRandomColor = 0;
	GLuint m_VBORader = 0;
	GLuint m_VBOTextureRect = 0;

	GLuint m_ParticleTextureID0 = 0;
	GLuint m_CheckerBoardTextureID = 0;
	GLuint m_RGBTextureID = 0;

	float* m_ParticleVertex{ nullptr };
	int m_ParticleCount = 0;

	int m_Count_GridMesh = 0;

	float m_Velocity;
	float* m_ParticleMoveVertex{ nullptr };
	int m_ParticleMoveCount = 0;

	int m_RectCount = 0;
	static float elapsedTime;
};

