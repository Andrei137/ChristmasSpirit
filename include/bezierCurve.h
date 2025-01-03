// Ilie Dumitru

#ifndef CS_BEZIER_CURVE
#define CS_BEZIER_CURVE

#include <vector>

template<class Point>
struct BezierCurve
{
	std::vector<Point> m_points;

	BezierCurve();
	BezierCurve(const std::vector<Point>& a_points);

	void addPoint(const Point& a_point);

	/**
	Interpolates between the points.
	Takes time proportional to the square of the number of points.
	This function expects a parameter in the range [0, 1].
	*/
	Point interpolate(const float a_time) const;
};

#include "../src/bezierCurve.cpp"

#endif // CS_BEZIER_CURVE
