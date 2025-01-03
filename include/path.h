// Ilie Dumitru

#ifndef CS_PATH
#define CS_PATH

#include "bezierCurve.h"
#include <vector>
#include "glm/vec3.hpp"

struct Path
{
	std::vector<float> m_times;
	std::vector<BezierCurve<glm::vec3> > m_curves;

	Path();

	void addPart(const float a_time, const BezierCurve<glm::vec3>& a_curve);

	/**
	Finds the curve that is responsible for that given time then interpolates on it.
	If a_time < 0 then it is threated as 0.
	If a_time > sum of m_times then it is threated as sum of m_times.
	Warning: Floating point operations are made in this function. Associated errors can occur.
	*/
	glm::vec3 interpolate(float a_time) const;

	static Path readFromFile(const char* a_file);
};

#endif // CS_PATH
