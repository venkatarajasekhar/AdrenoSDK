#pragma once

#include <MyUtils.h>
#include <MyCamera.h>
#include "Projectile.h"

class ProjectileManager
{
private:
	std::vector<Projectile*> m_listProjectiles;
public:
	ProjectileManager();
	~ProjectileManager();
	void init();
	void insertProjectileToList(Projectile* projectile);
	void removeProjectileFromList(Projectile* projectile);
	void update(UserInput& userInput, Timer& timer, Camera& camera, int width, int height);
	void render(Camera& camera, Light& light, SpriteBatch& spriteBatch);
	int getNProjectile();
};