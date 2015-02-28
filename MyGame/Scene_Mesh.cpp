
#include <FrmPlatform.h>
#include <FrmApplication.h>
#include <OpenGLES/FrmMesh.h>
#include <OpenGLES/FrmShader.h>
#include <OpenGLES/FrmPackedResourceGLES.h>
#include <OpenGLES/FrmUtilsGLES.h>
#include <Model/FrmModelIO.h>

#include <MyPerspectiveCamera.h>
#include <MyFileMesh1.h>
#include <MySkinnedMesh1.h>
#include <MySkinnedMesh2.h>
#include <MyVertex.h>

#include "Scene_Mesh.h"


static const MyVec3 MAIN_CAM_POS = MyVec3(10.0f, 15.0f, 25.0f);
static const MyVec3 MAIN_CAM_TARGET = MyVec3(0.0f, 5.0f, 0.0f);
/**/

/*
static const MyVec3 MAIN_CAM_POS = MyVec3(2.0f, 1.3f, 3.0f);
static const MyVec3 MAIN_CAM_TARGET = MyVec3(0.0f, 1.0f, 0.0f);
/**/

static const float MAIN_CAM_FAR = 200.0f;

//--------------------------------------------------------------------------------------
// Name: FrmCreateApplicationInstance()
// Desc: Global function to create the application instance
//--------------------------------------------------------------------------------------
CFrmApplication* FrmCreateApplicationInstance()
{
    return new CSample( "Skinning2" );
}

MyString resolveAssetsPath(const MyString& relativePath)
{
#ifdef WIN32
	return "Assets/" + relativePath;
	//#elif defined __ANDROID__
#endif
}

CSample::CSample( const CHAR* strName ) 
	: CFrmApplication( strName )
{
}

//--------------------------------------------------------------------------------------
// Name: Initialize()
// Desc: 
//--------------------------------------------------------------------------------------
BOOL CSample::Initialize()
{
	m_camera_main.init(MAIN_CAM_POS, MAIN_CAM_TARGET, 45.0f, 0.1f, MAIN_CAM_FAR);

    // Load model
	m_pModel = Adreno::FrmLoadModelFromFile( "Assets/Meshes/scorpion.model" );
	if( m_pModel == NULL )
	{
		return FALSE;
	}

	m_mesh_boy.Load("Assets/Meshes/Boy03.mesh");
	m_mesh_dman.Load("Assets/Meshes/Dman.mesh");

	// Load animation
	m_pAnim = Adreno::FrmLoadAnimationFromFile("Assets/Meshes/scorpion.anim");
	if (m_pAnim == NULL)
	{
		return FALSE;
	}

	FrmReadAnimation("Assets/Meshes/Boy03.anim", &m_animBoy);
	FrmReadAnimation("Assets/Meshes/Dman.anim", &m_anim_dman);

	// Load model textures
	{
		CFrmPackedResourceGLES resource;
		if (FALSE == resource.LoadFromFile("Assets/Textures/Scorpion.pak"))
		{
			return FALSE;
		}
		m_pModelTexture = FileMesh1::initTextures(m_pModel, resource);
	}
	

	
	m_shader_mesh.init(
		"Assets/Shaders/mesh.vs", 
		"Assets/Shaders/mesh.fs", 
		PosNorTexVertex::ShaderAttribsDesc,
		PosNorTexVertex::NumShaderAttribsDesc);

	m_shader_skinnedMesh.init(
		"Assets/Shaders/skinnedMesh1.vs",
		"Assets/Shaders/skinnedMesh1.fs",
		SkinnedVertex::ShaderAttribsDesc,
		SkinnedVertex::NumShaderAttribsDesc);

	m_shader_skinnedMesh2.init(
		"Assets/Shaders/skinnedMesh2.vs",
		"Assets/Shaders/skinnedMesh2.fs",
		SkinnedVertex::ShaderAttribsDesc,
		SkinnedVertex::NumShaderAttribsDesc);

	Material material;

	material.Ambient = MyVec3(0.05f, 0.05f, 0.05f);
	material.Diffuse = MyVec4(1.0f, 0.5f, 0.5f, 1.0f);
	material.Specular = MyVec4(0.5f, 0.5f, 0.5f, 1.0f);
	material.Shininess = 16.0f;

	//m_mesh_scorpion.init(m_pModel, m_pModelTexture, m_shader_mesh, material, MyVec3(), MyVec3(), MyVec3(1));

	m_skinnedMesh_scorpion.init(m_pModel, m_pAnim, m_pModelTexture, m_shader_skinnedMesh, material, MyVec3(), MyVec3(), MyVec3(1));

	{
		/*
		CFrmPackedResourceGLES resource;
		if (FALSE == resource.LoadFromFile("Assets/Textures/Boy03.pak"))
		{
			return FALSE;
		}
		m_skinnedMesh_boy.init(m_mesh_boy, m_animBoy, resource, m_shader_skinnedMesh2, MyVec3(0), MyVec3(0), MyVec3(1));
		/**/
		CFrmPackedResourceGLES resource;
		if (FALSE == resource.LoadFromFile("Assets/Textures/Dman.pak"))
		{
			return FALSE;
		}
		//m_skinnedMesh_dman.init(m_mesh_dman, m_anim_dman, resource, m_shader_skinnedMesh2, MyVec3(0), MyVec3(0), MyVec3(1));
	}

	// Successful
    return TRUE;
}


//--------------------------------------------------------------------------------------
// Name: Resize()
// Desc: 
//--------------------------------------------------------------------------------------
BOOL CSample::Resize()
{
	m_camera_main.resize(m_nWidth, m_nHeight);

    // Initialize the viewport
    glViewport( 0, 0, m_nWidth, m_nHeight );

    return TRUE;
}


//--------------------------------------------------------------------------------------
// Name: Update()
// Desc: Use the timer and input to update objects in the scene
//--------------------------------------------------------------------------------------
VOID CSample::Update()
{
	m_timer.update();
	m_camera_main.update();

	//m_mesh_scorpion.update();
	m_skinnedMesh_scorpion.update(m_timer);
	//m_skinnedMesh_boy.update(m_timer);
	//m_skinnedMesh_dman.update(m_timer);
}


//--------------------------------------------------------------------------------------
// Name: Render()
// Desc: Render the scene
//--------------------------------------------------------------------------------------
VOID CSample::Render()
{
    // Set default states
    glEnable( GL_DEPTH_TEST );
    glEnable( GL_CULL_FACE );

    // Clear the backbuffer and depth-buffer
    glClearColor( 0.5f, 0.5f, 0.5f, 0.0f );
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT );

	Light light;

	light.LightPos = MyVec3(50.0f, 50.0f, 50.0f);

    // Draw the skinned mesh
	//m_mesh_scorpion.render(m_camera_main, light);
	m_skinnedMesh_scorpion.render(m_camera_main, light);
	//m_skinnedMesh_boy.render(m_camera_main);
	//m_skinnedMesh_dman.render(m_camera_main);
}



//--------------------------------------------------------------------------------------
// Name: Destroy()
// Desc: Destroy objects created by the sample
//--------------------------------------------------------------------------------------
VOID CSample::Destroy()
{
	

	// Release model textures
	FileMesh1::destroyTextures(m_pModel, m_pModelTexture);

	// Destroy model
	if ( m_pModel )
	{
		Adreno::FrmDestroyLoadedModel( m_pModel );
	}

	// Destroy animation
	if (m_pAnim)
	{
		Adreno::FrmDestroyLoadedAnimation(m_pAnim);
	}

	delete m_animBoy;
	delete m_anim_dman;
}