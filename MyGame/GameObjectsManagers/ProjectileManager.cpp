#include "Global.h"
#include "ProjectileManager.h"

ProjectileManager::ProjectileManager()
{
}

ProjectileManager::~ProjectileManager()
{
	for (auto i = m_listProjectiles.begin(); i != m_listProjectiles.end(); i++)
		delete (*i);
}

void ProjectileManager::init()
{

}

void ProjectileManager::insertProjectileToList(Projectile* projectile)
{
	m_listProjectiles.push_back(projectile);
}

void ProjectileManager::removeProjectileFromList(Projectile* projectile)
{
	for (auto i = m_listProjectiles.begin(); i != m_listProjectiles.end(); ++i)
	{
		if (*i == projectile)
		{
			m_listProjectiles.erase(i);
			SAFE_DELETE(projectile);
			//delete instance;
			break;
		}
	}
}

void ProjectileManager::update(UserInput& userInput, Timer& timer, Camera& camera, int width, int height)
{
	auto i = m_listProjectiles.end();
	if (i != m_listProjectiles.begin())
	{
		i--;
		while (true)
		{
			bool flag = false;
			std::vector<Projectile*>::iterator j;

			if (i == m_listProjectiles.begin())
			{
				flag = true;
			}
			else
			{
				j = i;
				j--;
			}

			if (!(*i)->getActive())
				removeProjectileFromList((*i));

			if (flag)
			{
				break;
			}
			else
			{
				i = j;
			}
		}
	}

	for (auto i = m_listProjectiles.begin(); i != m_listProjectiles.end(); i++)
		(*i)->update(timer);
}

void ProjectileManager::render(Camera& camera, Light& light, SpriteBatch& spriteBatch)
{
	for (auto i = m_listProjectiles.begin(); i != m_listProjectiles.end(); i++)
		(*i)->render(camera);
}

int ProjectileManager::getNProjectile()
{
	return m_listProjectiles.size();
}