// Ilie Dumitru

#ifndef CS_SCENE
#define CS_SCENE

#include "glm/glm.hpp"
#include "mesh.h"
#include <vector>
#include <string>
#include <unordered_map>

struct Scene
{
	std::vector<std::string> m_meshes;
	std::vector<std::string> m_textures;
	std::vector<glm::mat4> m_transformations;

	void loadScene(const char* a_file);

	void clear();

	void draw(const std::unordered_map<std::string, Mesh>& a_meshes);
};

#endif // CS_SCENE
