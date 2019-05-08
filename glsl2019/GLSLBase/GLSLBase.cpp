#include "stdafx.h"
#include "Dependencies\glew.h"
#include "Dependencies\freeglut.h"
#include "Renderer.h"

Renderer* g_Renderer{ nullptr };

int g_WindowSizeX = 500;
int g_WindowSizeY = 500;

void RenderScene(void)
{
	// 클리어를 일부만 적용할 수 있음
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	
	//g_Renderer->FillAll(0.2);

	// 렌더링
	//g_Renderer->Test();	
	//g_Renderer->ParticleRender();
	//g_Renderer->GridMeshRender();
	//g_Renderer->ParticleMoveRender();
	//g_Renderer->ParticleStartLifeTimeRender();
	//g_Renderer->ParticleFlyRender();
	//g_Renderer->ParticleRandomColorRender();
	//g_Renderer->RaderRender();
	//g_Renderer->TextureRectRender();
	g_Renderer->TextureCheckerBoardRender();

	glutSwapBuffers();
}

void Idle(void)
{
	RenderScene();
}

void MouseInput(int button, int state, int x, int y)
{
	RenderScene();
}

void KeyInput(unsigned char key, int x, int y)
{
	if (key == ESCAPE)
		glutLeaveMainLoop();
	else
		RenderScene();
}

void SpecialKeyInput(int key, int x, int y)
{
	RenderScene();
}

int main(int argc, char **argv)
{
	// Initialize GL things
	glutInit(&argc, argv);
	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(0, 0);
	glutInitWindowSize(g_WindowSizeX, g_WindowSizeY);
	glutCreateWindow("GLSL KPU");

	glewInit();
	if (glewIsSupported("GL_VERSION_4_6"))
		cout << " GL Version is 4.6" << endl;
	else
		cout << "GLEW 4.6 not supported" << endl;

	// Initialize Renderer
	g_Renderer = new Renderer(g_WindowSizeX, g_WindowSizeY);

	glutDisplayFunc(RenderScene);
	glutIdleFunc(Idle);
	glutKeyboardFunc(KeyInput);
	glutMouseFunc(MouseInput);
	glutSpecialFunc(SpecialKeyInput);

	glutMainLoop();

	delete g_Renderer;
}

