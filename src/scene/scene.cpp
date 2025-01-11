// Ilie Dumitru

#include "scene/scene.h"
#include "helpers/utils.h"
#include "helpers/shaders.h"
#include "helpers/constants.h"
#include "glm/gtc/matrix_transform.hpp"
#include <fstream>
#include <cmath>

void Scene::loadScene(const char* a_file)
{
	clear();

	std::ifstream in(SCENES_PATH + "/" + a_file);
	std::string mesh, texture;
	float tx, ty, tz, rx, ry, rz, sx, sy, sz;
	const float PI = atan2(0, -1), DEG2RAD = PI / 180;

	while (in >> mesh >> texture >> tx >> ty >> tz >> rx >> ry >> rz >> sx >> sy >> sz)
	{
		rx *= DEG2RAD;
		ry *= DEG2RAD;
		rz *= DEG2RAD;
		m_meshes.push_back(mesh);
		m_textures.push_back(texture);
		m_transformations.push_back(
			glm::translate(glm::mat4(1.f), glm::vec3(tx, ty, tz))     *
			glm::rotate(glm::mat4(1.f), rx, glm::vec3(1.f, 0.f, 0.f)) *
			glm::rotate(glm::mat4(1.f), ry, glm::vec3(0.f, 0.f, 1.f)) *
			glm::rotate(glm::mat4(1.f), rz, glm::vec3(0.f, 1.f, 0.f)) *
			glm::scale(glm::mat4(1.f), glm::vec3(sx, sy, sz))
		);
	}
}

void Scene::clear()
{
	m_meshes.clear();
	m_textures.clear();
	m_transformations.clear();
}

void Scene::draw(const std::unordered_map<std::string, Mesh>& a_meshes)
{
	Shaders::SetMVP(
		glm::lookAt(Utils::cameraPos, Utils::cameraPos + Utils::cameraOrientation, Utils::cameraVertical),
		glm::infinitePerspective(FOV, GLfloat(Utils::width) / GLfloat(Utils::height), ZNEAR)
	);

	for (int i = 0; i < static_cast<int>(m_meshes.size()); ++i)
	{
		Shaders::SetMeshDefault(m_textures[i], m_transformations[i]);
		a_meshes.find(m_meshes[i])->second.draw();
	}
}
