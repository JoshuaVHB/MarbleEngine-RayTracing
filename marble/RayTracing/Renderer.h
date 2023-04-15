#pragma once

#include "../abstraction/Texture.h"
#include "../abstraction/Window.h"

#include <glm/glm.hpp>
#include <glad/glad.h>

#include "Random.h"

#include "Plane.h"
#include "Sphere.h"
namespace RT {


	class RTRenderer {

public:

	glm::vec3 sunPos = { 0,0,0 };

	RTRenderer() {

		m_target = std::make_shared<Renderer::Texture>(Window::getWinWidth(), Window::getWinHeight());
		glTextureStorage2D(m_target->getId(), 1, GL_RGBA16, m_target->getWidth(), m_target->getHeight());
	}


	std::shared_ptr<Renderer::Texture> render() {

		
		for (int y = 0; y < m_height; y++) {
			
			for (int x = 0; x < m_width; x++) {


				glm::vec2 uv = { (float)x / (float)m_width, (float)y / (float)m_height};
				uv = uv * 2.0F - 1.0f;
				pixelBuffer[x + y * m_width] = shader(uv);
			}

		}
	

		m_target->setData(pixelBuffer);
		return m_target;
	}


private:

	uint32_t shader(const glm::vec2& uv) 
	{
		Ray ray = {
			{0,0,0},
			{uv.x, uv.y, -1.0f}
		};

		Hit hitinfos;
		
		if (m_s.cast(ray, hitinfos)) 
		{
			glm::vec4 color = { 1,1,0,1 };
			float shading = std::max(glm::dot(glm::normalize(sunPos), hitinfos.normal), 0.0f);
			color *= shading; color.a = 1;
			return vec4ToUintNormalized(color);
		}

		

		if (m_plane.cast(ray, hitinfos)) {

			return  vec4ToUintNormalized(0.3, 0.3 , 0.3, 1);
		
		}


		return skyGradientColor(uv);
		//return vec4ToUintNormalized(0,0.1, 0.04, 1);
	}

	/* Values between 0->255 */
	uint32_t vec4ToUint(uint8_t r, uint8_t g, uint8_t b, uint8_t a) {

		return 0x00000000 | (a<< 24) | (b << 16) | (g << 8) | r;
	}

	uint32_t vec4ToUintNormalized(float r, float g, float b, float a)
	{

		return vec4ToUint(r*255.0F,g * 255.0F,b * 255.0F,a*255.0F);
	}

	uint32_t vec4ToUintNormalized(glm::vec4 val)
	{

		return vec4ToUint(val.x * 255.0F, val.y * 255.0F, val.z * 255.0F, val.a * 255.0F);
	}

	uint32_t skyGradientColor(glm::vec2 uv) 
	{
		return vec4ToUintNormalized(1 - uv.y, 1 - uv.y, 0.86, 1);
	}


private:

	int m_width = Window::getWinWidth();
	int m_height = Window::getWinHeight();
	const size_t bufSize = Window::getWinHeight() * Window::getWinWidth();

	uint32_t* pixelBuffer = new uint32_t[bufSize];

	std::shared_ptr<Renderer::Texture> m_target;

	std::vector<Shape> m_objects;
	Sphere m_s{ {0,0,-2.0f}, 0.5f };

	public:
	Plane m_plane{ {0,-1,0}, {0,-2.F,0} };
	};

}