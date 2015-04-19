
#pragma once

#include <MyBillboard.h>
#include "BaseEntity.h"
#include "MovingEntity.h"
#include <MyTexture.h>
#include <MySpriteSheet.h>

//=========================================================================================================
//
// Projectile class
//
//=========================================================================================================

enum PROJECTILE_TYPE
{
	PROJECTILE_ENERGY_BALL,
	PROJECTILE_FIRE_BALL,
};

class Projectile : public BaseEntity
{
public:
	Projectile();
	~Projectile();

	void init(Billboard& billboard,
		PROJECTILE_TYPE projectileType,
		std::vector<LivingEntity*>& lEnts);
	/*void init(Billboard& billboard, 
		int idEnemy, 
		int idHero,
		float range,
		int damage,
		TEAM_TYPE teamType);*/
	void update(Timer& timer);
	void render(Camera& camera);

	// Setter
	void setPos(const MyVec3& pos);
	MyVec3 getPos();
	float getRange();
	void setActive(bool active);
	bool getActive();
	void setTarget(const MyVec3& target);
	PROJECTILE_TYPE getProjectileType() const;

private:
	bool m_active;
	Billboard* m_billboard;
	MovingEntity m_movingEntity;
	LivingEntity* m_enemy;
	LivingEntity* m_hero;
	float m_range;
	int m_damage;
	PROJECTILE_TYPE m_projectileType;
	std::vector<LivingEntity*>* m_lEnts;
};

//=========================================================================================================
//
// ProjectilePool class
//
//=========================================================================================================

class ProjectilePool
{
public:
	static const int MAX_NUM_EACH_PROJECTILE = 4;

private:
	enum
	{
		BILLBOARD_ENERGY_BALL,
		BILLBOARD_FIRE_BALL,
		NUM_BILLBOARD,
	};

	enum
	{
		SPRITESHEET_ENERGY_BALL,
		SPRITESHEET_FIRE_BALL,
		NUM_SPRITESHEET,
	};

public:
	ProjectilePool();
	~ProjectilePool();

	void init(Shader& billboardShader, std::vector<LivingEntity*>& lEnts);
	void update(Timer& timer);
	void render(Camera& camera, Light& light);

private:
	Projectile* getFreeSlot(Projectile* container, int size, PROJECTILE_TYPE type);

	void spawnMyTeam();
	void spawnEnemyTeam();

private:
	SpriteSheet m_spriteSheets[NUM_SPRITESHEET];
	Billboard m_billboards[NUM_BILLBOARD];
	Projectile m_projectiles[MAX_NUM_EACH_PROJECTILE];
};