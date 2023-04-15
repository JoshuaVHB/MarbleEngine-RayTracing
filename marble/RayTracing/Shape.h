#pragma once

#include "Ray.h"
#include <glm/glm.hpp>
namespace RT {

	class Shape {



	public:


		virtual bool cast(const Ray& ray, Hit& hitInfos) = 0;


	};

}