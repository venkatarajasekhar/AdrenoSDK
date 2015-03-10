
#include <MyUtils.h>
#include <MyCamera.h>
#include "Dman.h"

class DmanManager
{
private:
	SkinnedMesh2 m_dman;
	std::map<int, Dman*> m_listDmans;
public:
	DmanManager();
	~DmanManager();
	void init(
		CFrmMesh& mesh,
		FRM_ANIMATION_SET* animationSet,
		CFrmPackedResourceGLES& resource,
		Shader& shader);
	void insertDmanToList(Dman* dman);
	void removeDmanFromList(Dman* dman);
	void update(Timer& timer);
	void render(Camera& camera, Light& light);
	int getNDman();
};