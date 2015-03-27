
#include "BaseEntity.h"

BaseEntity::BaseEntity()
{
}

BaseEntity::~BaseEntity()
{
}

ENTITY_TYPE BaseEntity::getEntityType()const
{
	return m_entityType;
}

void BaseEntity::setEntityType(ENTITY_TYPE entityType)
{
	m_entityType = entityType;
}