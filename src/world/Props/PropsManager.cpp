#include "PropsManager.h"

#include "../../abstraction/UnifiedRenderer.h"
#include "../../vendor/imgui/imgui.h"

namespace World {



void PropsManager::computeToBeRendered(const Renderer::Frustum& viewFrustum) {

	for (const auto& prop : m_props) {

		if (viewFrustum.isOnFrustum(prop.mesh->getBoundingBoxInstance(prop.position, prop.size))) {
			m_toRender.push(prop);
		}


	}

}

void PropsManager::feed(const std::shared_ptr<Renderer::Mesh>&  mesh, const glm::vec3& position, const glm::vec3& size) {

	m_props.emplace_back(Prop{ mesh, position, size });

}

void PropsManager::render(const Renderer::Camera& camera) {

	computeToBeRendered(Renderer::Frustum::createFrustumFromPerspectiveCamera(camera));

	while (!m_toRender.empty()) {

		const Prop& p = m_toRender.front();

		Renderer::renderMesh(camera, p.position, p.size, *p.mesh);

		m_toRender.pop();
	}

}

void PropsManager::onImGuiRender()  {

	ImGui::Begin("Scene props");

	for (unsigned int i = 0; i < m_props.size(); i ++) {

		Prop& p = m_props[i];
		std::stringstream ss;
		ss << "Prop #" << i;
		if (ImGui::CollapsingHeader(ss.str().c_str())) {

			ImGui::DragFloat3("Position##" + i, &p.position.x, 1.F);
			ImGui::DragFloat3("Size##" + i, &p.size.x, 1.F);

		}

	}


}

void PropsManager::clear() {
	m_props.clear();
}

}