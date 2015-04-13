
#pragma once

enum ENTITY_TYPE
{
	ENTITY_TYPE_UNDEFINED,
	ENTITY_TYPE_HERO,
	ENTITY_TYPE_PAWN,
	ENTITY_TYPE_TOWER,
	ENTITY_TYPE_PROJECTILE,
};

enum TEAM_TYPE
{
	TEAM_TYPE_UNDEFINED,
	TEAM_TYPE_MY_TEAM,
	TEAM_TYPE_ENEMY,
	TEAM_TYPE_NEUTRAL,
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