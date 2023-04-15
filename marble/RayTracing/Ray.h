#pragma once

#include <glm/glm.hpp>

struct Ray {

	glm::vec3 origin;
	glm::vec3 direction;

	glm::vec3 at(float t) const {
		return origin + direction * t;
	}

};


struct Hit {

	glm::vec3 position;
	glm::vec3 normal;
	float hitTime;

};