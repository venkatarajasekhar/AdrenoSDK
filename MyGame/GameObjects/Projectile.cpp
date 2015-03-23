
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
	m_billboard->setPos(m_movingEntity.getPos());
	m_billboard->render(camera);
}

// Setter

void Projectile::setPos(const MyVec3& pos)
{
	m_movingEntity.setPos(pos);
}

void Projectile::setVelocity(const MyVec3& velocity)
{
	m_movingEntity.setVelocity(velocity);
}