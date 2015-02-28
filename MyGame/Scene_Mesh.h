
#ifndef SCENE_H
#define SCENE_H

class CSample : public CFrmApplication
{
public:

	CSample( const CHAR* strName );

    virtual BOOL Initialize();
    virtual BOOL Resize();
    virtual VOID Update();
    virtual VOID Render();
    virtual VOID Destroy();

private:

    // Framework classes
	PerspectiveCamera m_camera_main;
	Timer m_timer;

	Adreno::Model*        m_pModel;
	Adreno::Animation*    m_pAnim;

	CFrmMesh m_mesh_boy;
	FRM_ANIMATION_SET* m_animBoy;

	CFrmMesh m_mesh_dman;
	FRM_ANIMATION_SET* m_anim_dman;

	Texture**         m_pModelTexture;

	Shader m_shader_mesh;
	Shader m_shader_skinnedMesh;
	Shader m_shader_skinnedMesh2;

	//FileMesh1 m_mesh_scorpion;
	SkinnedMesh1 m_skinnedMesh_scorpion;
	//SkinnedMesh2 m_skinnedMesh_boy;
	//SkinnedMesh2 m_skinnedMesh_dman;
};

#endif // SCENE_H
