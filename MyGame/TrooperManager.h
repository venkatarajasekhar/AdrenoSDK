
#include <MyUtils.h>
#include <MyCamera.h>
#include "Trooper.h"

class TrooperManager
{
public:
	SkinnedMesh2 m_trooper;
	std::map<int, Trooper*> m_listTroopers;
public:
	TrooperManager();
	~TrooperManager();
	void init(
		CFrmMesh& mesh,
		FRM_ANIMATION_SET* animationSet,
		CFrmPackedResourceGLES& resource,
		Shader& shader);
	void insertTrooperToList(Trooper* trooper);
	void removeTrooperFromList(Trooper* trooper);
	void update(Timer& timer);
	void render(Camera& camera, Light& light, SpriteBatch& spriteBatch);
	int getNTrooper();
};