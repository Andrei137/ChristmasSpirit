// Ilie Dumitru

#pragma once

#include "bezier/curve.h"
#include <vector>
#include "glm/vec3.hpp"

struct Path
{
	std::vector<float> m_times;
	std::vector<BezierCurve<glm::vec3> > m_curves;

	Path();

	static Path readFromFile(const char* a_file);
	void addPart(const float a_time, const BezierCurve<glm::vec3>& a_curve);

	/*
	Finds the curve that is responsible for that given time then interpolates on it.
	If a_time < 0 then it is treated as 0.
	If a_time > sum of m_times then it is threated as sum of m_times.
	Warning: Floating point operations are made in this function. Associated errors can occur.
	*/
	glm::vec3 interpolate(float a_time) const;

	float length() const;
};
