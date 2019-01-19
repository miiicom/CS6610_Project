#pragma once
#include <ShapeData.h>
#include <glm\glm.hpp>
class ShapeGenerator
{
public:
	static ShapeData makeCube();
	static ShapeData readObj(const char* path);
	ShapeGenerator();
	~ShapeGenerator();

protected:

};