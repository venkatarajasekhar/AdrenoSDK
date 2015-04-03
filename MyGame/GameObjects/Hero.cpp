
#include "Hero.h"

//===================================================================================================================
//
// Hero class
//
//===================================================================================================================

Hero::Hero()
	: m_skinnedMeshIns(nullptr),
	m_stateMachine(nullptr)
{
}

Hero::~Hero()
{
	SAFE_DELETE(m_stateMachine);
}

// Core functions

void Hero::init(
	Adreno::Model* model,
	Adreno::Animation* anim,
	Texture** modelTexture,
	Shader& shader,
	Material& material,
	const MyVec3& pos,
	const MyVec3& rot,
	const MyVec3& scale,
	std::map<MyString, SkinnedMesh1::AnimAction>& animActions,
	FLOAT32 speedFactor)
{
	// Mesh/Appearance elements
	m_skinnedMesh.init(
		model,
		anim,
		modelTexture,
		shader,
		&material,
		&animActions,
		speedFactor);

	m_skinnedMeshIns = SkinnedMesh1::buildSkinnedMeshInstance(pos, rot, scale, "Idle");
	m_skinnedMesh.addInstance(m_skinnedMeshIns);

	// Moving elements
	m_movingEnt.init(m_skinnedMeshIns->Position, m_skinnedMeshIns->Position, m_skinnedMeshIns->Rotation,
		0, 3.0f, 3.0f);

	// States manager
	m_stateMachine = new StateMachine<Hero>(this);
	m_stateMachine->SetCurrentState(HeroState_Idle::instance());
}

void Hero::update(UserInput& userInput, Timer& timer)
{
	// Moving elements
	m_movingEnt.update(timer);

	m_skinnedMeshIns->Position = m_movingEnt.getPos();
	m_skinnedMeshIns->Rotation = m_movingEnt.getRot();

	// States manager
	m_stateMachine->Update();

	// Mesh/Appearance elements
	m_skinnedMesh.update(timer);
}

void Hero::render(Camera& camera, Light& light)
{
	// Mesh/Appearance elements
	m_skinnedMesh.render(camera, &light);
}

StateMachine<Hero>* Hero::getFSM()const
{
	return m_stateMachine;
}

MyVec3 Hero::getPos()
{
	return m_movingEnt.getPos();
}

// Controlling hero
void Hero::changeAnimAction(const MyString& action)
{
	m_skinnedMeshIns->CurrentAction = action;
}

bool Hero::isMoving()
{
	return m_movingEnt.isMoving();
}

//===================================================================================================================
//
// Hero state idle
//
//===================================================================================================================

void HeroState_Idle::Enter(Hero* hero)
{

}

void HeroState_Idle::Execute(Hero* hero)
{
	hero->changeAnimAction("Idle");
	if (hero->isMoving())
	{
		hero->getFSM()->ChangeState(HeroState_Walk::instance());
	}
}

void HeroState_Idle::Exit(Hero* hero)
{

}

//===================================================================================================================
//
// Hero state walk
//
//===================================================================================================================

void HeroState_Walk::Enter(Hero* hero)
{

}

void HeroState_Walk::Execute(Hero* hero)
{
	hero->changeAnimAction("Walk");
	if (!hero->isMoving())
	{
		hero->getFSM()->ChangeState(HeroState_Idle::instance());
	}
}

void HeroState_Walk::Exit(Hero* hero)
{

}