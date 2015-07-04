
#include "MyBox.h"

Box::Box()
{
}

Box::~Box()
{
}

void Box::init(
	Shader& shader,
	const MyVec3& pos,
	const MyVec3& rot,
	const MyVec3& scale)
{
	//
	// Create the vertices.
	//
	std::vector<PosNorTexVertex> vertices;
	vertices.resize(24);

	// Fill in the front face vertex data.
	vertices[0] = PosNorTexVertex(-0.5f, +0.5f, +0.5f,  0, 0, 1,  0, 0);
	vertices[1] = PosNorTexVertex(-0.5f, -0.5f, +0.5f,  0, 0, 1,  0, 1);
	vertices[2] = PosNorTexVertex(+0.5f, -0.5f, +0.5f,  0, 0, 1,  1, 1);
	vertices[3] = PosNorTexVertex(+0.5f, +0.5f, +0.5f,  0, 0, 1,  1, 0);

	// Fill in the back face vertex data.
	vertices[4] = PosNorTexVertex(+0.5f, +0.5f, -0.5f,  0, 0, -1,  0, 0);
	vertices[5] = PosNorTexVertex(+0.5f, -0.5f, -0.5f,  0, 0, -1,  0, 1);
	vertices[6] = PosNorTexVertex(-0.5f, -0.5f, -0.5f,  0, 0, -1,  1, 1);
	vertices[7] = PosNorTexVertex(-0.5f, +0.5f, -0.5f,  0, 0, -1,  1, 0);

	// Fill in the top face vertex data.
	vertices[8]  = PosNorTexVertex(-0.5f, +0.5f, -0.5f,  0, 1, 0,  0, 0);
	vertices[9]  = PosNorTexVertex(-0.5f, +0.5f, +0.5f,  0, 1, 0,  0, 1);
	vertices[10] = PosNorTexVertex(+0.5f, +0.5f, +0.5f,  0, 1, 0,  1, 1);
	vertices[11] = PosNorTexVertex(+0.5f, +0.5f, -0.5f,  0, 1, 0,  1, 0);

	// Fill in the bottom face vertex data.
	vertices[12] = PosNorTexVertex(-0.5f, -0.5f, +0.5f,  0, -1, 0,  0, 0);
	vertices[13] = PosNorTexVertex(-0.5f, -0.5f, -0.5f,  0, -1, 0,  0, 1);
	vertices[14] = PosNorTexVertex(+0.5f, -0.5f, -0.5f,  0, -1, 0,  1, 1);
	vertices[15] = PosNorTexVertex(+0.5f, -0.5f, +0.5f,  0, -1, 0,  1, 0);

	// Fill in the left face vertex data.
	vertices[16] = PosNorTexVertex(-0.5f, +0.5f, -0.5f,  -1, 0, 0,  0, 0);
	vertices[17] = PosNorTexVertex(-0.5f, -0.5f, -0.5f,  -1, 0, 0,  0, 1);
	vertices[18] = PosNorTexVertex(-0.5f, -0.5f, +0.5f,  -1, 0, 0,  1, 1);
	vertices[19] = PosNorTexVertex(-0.5f, +0.5f, +0.5f,  -1, 0, 0,  1, 0);

	// Fill in the right face vertex data.
	vertices[20] = PosNorTexVertex(+0.5f, +0.5f, +0.5f,  1, 0, 0,  0, 0);
	vertices[21] = PosNorTexVertex(+0.5f, -0.5f, +0.5f,  1, 0, 0,  0, 1);
	vertices[22] = PosNorTexVertex(+0.5f, -0.5f, -0.5f,  1, 0, 0,  1, 1);
	vertices[23] = PosNorTexVertex(+0.5f, +0.5f, -0.5f,  1, 0, 0,  1, 0);

	//
	// Create the indices.
	//
	UIntArray indices;
	indices.resize(36);

	// Fill in the front face index data
	indices[0] = 0;
	indices[1] = 1;
	indices[2] = 2;

	indices[3] = 0;
	indices[4] = 2;
	indices[5] = 3;

	// Fill in the back face index data
	indices[6] = 4;
	indices[7] = 5;
	indices[8] = 6;

	indices[9] = 4;
	indices[10] = 6;
	indices[11] = 7;

	// Fill in the top face index data
	indices[12] = 8;
	indices[13] = 9;
	indices[14] = 10;

	indices[15] = 8;
	indices[16] = 10;
	indices[17] = 11;

	// Fill in the bottom face index data
	indices[18] = 12;
	indices[19] = 13;
	indices[20] = 14;

	indices[21] = 12;
	indices[22] = 14;
	indices[23] = 15;

	// Fill in the left face index data
	indices[24] = 16;
	indices[25] = 17;
	indices[26] = 18;

	indices[27] = 16;
	indices[28] = 18;
	indices[29] = 19;

	// Fill in the right face index data
	indices[30] = 20;
	indices[31] = 21;
	indices[32] = 22;

	indices[33] = 20;
	indices[34] = 22;
	indices[35] = 23;

	BasicMesh::init(vertices, indices, shader, nullptr, pos, rot, scale);
}

void Box::update(Timer& timer)
{
	MyVec3 rot = getRot();
	rot.y += timer.getElapsedTime() * 50;
	rot.x += timer.getElapsedTime() * 30;
	setRot(rot);

	Mesh::update(timer);
}