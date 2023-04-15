#pragma once

#include "Shape.h"

namespace RT {

	class Plane : public Shape {

	public:

		Plane(const glm::vec3& normal, const glm::vec3& origin) {
			m_normal = normal;
			m_origin = origin;
		}

		virtual bool cast(const Ray& ray, Hit& hitinfo) override {

			float dot = glm::dot(m_normal, ray.direction);
			if (dot > 0.0001f) {

				glm::vec3 a = m_origin - ray.origin;
				hitinfo.hitTime = glm::dot(a, m_normal) / dot;

				if (hitinfo.hitTime >= 0) {

					hitinfo.normal = m_normal;
					hitinfo.position = ray.at(hitinfo.hitTime);
					return true;
				}

			}

			return false;
		}
	public:


		glm::vec3 m_normal;
		glm::vec3 m_origin;


	};
}