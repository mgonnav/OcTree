#pragma once
#include "Point3D.hpp"

template <typename T>
struct RectangularCuboid {
  Point3D<T> center;
  T width, height, depth;

  RectangularCuboid(Point3D<T> center, T w, T h, T d)
      : center(center), width(w), height(h), depth(d) {}
  ~RectangularCuboid() {}
  bool contains(const Point3D<T>& point) const;
};

template <typename T>
bool RectangularCuboid<T>::contains(const Point3D<T>& point) const {
  return (point.x >= center.x - width && point.x <= center.x + width &&
          point.y >= center.y - height && point.y <= center.y + height &&
          point.z >= center.z - depth && point.z <= center.z + depth);
}
