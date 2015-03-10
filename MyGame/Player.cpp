#include "Player.h"

MyVec3 PositionDman;
MyVec3 PositionPlayer;

void Player::init(
	CFrmMesh& mesh,
	FRM_ANIMATION_SET* animationSet,
	CFrmPackedResourceGLES& resource,
	Shader& shader,
	const MyVec3& pos,
	const MyVec3& rot,
	const MyVec3& scale)
{
	std::map<MyString, SkinnedMesh2::AnimAction> animationPose =
	{
		{ "Stand", { 0.0f, 0.8f } }, // Stand
		{ "Run", { 1.8f, 2.5f } }, // Run
		{ "Beat", { 3.5f, 7.3f } }, // Beat
	};

	m_helth = 5000;
	m_dam = 70;
	m_pointTouch = pos;

	{
		Material material;

		material.Ambient = MyVec3(0.1f, 0.1f, 0.1f);
		material.Diffuse = MyVec4(1.0f, 1.0f, 1.0f, 1.0f);
		material.Specular = MyVec4(0.4f, 0.4f, 0.4f, 1.0f);
		material.Shininess = 16.0f;

		m_player.init(mesh, animationSet, resource, shader, &material, &animationPose);

		m_instance = SkinnedMesh2::buildSkinnedMeshInstance(pos, rot, scale, "Stand");

		m_player.addInstance(m_instance);
	}

	
	//m_player.setCurrentPose("Stand");
}

void Player::update(UserInput& userInput, Timer& timer, Camera& camera, int width, int height)
{
	MyVec3 position = m_instance->Position; //m_player.getPos();

	MyVec2 pointTouch;
	if (userInput.pointer_Pressing(pointTouch))//.isTouching(pointTouch))
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

		rotatePlayer();
	}

	float moveFactor = 40.0f;

	float e = 0.1f;
	if ((fabs(position.x - m_pointTouch.x) > e) || (fabs(position.z - m_pointTouch.z) > e))
	{
		m_instance->CurrentAction = "Run"; //m_player.setCurrentPose("Run");
		position.x += m_direction.x * timer.getElapsedTime() * moveFactor;
		position.z += m_direction.z * timer.getElapsedTime() * moveFactor;
	}
	else
	{
		if (m_instance->CurrentAction == "Run")
		{
			m_instance->Rotation.y += 180.0f;
		}
			
		m_instance->CurrentAction = "Stand"; //m_player.setCurrentPose("Stand");
		float dist = distance(position, PositionDman);
		if (dist < 2.0f)
			m_instance->CurrentAction = "Beat"; //m_player.setCurrentPose("Beat");
	}
	m_instance->Position = position; //m_player.setPos(position);
	PositionPlayer = m_instance->Position;// m_player.getPos();

	m_player.update(timer);
}

void Player::render(Camera& camera, Light& light)
{
	m_player.render(camera, &light);
}

void Player::rotatePlayer()
{
	MyVec3 position = m_instance->Position;//m_player.getPos();
	MyVec3 dir = position - m_pointTouch;
	MyVec3 baseVec = MyVec3(0, 0, -1);

	dir = normalize(dir);
	float cos = dot(dir, baseVec);

	if (cos < -1)
		cos = -1;
	if (cos > 1)
		cos = 1;

	float angle = dACos(cos);
	if (m_pointTouch.x <= position.x)
	{
		angle = -angle;
	}

	m_instance->Rotation.y = angle - 10;
}

void Player::setHelth(int helth)
{
	m_helth = helth;
}

int Player::getHelth()
{
	return m_helth;
}

void Player::setDam(int dam)
{
	m_dam = dam;
}

int Player::getDam()
{
	return m_dam;
}