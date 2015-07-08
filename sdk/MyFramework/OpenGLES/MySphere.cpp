
#include "MySphere.h"

Sphere::Sphere()
{
}

Sphere::~Sphere()
{
}

void Sphere::init(
	Shader& shader,
	Texture* diffuseMap,
	const MyVec3& pos,
	const MyVec3& rot,
	const MyVec3& scale,
	int sliceCount,
	int stackCount,
	MyVec3 distortMod)
{
	m_distortMod = distortMod;
	std::vector<PosNorTexVertex> vertices;
	UIntArray indices;

	float radius = 1.0f;

	//
	// Compute the vertices stating at the top pole and moving down the stacks.
	//

	// Poles: note that there will be texture coordinate distortion as there is
	// not a unique point on the texture map to assign to the pole when mapping
	// a rectangular texture onto a sphere.
	PosNorTexVertex topVertex(0.0f, +radius, 0.0f, 0.0f, +1.0f, 0.0f, 0.0f, 0.0f);
	PosNorTexVertex bottomVertex(0.0f, -radius, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f);
	
	vertices.push_back(topVertex);

	float phiStep = PI / stackCount;
	float thetaStep = 2.0f * PI / sliceCount;

	// Compute vertices for each stack ring (do not count the poles as rings).
	for (UINT i = 1; i <= stackCount - 1; ++i)
	{
		float phi = i*phiStep;

		// Vertices of ring.
		for (UINT j = 0; j <= sliceCount; ++j)
		{
			float theta = j*thetaStep;

			PosNorTexVertex v;

			// spherical to cartesian
			v.pos.x = radius*sinf(phi)*cosf(theta);
			v.pos.y = radius*cosf(phi);
			v.pos.z = radius*sinf(phi)*sinf(theta);

			v.nor = normalize(v.pos);

			v.tex.x = theta / (2.0f * PI);
			v.tex.y = phi / PI;

			vertices.push_back(v);
		}
	}

	vertices.push_back(bottomVertex);

	//
	// Compute indices for top stack.  The top stack was written first to the vertex buffer
	// and connects the top pole to the first ring.
	//

	for (UINT i = 1; i <= sliceCount; ++i)
	{
		indices.push_back(0);
		indices.push_back(i + 1);
		indices.push_back(i);
	}

	//
	// Compute indices for inner stacks (not connected to poles).
	//

	// Offset the indices to the index of the first vertex in the first ring.
	// This is just skipping the top pole vertex.
	UINT baseIndex = 1;
	UINT ringVertexCount = sliceCount + 1;
	for (UINT i = 0; i < stackCount - 2; ++i)
	{
		for (UINT j = 0; j < sliceCount; ++j)
		{
			indices.push_back(baseIndex + i*ringVertexCount + j);
			indices.push_back(baseIndex + i*ringVertexCount + j + 1);
			indices.push_back(baseIndex + (i + 1)*ringVertexCount + j);

			indices.push_back(baseIndex + (i + 1)*ringVertexCount + j);
			indices.push_back(baseIndex + i*ringVertexCount + j + 1);
			indices.push_back(baseIndex + (i + 1)*ringVertexCount + j + 1);
		}
	}

	//
	// Compute indices for bottom stack.  The bottom stack was written last to the vertex buffer
	// and connects the bottom pole to the bottom ring.
	//

	// South pole vertex was added last.
	UINT southPoleIndex = (UINT)vertices.size() - 1;

	// Offset the indices to the index of the first vertex in the last ring.
	baseIndex = southPoleIndex - ringVertexCount;

	for (UINT i = 0; i < sliceCount; ++i)
	{
		indices.push_back(southPoleIndex);
		indices.push_back(baseIndex + i);
		indices.push_back(baseIndex + i + 1);
	}

	// Init base
	BasicMesh::init(vertices, indices, shader, diffuseMap, pos, rot, scale);
}

void Sphere::update(Timer& timer)
{
	/*
	MyVec3 rot = getRot();
	rot.y += timer.getElapsedTime() * 50;
	rot.x += timer.getElapsedTime() * 30;
	setRot(rot);
	/**/

	m_shader->apply();
	m_shader->setUniform("u_timer", timer.getTotalTime());
	m_shader->setUniform("u_distortModifier", m_distortMod);

	Mesh::update(timer);
}

void Sphere::render(Camera& camera, Light* light)
{
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);

	BasicMesh::render(camera, light);

	glDisable(GL_BLEND);
}