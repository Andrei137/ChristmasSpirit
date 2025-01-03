// Ilie Dumitru

#pragma once

#include "bezier/curve.h"
#include <cassert>

template<class Point>
BezierCurve<Point>::BezierCurve() :
	m_points()
	{}

template<class Point>
BezierCurve<Point>::BezierCurve(const std::vector<Point>& a_points) :
	m_points(a_points)
	{}

template<class Point>
void BezierCurve<Point>::addPoint(const Point& a_point)
{
	m_points.push_back(a_point);
}

template<class Point>
Point interpolate(const float a_time, const Point& a_p0, const Point& a_p1)
{
	return a_p0 * (1 - a_time) + a_p1 * a_time;
}

template<class Point>
Point BezierCurve<Point>::interpolate(const float a_time) const
{
	assert(a_time >= 0 && a_time <= 1);
	assert(! m_points.empty());

	std::vector<Point> v[2], *curr = v, *next = v + 1;
	v[0] = m_points;
	int i;

	while (curr->size() > 1u)
	{
		next->clear();
		for (i = 1; i < (int)curr->size(); ++i)
			next->push_back(::interpolate(a_time, curr->at(i - 1), curr->at(i)));
		curr = next;
		next = 1 - (curr - v) + v;
	}

	return curr->at(0);
}
