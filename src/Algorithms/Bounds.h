#pragma once

#include<glm/glm.hpp>

enum class BoundTypes
{
	AABB = 0x00,	//Axis Align Bounding box
	SPHERE = 0x01, 
};

class BoundingRegion
{
public:
	BoundTypes type;

	//sphere values
	glm::vec3 center;
	float radius;

	//bounding box values
	glm::vec3 min;
	glm::vec3 max;

	/*
		Constructors
	*/

	// intialize with type
	BoundingRegion(BoundTypes type = BoundTypes::AABB);

	//intialize as sphere
	BoundingRegion(glm::vec3 center, float raduis);

	//initialize as AABB
	BoundingRegion(glm::vec3 min, glm::vec3 max);

	/*
		calculation values for the regions
	*/

	//center
	glm::vec3 calculateCenter();

	//calculate Dimensions
	glm::vec3 calculateDimensions();

	/*
		testing methods
	*/

	//Determine if points inside
	bool containsPoint(glm::vec3 pt);

	//determine if region inside
	bool containsRegion(BoundingRegion br);

	//determine if region intersects
	bool intersectsWith(BoundingRegion br);
};