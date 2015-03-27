#include "ScorpionHero.h"
MyVec3 PositionPlayer;

void ScorpionHero::init(
	Adreno::Model* model,
	Adreno::Animation* animation,
	FileMesh1::MeshTextures& textures,
	Shader& shader,
	const MyVec3& pos,
	const MyVec3& rot,
	const MyVec3& scale,
	BloodBar* bloodBar,
	Billboard& billboard)
{
	std::map<MyString, SkinnedMesh1::AnimAction> animationPose =
	{
		{ "Stand", { 85, 65 } }, // Stand
		{ "Run", { 10, 25 } }, // Run
		{ "Beat1", { 35, 60 } }, // Beat1
		{ "Beat2", { 150, 55 } }, // Beat2
		{ "Defence", { 195, 50 } }, // Defence
		{ "Dead", { 245, 0 } }, // Dead
	};

	m_health = MaxHealthScorpion;
	m_dam = 20;
	m_pointTouch = pos;

	{
		Material material;

		material.Ambient = MyVec3(0.05f, 0.05f, 0.05f);
		material.Diffuse = MyVec4(1.0f, 0.5f, 0.5f, 1.0f);
		material.Specular = MyVec4(0.5f, 0.5f, 0.5f, 1.0f);
		material.Shininess = 16.0f;

		m_player.init(model, animation, textures.Textures, shader, &material, &animationPose);

		m_instance = SkinnedMesh1::buildSkinnedMeshInstance(pos, rot, scale, "Stand");

		m_player.addInstance(m_instance);
	}

	m_bloodBar = bloodBar;

	m_projectile.init(billboard);
}

void ScorpionHero::update(UserInput& userInput, Timer& timer, Camera& camera, int width, int height)
{
	if (m_idEmemy != -1)
		if (g_dmanManager.getTrooperById(m_idEmemy)->getIsDeleted()) m_idEmemy = -1;

	if (m_isUsingSkill)
	{
		g_dmanManager.beatTroopers(m_projectile.getPos(), 3*m_dam);

		int nFrame = m_instance->LeftFrame;
		if (nFrame - 150 == 34)
		{
			MyVec3 dir(dSin(m_instance->Rotation.y), 0, dCos(m_instance->Rotation.y));
			{
				MyVec3 offset = normalize(dir) + MyVec3(0, 3, 0);
				m_projectile.setPos(PositionPlayer + 0.5f*offset);
			}

			{
				MyVec3 vel = normalize(dir) * 7.0f;
				m_projectile.setVelocity(vel);
			}

			m_projectile.setActive(true);
		}

		if (nFrame - 150 == 53)
			m_isUsingSkill = false;
	}
	
	MyVec3 position = m_instance->Position;

	MyVec2 pointTouch;
	if (userInput.pointer_Releasing(pointTouch))
	{
		Plane terrainPlane = { 0.0f, MyVec3(0, 1, 0) };
		Ray ray = createRayInWorld(pointTouch, width, height, camera.getView(), camera.getProj());

		MyVec3 pointTouchWorld = intersect(ray, terrainPlane);

		m_pointTouch = pointTouchWorld;
		float x = m_pointTouch.x - position.x;
		float y = m_pointTouch.y - position.y;
		float z = m_pointTouch.z - position.z;
		float t = max(fabs(x), fabs(z)) * 10;
		m_direction = MyVec3(x / t, y / t, z / t);

		rotatePlayer(m_pointTouch);
	}

	float moveFactor = 40.0f;

	float e = 0.1f;
	if ((fabs(position.x - m_pointTouch.x) > e) || (fabs(position.z - m_pointTouch.z) > e))
	{
		if (!m_isUsingSkill) m_instance->CurrentAction = "Run";
		position.x += m_direction.x * timer.getElapsedTime() * moveFactor;
		position.z += m_direction.z * timer.getElapsedTime() * moveFactor;
	}
	else
	{
		if (!m_isUsingSkill) m_instance->CurrentAction = "Stand";
		if (m_idEmemy == -1) m_idEmemy = findTrooperToBeat();
		if (m_idEmemy != -1)
		{
			rotatePlayer(g_dmanManager.getTrooperById(m_idEmemy)->getTrooper()->Position);
			if (!m_isUsingSkill) m_instance->CurrentAction = "Beat1";
			m_countTime += timer.getElapsedTime();

			if (m_countTime >= 1.5f)
			{
				Trooper* trooper = g_dmanManager.getTrooperById(m_idEmemy);
				trooper->setHealth(trooper->getHealth() - m_dam);
				m_countTime = 0;
			}
		}
	}
	m_instance->Position = position;
	PositionPlayer = m_instance->Position;

	m_player.update(timer);
	m_projectile.update(timer);
}

int ScorpionHero::findTrooperToBeat()
{
	MyVec3 position = m_instance->Position;
	int idTrooper = g_dmanManager.getIdTrooperToBeat(position);
	return idTrooper;
}

void ScorpionHero::render(Camera& camera, Light& light, SpriteBatch& spriteBatch)
{
	m_player.render(camera, &light);
	m_bloodBar->render(spriteBatch, camera, m_instance->Position + MyVec3(-1, 2.5, 0), m_health / (float)MaxHealthScorpion);
	m_projectile.render(camera);
}

void ScorpionHero::rotatePlayer(MyVec3 pointDestination)
{
	MyVec3 position = m_instance->Position;
	MyVec3 dir = position - pointDestination;
	MyVec3 baseVec = MyVec3(0, 0, -1);

	dir = normalize(dir);
	float cos = dot(dir, baseVec);

	if (cos < -1)
		cos = -1;
	if (cos > 1)
		cos = 1;

	float angle = dACos(cos);
	if (pointDestination.x <= position.x)
	{
		angle = -angle;
	}

	m_instance->Rotation.y = angle - 0;
}

void ScorpionHero::setHealth(int helth)
{
	m_health = helth;
}

int ScorpionHero::getHealth()
{
	return m_health;
}

void ScorpionHero::setDam(int dam)
{
	m_dam = dam;
}

int ScorpionHero::getDam()
{
	return m_dam;
}

void ScorpionHero::projectile()
{
	m_instance->setActionAndReset("Beat2");
	m_isUsingSkill = true;
}