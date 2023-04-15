#pragma once

#include "../Scene.h"
#include "../../RayTracing/Renderer.h"

#include <random>
#include <chrono>

using namespace std::chrono;
class RayTracingScene : public Scene {
private:

	Renderer::BlitPass m_blit;

	
	float elapsed = 0.F;

	std::chrono::time_point<std::chrono::high_resolution_clock> m_Start;
	std::shared_ptr < Renderer::Texture> m_tex;
	RT::RTRenderer m_renderer;

public:
	RayTracingScene()
	{
		
	}

	void step(float delta) override
	{
		m_Start = std::chrono::high_resolution_clock::now();


		m_tex = m_renderer.render();
		m_tex->bind();
		m_blit.doBlit();

		elapsed = std::chrono::duration_cast<nanoseconds>(high_resolution_clock::now() - m_Start).count() * 0.001f * 0.001f * 0.001f;
		elapsed *= 1000.0f;


	}

	void onRender() override
	{
		Renderer::clear();
		m_blit.doBlit();
	}

	void onImGuiRender() override
	{
		ImGui::Text("Elapsed time : %.3f", elapsed);
		ImGui::DragFloat3("sunPos", &m_renderer.sunPos.x);
		ImGui::DragFloat3("plane", &m_renderer.m_plane.m_origin.x, 0.05f);
	}

	CAMERA_NOT_DEFINED()


};

