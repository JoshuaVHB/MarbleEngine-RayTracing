#pragma once

#include "Shape.h"

namespace RT {

	class Sphere : public Shape 
	{
	private:

		glm::vec3 m_origin;
		float m_radius;


	public:

		Sphere(const glm::vec3& origin, float radius)
		{
			m_radius = radius;
			m_origin = origin;
		}

		virtual bool cast(const Ray& ray, Hit& hitInfos) override 
		{
			float a = glm::dot(ray.direction, ray.direction);
			float b = 2.f * glm::dot(ray.direction, ray.origin - m_origin);
			float c = glm::dot(ray.origin - m_origin, ray.origin - m_origin) - m_radius * m_radius;

			float delta = b * b - 4.0f * a * c;

			if (delta >= 0) {

				glm::vec2 solutions = {
					(-b + sqrt(delta)) / (2.0f * a),
					(-b - sqrt(delta)) / (2.0f * a)
				};

				float closest = std::min(solutions.x, solutions.y);

				hitInfos.position = ray.at(closest);
				hitInfos.normal = glm::normalize(hitInfos.position - m_origin);
				hitInfos.hitTime = closest;
				return true;
			}
			return false;
		};

	};

}