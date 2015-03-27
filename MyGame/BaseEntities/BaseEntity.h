
#pragma once

enum ENTITY_TYPE
{
	ENTITY_TYPE_HERO,
	ENTITY_TYPE_PAWN,
	ENTITY_TYPE_TOWER,
	ENTITY_TYPE_PROJECTILE,
};

class BaseEntity
{
public:
	BaseEntity();
	virtual ~BaseEntity();

	ENTITY_TYPE getEntityType()const;
	void setEntityType(ENTITY_TYPE entityType);

protected:
	ENTITY_TYPE m_entityType;
};