
#include "FrmPlatform.h"
#include "FrmApplication.h"
#include "MyUtils.h"

#include <stdio.h>
#include <crtdbg.h>

#include <GLEW/glew.h>
#include <FreeGLUT/freeglut.h>

//====================================================================================================================
//
// Global variables
//
//====================================================================================================================

static int g_argc;
static char **g_argv;
CFrmApplication* g_pApplication(nullptr);

//====================================================================================================================
//
// OpenGL functions
//
//====================================================================================================================

static void GLRender()
{
}

static void GLResize(int width, int height)
{
	g_pApplication->m_nWidth = width;
	g_pApplication->m_nHeight = height;

	g_pApplication->Resize();
}

static void GLUpdate()
{
	g_pApplication->Update();
	g_pApplication->Render();

	glutSwapBuffers();
}

// Keyboard

static void GLASCIIKeyDown(unsigned char key, int x, int y)
{
}

static void GLASCIIKeyUp(unsigned char key, int x, int y)
{
}

static void GLSpecialKeyDown(int key, int x, int y)
{
}

static void GLSpecialKeyUp(int key, int x, int y)
{
}

// Mouse

static void GLMouseButton(int button, int state, int x, int y)
{
	// Set pointer position
	FRMVECTOR2 vPointerPosition;
	vPointerPosition.x = +2.0f * x / (FLOAT32)(g_pApplication->m_nWidth - 1) - 1.0f;
	vPointerPosition.y = -2.0f * y / (FLOAT32)(g_pApplication->m_nHeight - 1) + 1.0f;

	g_pApplication->m_Input.m_vPointerPosition = vPointerPosition;

	// Set pointer status
	if (state == GLUT_DOWN)
	{
		g_pApplication->m_Input.m_nPointerState = FRM_INPUT::POINTER_DOWN;
		g_pApplication->m_Input.m_nPointerState |= FRM_INPUT::POINTER_PRESSED;
	}
	else if (state == GLUT_UP)
	{
		g_pApplication->m_Input.m_nPointerState = FRM_INPUT::POINTER_RELEASED;
	}
}

static void GLMouseMove(int x, int y)
{
	FRMVECTOR2 vPointerPosition;
	vPointerPosition.x = +2.0f * x / (FLOAT32)(g_pApplication->m_nWidth - 1) - 1.0f;
	vPointerPosition.y = -2.0f * y / (FLOAT32)(g_pApplication->m_nHeight - 1) + 1.0f;

	g_pApplication->m_Input.m_vPointerPosition = vPointerPosition;

	g_pApplication->m_Input.m_nPointerState = FRM_INPUT::POINTER_DOWN;
}

//====================================================================================================================
//
// CFrmAppContainer declaration
//
//====================================================================================================================

class CFrmAppContainer
{
public:
	CFrmApplication*  m_pApplication;

public:
	CFrmAppContainer();
	~CFrmAppContainer();

	BOOL Run();

};

//====================================================================================================================
//
// CFrmAppContainer implementation
//
//====================================================================================================================

CFrmAppContainer::CFrmAppContainer()
{
	m_pApplication = NULL;
}

CFrmAppContainer::~CFrmAppContainer()
{
	if (m_pApplication)
	{
		m_pApplication->Destroy();
		delete m_pApplication;
	}
}

BOOL CFrmAppContainer::Run()
{
	// Create the Application
	m_pApplication = FrmCreateApplicationInstance();

	if( NULL == m_pApplication )
        return FALSE;

	g_pApplication = m_pApplication;

	// Init FreeGlut here ...

	glutInit(&g_argc, g_argv);
	glutInitContextVersion(2, 0);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(m_pApplication->m_nWidth, m_pApplication->m_nHeight);
	glutCreateWindow(m_pApplication->m_strName);

	// Core callback
	glutDisplayFunc(GLRender);
	glutReshapeFunc(GLResize);
	glutIdleFunc(GLUpdate);

	// Keyboard callback
	glutKeyboardFunc(GLASCIIKeyDown);
	glutKeyboardUpFunc(GLASCIIKeyUp);
	glutSpecialFunc(GLSpecialKeyDown);
	glutSpecialUpFunc(GLSpecialKeyUp);

	// Mouse callback
	glutMouseFunc(GLMouseButton);
	glutMotionFunc(GLMouseMove);          // when the mouse is moved and a button is pressed.
	//glutPassiveMotionFunc(GLMouseMove); // when the mouse is moving but no buttons are pressed.

	// Init Glew here ...

	GLenum glew_status = glewInit();
	throwIfFailed((glew_status == GLEW_OK), "ERROR: Can't initialize GLEW.");
	throwIfFailed((GLEW_VERSION_2_0 == TRUE), "ERROR: Your graphic card does not support OpenGL 2.0.");

	// Initializing application

	throwIfFailed(
		((TRUE == m_pApplication->Initialize()) && (TRUE == m_pApplication->Resize())),
		"Application failed during scene initialization!");

	// Begin main loop here ...
	// Remember to call m_pApplication->Update(); and m_pApplication->Render();

	glutMainLoop();

    return TRUE;
}

//====================================================================================================================
//
// Main entry
//
//====================================================================================================================

int main(int argc, char *argv[])
{
	g_argc = argc;
	g_argv = argv;

	CFrmAppContainer appContainer;
	return (INT32)appContainer.Run();
}