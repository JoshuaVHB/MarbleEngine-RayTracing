#pragma once

#include <vector> 
#include <array>
#include <functional>

#include <string>
#include <sstream>

#include <map>
#include "VFX.h"

#include "Bloom.h"
#include "Sharpness.h"
#include "Saturation.h"
#include "Contrast.h"
#include "GammaCorrection.h"
#include "Tonemapper.h"


namespace visualEffects {

	class VFXPipeline {

		
	private:
		
		PipelineContext m_context ;

		Renderer::BlitPass	m_blitData;
		std::vector< VFX* > m_effects;

	public:
		

		VFXPipeline(int w, int h) {

			m_context.targetTexture = Renderer::Texture(w,h);
			m_context.originTexture = Renderer::Texture(w,h);
			m_context.depthTexture = Renderer::Texture::createDepthTexture(w,h);

			m_context.fbo.setTargetTexture(m_context.targetTexture);
			m_context.fbo.setDepthTexture(m_context.depthTexture);

		}


		template<typename T> 
		void addContextParam(const T& param, const std::string& name) {
			m_context.m_params[name] =  (void*)(&param);
		}
		
		/* GETS AND SETS */
		template<typename T>
		T& getContextParam(const std::string& key) {
			return *(T*)m_context.m_params.at(key);
		}

		template<typename T>
		void setContextParam(const std::string& key, const T& value) {
			m_context.m_params[key] =(void*) & value;
		}


		void setTargetTexture(Renderer::Texture&& texture) {
				
				m_context.targetTexture = std::move(texture);
			
		}


		void setShaderOfEffect(EffectType type,const std::filesystem::path& fragmentShader) {

			for (const auto& effect : m_effects) {

				if (effect->getType() == type) {

					effect->setFragmentShader(fragmentShader);

				}


			}

		}
		void bind() {
			m_context.fbo.setTargetTexture(m_context.originTexture);
			m_context.fbo.bind();
			Renderer::FrameBufferObject::setViewportToTexture(m_context.originTexture);
		}

		void unbind() const {
			m_context.fbo.unbind();
			Renderer::FrameBufferObject::setViewport(Window::getWinWidth(), Window::getWinHeight());
		}


		template<typename T> 
		void registerEffect() {

			T* ptr = new T;
			m_effects.push_back( ptr  );
			std::cout << "Registered an effect : " << ((VFX*)ptr)->getName() << std::endl;

		}



		/*
		Sorts the effect list according to the defined order in VFX.h
		*/ 
		
		void sortPipeline() {

			std::array<int, 9> indicies = { -1, -1, -1, -1, -1, -1, -1 ,-1, -1};
			std::vector<VFX*> temp;

			int index = 0;
			for (VFX* effect : m_effects) {
				
				int i = effect->getType();
				indicies[i] = index; // this implies that there is only one type of each effect
				index++;
			}

			for (const auto& position : indicies) {
				if (position > -1)
					temp.push_back(m_effects.at(position));

			}

			m_effects.swap(temp);

		}


		void renderPipeline()  {

			for (VFX* effect : m_effects) {

				if (!effect->isEnabled()) continue;


				effect->applyEffect(m_context);

				// Recycle textures
				std::swap(m_context.originTexture, m_context.targetTexture);
				
				
				 
				
			}
			
			m_blitData.doBlit(m_context.originTexture);

		}

		void onImGuiRender() {
			if(ImGui::Begin("Effects")) {
			  for (VFX* effect : m_effects)
				  effect->onImGuiRender();
			}
			ImGui::End();
		}

		Renderer::Texture &getTargetTexture() {
			return m_context.targetTexture;
		}

		//-------- Helpers

		void printCurrentEffectOrder() const {

			for (VFX* effect : m_effects)
				std::cout << effect->getName() << std::endl;
		}
	};


}