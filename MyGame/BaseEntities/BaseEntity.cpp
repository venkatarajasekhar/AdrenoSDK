
#include "BaseEntity.h"

BaseEntity::BaseEntity()
	: m_entityType(ENTITY_TYPE_UNDEFINED),
	m_teamType(TEAM_TYPE_UNDEFINED)
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

TEAM_TYPE BaseEntity::getTeamType()const
{
	return m_teamType;
}

void BaseEntity::setTeamType(TEAM_TYPE teamType)
{
	m_teamType = teamType;
}