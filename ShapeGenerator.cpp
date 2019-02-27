#include "ShapeGenerator.h"
#define NUM_ARRAY_ELEMENTS(a) sizeof(a)/ sizeof(*a)

ShapeGenerator::ShapeGenerator()
{
}


ShapeGenerator::~ShapeGenerator()
{
}

ShapeData ShapeGenerator::makeCube() {
	ShapeData ret;
	Vertex stackVerts[] =
	{	// my test cube
		// Top
		glm::vec3(-1.0f, +1.0f, +1.0f),        // 0
		glm::vec3(+0.0f, +1.0f, +0.0f),        // Normal
		glm::vec2(+0.0f, +1.0f),               // UV

		glm::vec3(+1.0f, +1.0f, +1.0f),        // 1
		glm::vec3(+0.0f, +1.0f, +0.0f),        // Normal
		glm::vec2(+1.0f, +1.0f),               // UV

		glm::vec3(+1.0f, +1.0f, -1.0f),        // 2
		glm::vec3(+0.0f, +1.0f, +0.0f),        // Normal
		glm::vec2(+1.0f, +0.0f),               // UV

		glm::vec3(-1.0f, +1.0f, -1.0f),        // 3
		glm::vec3(+0.0f, +1.0f, +0.0f),        // Normal
		glm::vec2(+0.0f, +0.0f),               // UV


		// Front
		glm::vec3(-1.0f, +1.0f, -1.0f),        // 4
		glm::vec3(+0.0f, +0.0f, -1.0f),        // Normal
		glm::vec2(+0.0f, +1.0f),               // UV

		glm::vec3(+1.0f, +1.0f, -1.0f),        // 5
		glm::vec3(+0.0f, +0.0f, -1.0f),        // Normal
		glm::vec2(+1.0f, +1.0f),               // UV

		glm::vec3(+1.0f, -1.0f, -1.0f),        // 6
		glm::vec3(+0.0f, +0.0f, -1.0f),        // Normal
		glm::vec2(+1.0f, +0.0f),               // UV

		glm::vec3(-1.0f, -1.0f, -1.0f),        // 7
		glm::vec3(+0.0f, +0.0f, -1.0f),        // Normal
		glm::vec2(+0.0f, +0.0f),               // UV
		
		// Right
		glm::vec3(+1.0f, +1.0f, -1.0f),        // 8
		glm::vec3(+1.0f, +0.0f, +0.0f),        // Normal
		glm::vec2(+1.0f, +0.0f),               // UV

		glm::vec3(+1.0f, +1.0f, +1.0f),        // 9
		glm::vec3(+1.0f, +0.0f, +0.0f),        // Normal
		glm::vec2(+0.0f, +0.0f),               // UV

		glm::vec3(+1.0f, -1.0f, +1.0f),        // 10
		glm::vec3(+1.0f, +0.0f, +0.0f),        // Normal
		glm::vec2(+0.0f, +1.0f),               // UV

		glm::vec3(+1.0f, -1.0f, -1.0f),        // 11
		glm::vec3(+1.0f, +0.0f, +0.0f),        // Normal
		glm::vec2(+1.0f, +1.0f),               // UV

		
		// Left
		glm::vec3(-1.0f, +1.0f, +1.0f),        // 12
		glm::vec3(-1.0f, +0.0f, +0.0f),        // Normal
		glm::vec2(+1.0f, +0.0f),               // UV

		glm::vec3(-1.0f, +1.0f, -1.0f),        // 13
		glm::vec3(-1.0f, +0.0f, +0.0f),        // Normal
		glm::vec2(+0.0f, +0.0f),               // UV

		glm::vec3(-1.0f, -1.0f, -1.0f),        // 14
		glm::vec3(-1.0f, +0.0f, +0.0f),        // Normal
		glm::vec2(+0.0f, +1.0f),               // UV

		glm::vec3(-1.0f, -1.0f, +1.0f),        // 15
		glm::vec3(-1.0f, +0.0f, +0.0f),        // Normal
		glm::vec2(+1.0f, +1.0f),               // UV

		
		// Back
		glm::vec3(+1.0f, +1.0f, +1.0f),        // 16
		glm::vec3(+0.0f, +0.0f, +1.0f),        // Normal
		glm::vec2(+1.0f, +0.0f),               // UV

		glm::vec3(-1.0f, +1.0f, +1.0f),        // 17
		glm::vec3(+0.0f, +0.0f, +1.0f),        // Normal
		glm::vec2(+0.0f, +0.0f),               // UV

		glm::vec3(-1.0f, -1.0f, +1.0f),        // 18
		glm::vec3(+0.0f, +0.0f, +1.0f),        // Normal
		glm::vec2(+0.0f, +1.0f),               // UV

		glm::vec3(+1.0f, -1.0f, +1.0f),        // 19
		glm::vec3(+0.0f, +0.0f, +1.0f),        // Normal
		glm::vec2(+1.0f, +1.0f),               // UV

		
										  // Bottom
		glm::vec3(+1.0f, -1.0f, -1.0f),        // 20
		glm::vec3(+0.0f, -1.0f, +0.0f),        // Normal
		glm::vec2(+1.0f, +1.0f),               // UV

		glm::vec3(-1.0f, -1.0f, -1.0f),        // 21
		glm::vec3(+0.0f, -1.0f, +0.0f),        // Normal
		glm::vec2(+0.0f, +1.0f),               // UV

		glm::vec3(-1.0f, -1.0f, +1.0f),        // 22
		glm::vec3(+0.0f, -1.0f, +0.0f),        // Normal
		glm::vec2(+0.0f, +0.0f),               // UV

		glm::vec3(+1.0f, -1.0f, +1.0f),        // 23
		glm::vec3(+0.0f, -1.0f, +0.0f),        // Normal
		glm::vec2(+1.0f, +0.0f),               // UV

	};

	ret.numVertices = NUM_ARRAY_ELEMENTS(stackVerts);
	ret.vertices = new Vertex[ret.numVertices];
	memcpy(ret.vertices, stackVerts, sizeof(stackVerts));

	unsigned short stackIndices[] = {
		0,   1,  2,  0,  2,  3, // Top
		4,   5,  6,  4,  6,  7, // Front
		8,   9, 10,  8, 10, 11, // Right
		12, 13, 14, 12, 14, 15, // Left
		16, 17, 18, 16, 18, 19, // Back
		20, 22, 21, 20, 23, 22, // Bottom
	};
	ret.numIndices = NUM_ARRAY_ELEMENTS(stackIndices);
	ret.indices = new GLushort[ret.numIndices];
	memcpy(ret.indices, stackIndices, sizeof(stackIndices));

	return ret;
}

ShapeData ShapeGenerator::readObj(const char* path) {
	ShapeData ret;
	return ret;
}

ShapeData ShapeGenerator::makefillerQuard()
{
	ShapeData ret;
	Vertex verts[] =
	{	//Main sahpe
		glm::vec3(+20.0f, +20.0f, +0.0f),			// position
		glm::vec3(+0.0f, +0.0f, +1.0f),			// normal
		glm::vec2(+1.0f, +1.0f),					// uv

		glm::vec3(+20.0f, -20.0f, +0.0f),			// position
		glm::vec3(+0.0f, +0.0f, +1.0f),			// normal
		glm::vec2(+1.0f, +0.0f),					// uv

		glm::vec3(-20.0f, +20.0f, +0.0f),			// position
		glm::vec3(+0.0f, +0.0f, +1.0f),			// normal
		glm::vec2(+0.0f, +1.0f),					// uv

		glm::vec3(-20.0f, -20.0f, +0.0f),			// position
		glm::vec3(+0.0f, +0.0f, +1.0f),			// normal
		glm::vec2(+0.0f, +0.0f),					// uv
	};
	ret.numVertices = NUM_ARRAY_ELEMENTS(verts);
	ret.vertices = new Vertex[ret.numVertices];
	memcpy(ret.vertices, verts, sizeof(verts));

	GLushort indices[] = { 0,2,1,2,3,1 };
	ret.numIndices = NUM_ARRAY_ELEMENTS(indices);
	ret.indices = new GLushort[ret.numIndices];
	memcpy(ret.indices, indices, sizeof(indices));

	return ret;
}