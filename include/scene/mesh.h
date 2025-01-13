// Ilie Dumitru

#ifndef CS_MESH
#define CS_MESH

#include <vector>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include "glm/vec3.hpp"
#include "glm/vec2.hpp"

struct Mesh
{
	GLuint m_VaoId, m_VboId; // For the few models we'll use and the fact that they are small we will just not use an EBO to simplify the code.

	std::vector<glm::vec3> m_vertices;
	std::vector<glm::vec3> m_normals;
	std::vector<glm::vec2> m_uvs;

	Mesh();
	~Mesh();

	void copy(const Mesh& a_mesh);

	void createGLids();

	void loadMesh(const char* a_file);

	void draw() const;

	void clear();
};

#endif // CS_MESH