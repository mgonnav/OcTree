#pragma once
#include "Point3D.hpp"

struct RectangularCuboid {
  Point3D center;
  double width, height, depth;

  RectangularCuboid(Point3D center, double w, double h, double d)
      : center(center), width(w), height(h), depth(d) {}
  ~RectangularCuboid() {}
  bool contains(const Point3D& point) const;
};

bool RectangularCuboid::contains(const Point3D& point) const
{
  return (point.x >= center.x - width && point.x <= center.x + width &&
          point.y >= center.y - height && point.y <= center.y + height &&
          point.z >= center.z - depth && point.z <= center.z + depth);
}

