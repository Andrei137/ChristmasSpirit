// Ilie Dumitru

#include "bezier/path.h"
#include <iostream>
#include <fstream>

Path::Path() : m_times(), m_curves() {}

void Path::addPart(const float a_time, const BezierCurve<glm::vec3>& a_curve)
{
	m_times.push_back(a_time);
	m_curves.push_back(a_curve);
}

Path Path::readFromFile(const char* a_file)
{
	std::ifstream f{ a_file };

	if (! f)
	{
		// Error
		std::cerr << "File \"" << a_file << "\" could not be opened.\n";
		return Path();
	}

	Path path;
	BezierCurve<glm::vec3> curve;
	float time;
	int N, i, n, j;

	if (! (f >> N))
	{
		// Error
		std::cerr << "File \"" << a_file << "\" is not in correct format. Expected the number of parts as the first number.\n";
		return Path();
	}

	assert(N > 0);
	for (i = 0; i < N; ++i)
	{
		if (! (f >> n >> time))
		{
			// Error
			std::cerr << "File \"" << a_file << "\" is not in correct format. Expected the number of points and the time for part " << i << " but not found.\n";
			return Path();
		}
		assert(n > 0);
		assert(time > 0);

		curve.m_points.resize(n);
		for (j = 0; j < n; ++j)
		{
			if (! (f >> curve.m_points[j].x >> curve.m_points[j].y >> curve.m_points[j].z))
			{
				// Error
				std::cerr << "File \"" << a_file << "\" is not in correct format. Expected 3 coordinates for point " << j << " of part " << i << " but not found.\n";
				return Path();
			}
		}

		path.addPart(time, curve);
	}

	return path;
}

glm::vec3 Path::interpolate(float a_time) const
{
	assert(m_times.size() == m_curves.size());
	assert(! m_times.empty());

	if (a_time < 0)
		a_time = 0;

	int i;

	for (i = 0; i < (int)m_times.size(); ++i)
	{
		if (m_times[i] >= a_time)
			return m_curves[i].interpolate(a_time / m_times[i]);
		a_time -= m_times[i];
	}

	return m_curves.back().interpolate(1.);
}