
#include "MyCylinder.h"

#pragma region Helpers

static void BuildCylinderTopCap(
	float bottomRadius, 
	float topRadius, 
	float height,
	UINT sliceCount, 
	UINT stackCount, 
	std::vector<PosNorTexVertex>& vertices,
	UIntArray& indices)
{
	UINT baseIndex = (UINT)vertices.size();

	float y = 0.5f*height;
	float dTheta = 2.0f*PI / sliceCount;

	// Duplicate cap ring vertices because the texture coordinates and normals differ.
	for (UINT i = 0; i <= sliceCount; ++i)
	{
		float x = topRadius*cosf(i*dTheta);
		float z = topRadius*sinf(i*dTheta);

		// Scale down by the height to try and make top cap texture coord area
		// proportional to base.
		float u = x / height + 0.5f;
		float v = z / height + 0.5f;

		vertices.push_back(PosNorTexVertex(x, y, z, 0.0f, 1.0f, 0.0f, u, v));
	}

	// Cap center vertex.
	vertices.push_back(PosNorTexVertex(0.0f, y, 0.0f, 0.0f, 1.0f, 0.0f, 0.5f, 0.5f));

	// Index of center vertex.
	UINT centerIndex = (UINT)vertices.size() - 1;

	for (UINT i = 0; i < sliceCount; ++i)
	{
		indices.push_back(centerIndex);
		indices.push_back(baseIndex + i + 1);
		indices.push_back(baseIndex + i);
	}
}

static void BuildCylinderBottomCap(
	float bottomRadius, 
	float topRadius, 
	float height,
	UINT sliceCount, 
	UINT stackCount, 
	std::vector<PosNorTexVertex>& vertices, 
	UIntArray& indices)
{
	// 
	// Build bottom cap.
	//

	UINT baseIndex = (UINT)vertices.size();
	float y = -0.5f*height;

	// vertices of ring
	float dTheta = 2.0f*PI / sliceCount;
	for (UINT i = 0; i <= sliceCount; ++i)
	{
		float x = bottomRadius*cosf(i*dTheta);
		float z = bottomRadius*sinf(i*dTheta);

		// Scale down by the height to try and make top cap texture coord area
		// proportional to base.
		float u = x / height + 0.5f;
		float v = z / height + 0.5f;

		vertices.push_back(PosNorTexVertex(x, y, z, 0.0f, -1.0f, 0.0f, u, v));
	}

	// Cap center vertex.
	vertices.push_back(PosNorTexVertex(0.0f, y, 0.0f, 0.0f, -1.0f, 0.0f, 0.5f, 0.5f));

	// Cache the index of center vertex.
	UINT centerIndex = (UINT)vertices.size() - 1;

	for (UINT i = 0; i < sliceCount; ++i)
	{
		indices.push_back(centerIndex);
		indices.push_back(baseIndex + i);
		indices.push_back(baseIndex + i + 1);
	}
}

#pragma endregion

Cylinder::Cylinder()
{
}

Cylinder::~Cylinder()
{
}

void Cylinder::init(
	Shader& shader,
	Texture* diffuseMap,
	const MyVec3& pos,
	const MyVec3& rot,
	const MyVec3& scale,
	int sliceCount,
	int stackCount)
{
	std::vector<PosNorTexVertex> vertices;
	UIntArray indices;

	float height = 1.0f, topRadius = 1.0f, bottomRadius = 1.0f;

	//
	// Build Stacks.
	// 

	float stackHeight = height / stackCount;

	// Amount to increment radius as we move up each stack level from bottom to top.
	float radiusStep = (topRadius - bottomRadius) / stackCount;

	UINT ringCount = stackCount + 1;

	// Compute vertices for each stack ring starting at the bottom and moving up.
	for (UINT i = 0; i < ringCount; ++i)
	{
		float y = -0.5f*height + i*stackHeight;
		float r = bottomRadius + i*radiusStep;

		// vertices of ring
		float dTheta = 2.0f*PI / sliceCount;
		for (UINT j = 0; j <= sliceCount; ++j)
		{
			PosNorTexVertex vertex;

			float c = cosf(j*dTheta);
			float s = sinf(j*dTheta);

			vertex.pos = MyVec3(r*c, y, r*s);

			vertex.tex.x = (float)j / sliceCount;
			vertex.tex.y = 1.0f - (float)i / stackCount;

			// Cylinder can be parameterized as follows, where we introduce v
			// parameter that goes in the same direction as the v tex-coord
			// so that the bitangent goes in the same direction as the v tex-coord.
			//   Let r0 be the bottom radius and let r1 be the top radius.
			//   y(v) = h - hv for v in [0,1].
			//   r(v) = r1 + (r0-r1)v
			//
			//   x(t, v) = r(v)*cos(t)
			//   y(t, v) = h - hv
			//   z(t, v) = r(v)*sin(t)
			// 
			//  dx/dt = -r(v)*sin(t)
			//  dy/dt = 0
			//  dz/dt = +r(v)*cos(t)
			//
			//  dx/dv = (r0-r1)*cos(t)
			//  dy/dv = -h
			//  dz/dv = (r0-r1)*sin(t)

			// This is unit length.
			float dr = bottomRadius - topRadius;

			MyVec3 T = MyVec3(-s, 0.0f, c);
			MyVec3 B = MyVec3(dr*c, -height, dr*s);
			MyVec3 N = cross(T, B);
			vertex.nor = normalize(N);

			vertices.push_back(vertex);
		}
	}

	// Add one because we duplicate the first and last vertex per ring
	// since the texture coordinates are different.
	UINT ringVertexCount = sliceCount + 1;

	// Compute indices for each stack.
	for (UINT i = 0; i < stackCount; ++i)
	{
		for (UINT j = 0; j < sliceCount; ++j)
		{
			indices.push_back(i*ringVertexCount + j);
			indices.push_back((i + 1)*ringVertexCount + j);
			indices.push_back((i + 1)*ringVertexCount + j + 1);

			indices.push_back(i*ringVertexCount + j);
			indices.push_back((i + 1)*ringVertexCount + j + 1);
			indices.push_back(i*ringVertexCount + j + 1);
		}
	}

	BuildCylinderTopCap(bottomRadius, topRadius, height, sliceCount, stackCount, vertices, indices);
	BuildCylinderBottomCap(bottomRadius, topRadius, height, sliceCount, stackCount, vertices, indices);

	// Init base
	BasicMesh::init(vertices, indices, shader, diffuseMap, pos, rot, scale);
}

void Cylinder::update(Timer& timer)
{
	MyVec3 rot = getRot();
	rot.y += timer.getElapsedTime() * 50;
	rot.x += timer.getElapsedTime() * 30;
	setRot(rot);

	Mesh::update(timer);
}

void Cylinder::render(Camera& camera, Light* light)
{
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);

	m_shader->apply();
	m_shader->setUniform("u_texMat", m_diffuseMap->getTexMat());

	BasicMesh::render(camera, light);

	glDisable(GL_BLEND);
}