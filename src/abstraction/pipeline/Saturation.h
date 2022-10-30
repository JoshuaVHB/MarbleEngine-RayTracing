#pragma once
#include "VFX.h"

namespace visualEffects {



	class Saturation : public VFX 
	{

	private:

		float m_saturation;


	public:


		Saturation()
			: VFX("Saturation")
		{
			m_saturation = 1.2f;
		}

		virtual void onImGuiRender() override {

			VFX::onImGuiRender();
			if (ImGui::SliderFloat("Saturation", &m_saturation, -1.f, 2.f)) 
				m_blitData.getShader().setUniform1f("u_saturation", m_saturation);
			


		}

		EFFECT_CLASS_TYPE(SaturationEffect);
		

	};
}