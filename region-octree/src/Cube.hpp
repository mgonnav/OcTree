#pragma once
#include "Point3D.hpp"

template <typename T>
struct Cube {
  Point3D<double> center;
  double half_side;

  Cube(Point3D<double> center, double half_side)
      : center(center), half_side(half_side) {}
  ~Cube() {}
  bool contains(const Point3D<T>& point) const;
};

template <typename T>
bool Cube<T>::contains(const Point3D<T>& point) const {
  return (point.x >= center.x - half_side && point.x <= center.x + half_side &&
          point.y >= center.y - half_side && point.y <= center.y + half_side &&
          point.z >= center.z - half_side && point.z <= center.z + half_side);
}

