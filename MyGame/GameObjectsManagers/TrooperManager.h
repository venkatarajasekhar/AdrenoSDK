
#include <MyUtils.h>
#include <MyCamera.h>
#include "Trooper.h"

class TrooperManager
{
private:
	int m_type;
	SkinnedMesh2 m_trooper;
	std::map<int, Trooper*> m_listTroopers;
public:
	TrooperManager();
	~TrooperManager();
	void init(
		int type,
		CFrmMesh& mesh,
		FRM_ANIMATION_SET* animationSet,
		CFrmPackedResourceGLES& resource,
		Shader& shader);
	void insertTrooperToList(Trooper* trooper);
	void removeTrooperFromList(Trooper* trooper);
	void update(Timer& timer);
	void render(Camera& camera, Light& light, SpriteBatch& spriteBatch);
	int getNTrooper();
	Trooper* getTrooperById(int id);
	int getIdTrooperToBeat(MyVec3 positionPlayer);
	bool checkTrooperCanMove(MyVec3 positionTrooper, int type);
	void beatTroopers(MyVec3 positionBeat, int dam);
};