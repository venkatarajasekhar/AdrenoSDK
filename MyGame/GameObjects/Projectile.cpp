
#include "Projectile.h"

Projectile::Projectile()
	: m_billboard(nullptr)
{
}

Projectile::~Projectile()
{
}

void Projectile::init(Billboard& billboard)
{
	m_billboard = &billboard;
}

void Projectile::update(Timer& timer)
{
	m_movingEntity.update(timer);
}

void Projectile::render(Camera& camera)
{
	if (m_active)
	{
		m_billboard->setPos(m_movingEntity.getPos());
		m_billboard->render(camera);
	}
}

// Setter

void Projectile::setPos(const MyVec3& pos)
{
	m_movingEntity.setPos(pos);
}

MyVec3 Projectile::getPos()
{
	return m_movingEntity.getPos();
}

void Projectile::setVelocity(const MyVec3& velocity)
{
	m_movingEntity.setVelocity(velocity);
}

void Projectile::setActive(bool active)
{
	m_active = active;
}