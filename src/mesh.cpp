// Ilie Dumitru

#include "mesh.h"
#include "glm/glm.hpp"
#include <string>
#include <fstream>
#include <iostream>
#include <sstream>

Mesh::Mesh() :
	m_VaoId{ 0 },
	m_VboId{ 0 },
	m_vertices{},
	m_normals{},
	m_uvs{}
	{}

Mesh::~Mesh()
{
	clear();
}

void Mesh::copy(const Mesh& a_mesh)
{
	clear();

	m_vertices = a_mesh.m_vertices;
	m_normals = a_mesh.m_normals;
	m_uvs = a_mesh.m_uvs;

	createGLids();
}

void Mesh::createGLids()
{
	glGenVertexArrays(1, &m_VaoId);
	glBindVertexArray(m_VaoId);

	glGenBuffers(1, &m_VboId);
	glBindBuffer(GL_ARRAY_BUFFER, m_VboId);
	glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(glm::vec3) + m_normals.size() * sizeof(glm::vec3) + m_uvs.size() * sizeof(glm::vec2), NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, m_vertices.size() * sizeof(glm::vec3), &m_vertices[0]);
    glBufferSubData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(glm::vec3), m_normals.size() * sizeof(glm::vec3), &m_normals[0]);
    glBufferSubData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(glm::vec3) + m_normals.size() * sizeof(glm::vec3), m_uvs.size() * sizeof(glm::vec2), &m_uvs[0]);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid*)(m_vertices.size() * sizeof(glm::vec3)));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, (GLvoid*)(m_vertices.size() * sizeof(glm::vec3) + m_normals.size() * sizeof(glm::vec3)));
}

void Mesh::loadMesh(const char* a_file)
{
	clear();

	std::string row;
	std::ifstream in(a_file);

	std::vector<glm::vec3> temp_vertices;
	std::vector<glm::vec3> temp_normals;
	std::vector<glm::vec2> temp_uvs;

	while (std::getline(in, row))
	{
		// Ignore comment
		size_t commentStart = row.find("#");
		if (commentStart != std::string::npos)
			row.resize(commentStart);

		if (row.empty())
			continue;

		if (row[0] == 'v')
		{
			if (row[1] == ' ')
			{
				// Geometric vertex
				std::stringstream reader;
				float x, y, z;

				reader << (row.c_str() + 2);
				reader >> x >> y >> z;
				temp_vertices.push_back(glm::vec3(x, y, z));
			}
			else if (row[1] == 'p')
			{
				// Parametric space vertex
				// Don't think we'll need this
				std::cerr << "vp found in obj file \"" << a_file << "\".\n";
			}
			else if (row[1] == 'n')
			{
				// Vertex normal
				std::stringstream reader{ row.c_str() + 3 };
				float x, y, z;

				reader >> x >> y >> z;
				temp_normals.push_back(glm::normalize(glm::vec3(x, y, z)));
			}
			else if (row[1] == 't')
			{
				// Vertex texture
				std::stringstream reader{ row.c_str() + 3 };
				float u = 0.f, v = 0.f;

				reader >> u >> v;
				temp_uvs.push_back(glm::vec2(u, v));
			}
		}
		else if (row[0] == 'f')
		{
			// Face
			// At least 3 vertices
			int cntSlashes = 0, cntSpaces = 0;

			for (char c : row)
			{
				if (c == '/')
					++cntSlashes;
				else if (c == ' ')
					++cntSpaces;
			}

			if (cntSlashes % cntSpaces || cntSpaces < 3)
				std::cerr << "Face data malformed.\n" << row << "\n";
			else
			{
				if (cntSlashes == 0)
				{
					// Only vertex ids
					std::stringstream reader{ row.c_str() + 2 };
					int a, b, c;

					reader >> a >> b;
					--a;
					--b;
					while (reader >> c)
					{
						--c;
                        m_vertices.push_back(temp_vertices[a]);
                        m_vertices.push_back(temp_vertices[b]);
                        m_vertices.push_back(temp_vertices[c]);
                        m_normals.push_back(glm::vec3(0.f)); // Dummy normal for alignment
                        m_normals.push_back(glm::vec3(0.f)); // Dummy normal for alignment
                        m_normals.push_back(glm::vec3(0.f)); // Dummy normal for alignment
                        m_uvs.push_back(glm::vec2(0.f)); // Dummy uv for alignment
                        m_uvs.push_back(glm::vec2(0.f)); // Dummy uv for alignment
                        m_uvs.push_back(glm::vec2(0.f)); // Dummy uv for alignment
                        b=c;
					}
				}
				else if (cntSlashes == cntSpaces)
				{
					// Vertex id/Texture id
					for (char& c : row)
						if (c == '/')
							c = ' ';
					std::stringstream reader{ row.c_str() + 2 };
					int va, vb, vc;
					int ta, tb, tc;

					reader >> va >> ta >> vb >> tb;
					--va;
					--vb;
					--ta;
					--tb;
					while(reader >> vc >> tc)
					{
						--vc;
						--tc;
						m_vertices.push_back(temp_vertices[va]);
                        m_vertices.push_back(temp_vertices[vb]);
                        m_vertices.push_back(temp_vertices[vc]);
                        m_normals.push_back(glm::vec3(0.f)); // Dummy normal for alignment
                        m_normals.push_back(glm::vec3(0.f)); // Dummy normal for alignment
                        m_normals.push_back(glm::vec3(0.f)); // Dummy normal for alignment
                        m_uvs.push_back(temp_uvs[ta]);
                        m_uvs.push_back(temp_uvs[tb]);
                        m_uvs.push_back(temp_uvs[tc]);
                        vb=vc;
                        tb=tc;
					}
				}
				else if (cntSlashes == cntSpaces * 2)
				{
					// Vertex id/Texture id/Normal id
					// or
					// Vertex id//Normal id
					for (char& c : row)
						if (c == '/')
							c = ' ';

					int i;
					for (i = 2; i < (int)row.size() && row[i] != ' '; ++i);

					if (row[i + 1] == ' ')
					{
						// Vertex id//Normal id
						std::stringstream reader{ row.c_str() + 2 };
						int va, vb, vc;
						int na, nb, nc;

						reader >> va >> na >> vb >> nb;
						--va;
						--vb;
						--na;
						--nb;
						while(reader >> vc >> nc)
						{
							--vc;
							--nc;
							m_vertices.push_back(temp_vertices[va]);
							m_vertices.push_back(temp_vertices[vb]);
							m_vertices.push_back(temp_vertices[vc]);
							m_normals.push_back(temp_normals[na]);
							m_normals.push_back(temp_normals[nb]);
							m_normals.push_back(temp_normals[nc]);
							m_uvs.push_back(glm::vec2(0.f)); // Dummy uv for alignment
							m_uvs.push_back(glm::vec2(0.f)); // Dummy uv for alignment
							m_uvs.push_back(glm::vec2(0.f)); // Dummy uv for alignment
							nb=nc;
							vb=vc;
						}
					}
					else
					{
						// Vertex id/Texture id/Normal id
						std::stringstream reader{ row.c_str() + 2 };
						int va, vb, vc;
						int na, nb, nc;
						int ta, tb, tc;

						reader >> va >> ta >> na >> vb >> tb >> nb;
						--va;
						--vb;
						--ta;
						--tb;
						--na;
						--nb;
						while(reader >> vc >> tc >> nc)
						{
							--vc;
							--tc;
							--nc;
							m_vertices.push_back(temp_vertices[va]);
							m_vertices.push_back(temp_vertices[vb]);
							m_vertices.push_back(temp_vertices[vc]);
							m_normals.push_back(temp_normals[na]);
							m_normals.push_back(temp_normals[nb]);
							m_normals.push_back(temp_normals[nc]);
							m_uvs.push_back(temp_uvs[ta]);
							m_uvs.push_back(temp_uvs[tb]);
							m_uvs.push_back(temp_uvs[tc]);
							vb=vc;
							tb=tc;
							nb=nc;
						}
					}
				}
			}
		}
		else if(row[0] == 'l')
		{
			// Polygonal line. Will not implement.
		}
		else
		{
			// If it ever gets here, it is something we don't need most likely
		}
	}

	createGLids();
}

void Mesh::draw() const
{
	glBindVertexArray(m_VaoId);
	glDrawArrays(GL_TRIANGLES, 0, m_vertices.size());
}

void Mesh::clear()
{
	m_vertices.clear();
	m_normals.clear();
	m_uvs.clear();
	if (m_VboId)
	{
		glBindBuffer(GL_ARRAY_BUFFER, 0);
        glDeleteBuffers(1, &m_VboId);
        glBindVertexArray(0);
        glDeleteVertexArrays(1, &m_VaoId);

        m_VboId = m_VaoId = 0;
	}
}