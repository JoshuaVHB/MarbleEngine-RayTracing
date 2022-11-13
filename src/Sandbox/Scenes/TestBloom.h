#pragma once

#include "../Scene.h"

#include "../../abstraction/Renderer.h"
#include "../../abstraction/Camera.h"

#include "../../abstraction/pipeline/Bloom.h"
#include "../../world/Light/Light.h"
#include "../../abstraction/pipeline/VFXPipeline.h"

class TestBloomScene : public Scene {
private:


    Renderer::Cubemap  m_skybox;
    Player             m_player;
    Renderer::Mesh m_cubeMesh = Renderer::createCubeMesh(1);
    Renderer::FrameBufferObject m_fbo;
    Renderer::Texture m_target{ Window::getWinWidth(), Window::getWinHeight() };
    Renderer::Texture m_depth = Renderer::Texture::createDepthTexture(Window::getWinWidth(), Window::getWinHeight());
    bool write = false;


    visualEffects::VFXPipeline  m_pipeline{ Window::getWinWidth(), Window::getWinHeight() };



    std::vector<Light> m_lights;
    Renderer::BlitPass m_blit;
    Renderer::BlitPass m_blitFinal;

    BloomRenderer m_bloomRenderer;

    float m_exposure = 1.0f;
    float m_strenght = 0.05f;

    bool m_lightsOn[12] =
    {
        0,0,0,
        0,0,0,
        0,0,0,
        0,0,0
    };

public:
    TestBloomScene() : m_skybox {
        "res/skybox_dbg/skybox_front.bmp", "res/skybox_dbg/skybox_back.bmp",
            "res/skybox_dbg/skybox_left.bmp", "res/skybox_dbg/skybox_right.bmp",
            "res/skybox_dbg/skybox_top.bmp", "res/skybox_dbg/skybox_bottom.bmp"
    }
    {
        m_fbo.setTargetTexture(m_target);
        m_fbo.setDepthTexture(m_depth);
        Renderer::setUniformPointLights(m_lights);
        m_blitFinal.setShader("res/shaders/bloom/finalBloom.fs");
        m_blitFinal.getShader().bind();
        m_blitFinal.getShader().setUniform1i("u_sceneTexture", 0);
        m_blitFinal.getShader().setUniform1i("u_finalBloom", 1);
        m_blitFinal.getShader().unbind();

        //===============//
        /*
        m_pipeline.registerEffect<visualEffects::Saturation>();
        m_pipeline.registerEffect<visualEffects::GammaCorrection>();
        m_pipeline.registerEffect<visualEffects::Contrast>();
        m_pipeline.registerEffect<visualEffects::Sharpness>();

        m_pipeline.setShaderOfEffect(visualEffects::SaturationEffect,       "res/shaders/saturation.fs");
        m_pipeline.setShaderOfEffect(visualEffects::GammaCorrectionEffect,  "res/shaders/gammacorrection.fs");
        m_pipeline.setShaderOfEffect(visualEffects::ContrastEffect,         "res/shaders/contrast.fs");
        m_pipeline.setShaderOfEffect(visualEffects::SharpnessEffect,        "res/shaders/sharpness.fs");
        */

        m_pipeline.registerEffect<visualEffects::Bloom>();
        m_pipeline.sortPipeline();




    }

    ~TestBloomScene()
    {
    }

    void step(float delta) override
    {
        m_player.step(delta);
    }

    void onRender() override
    {
        Renderer::FrameBufferObject::setViewportToWindow();
        
        m_pipeline.bind();

        Renderer::clear();

        Renderer::CubemapRenderer::drawCubemap(m_skybox, m_player.getCamera());

        for (const auto& light : m_lights) {

            Renderer::renderMesh(light.getPosition(), glm::vec3(3), m_cubeMesh, m_player.getCamera());
        }

        Renderer::renderMesh({0,0,0}, glm::vec3(3), m_cubeMesh, m_player.getCamera());
        Renderer::renderMesh({10,0,0}, glm::vec3(3), m_cubeMesh, m_player.getCamera());
        Renderer::renderMesh({0,10,0}, glm::vec3(3), m_cubeMesh, m_player.getCamera());
        Renderer::renderMesh({0,0,10}, glm::vec3(3), m_cubeMesh, m_player.getCamera());


        m_pipeline.unbind();

        m_pipeline.renderPipeline();

    }

    void onImGuiRender() override
    {
        m_pipeline.onImGuiRender();

  

        if (ImGui::SliderFloat("bloomStrength", &m_strenght, 0, 5)) {
            m_blitFinal.getShader().bind();
            m_blitFinal.getShader().setUniform1f("u_bloomStrength", m_strenght);
            m_blitFinal.getShader().unbind();
        }

        if (m_lights.size() < 12) {

            if (ImGui::Button("Generate a light")) 
            {
                m_lights.push_back({});
            }

        }

        for (unsigned int i = 0; i < m_lights.size(); i++) {

            Light& light = m_lights.at(i);

            if (ImGui::Checkbox((std::stringstream{ "Switch n" } << i).str().c_str(), &m_lightsOn[i])) {

                m_lights.at(i).setOn(m_lightsOn[i]);
                Renderer::setUniformPointLights(m_lights);
            }

            if (m_lights.at(i).isOn()) {



                glm::vec3 pos = light.getPosition();
                Light::LightParam params = light.getParams();
                float distance = light.getDistance();

                std::stringstream ss{ std::string() };
                ss << "Light " << i+1;

                if (ImGui::CollapsingHeader(ss.str().c_str())) {

                    if (
                        ImGui::DragFloat3((std::stringstream{ "LightPosition n" } << i).str().c_str(), &pos.x, 2.f) +
                        ImGui::SliderFloat3((std::stringstream{ "Ambiant n" } << i).str().c_str(), &params.ambiant.x, 0, 15) +
                        ImGui::SliderFloat3((std::stringstream{"Diffuse n" } << i).str().c_str(), &params.diffuse.x, 0, 15) +
                        ImGui::SliderFloat3((std::stringstream{"Specular n" } << i).str().c_str(), &params.specular.x, 0, 15) +

                        ImGui::DragFloat((std::stringstream{ "Distance n" } << i).str().c_str(), &distance, 30.f)) {
                 
                            Light l = Light{
                                 pos,
                                 params,
                                 distance,
                                 m_lightsOn[i]
                            };
                            m_lights.at(i) = l;

                            Renderer::setUniformPointLights(m_lights);

                    }

                }
            }


        }

    }
};