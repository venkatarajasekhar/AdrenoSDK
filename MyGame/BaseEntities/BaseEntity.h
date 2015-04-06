
#pragma once

enum ENTITY_TYPE
{
	ENTITY_TYPE_HERO,
	ENTITY_TYPE_PAWN,
	ENTITY_TYPE_TOWER,
	ENTITY_TYPE_PROJECTILE,
};

enum TEAM_TYPE
{
	MY_TEAM,
	ENEMY,
	NEUTRAL,
};

class BaseEntity
{
public:
	BaseEntity();
	virtual ~BaseEntity();

	ENTITY_TYPE getEntityType()const;
	void setEntityType(ENTITY_TYPE entityType);

	TEAM_TYPE getTeamType()const;
	void setTeamType(TEAM_TYPE teamType);

protected:
	ENTITY_TYPE m_entityType;
	TEAM_TYPE m_teamType;
};