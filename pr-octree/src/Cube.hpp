#pragma once
#include "Point3D.hpp"
#include "RectangularCuboid.hpp"

template <typename T>
struct Cube {
  Point3D<double> center;
  double half_side;

  Cube(Point3D<double> center, double half_side)
      : center(center), half_side(half_side) {}
  ~Cube() {}
  bool contains(const Point3D<T>& point) const;
  bool intersects(const Cube<T>& other) const;
  bool intersects(const RectangularCuboid<T>& other) const;
};

template <typename T>
bool Cube<T>::contains(const Point3D<T>& point) const {
  return (point.x >= center.x - half_side && point.x <= center.x + half_side &&
          point.y >= center.y - half_side && point.y <= center.y + half_side &&
          point.z >= center.z - half_side && point.z <= center.z + half_side);
}

template <typename T>
bool Cube<T>::intersects(const Cube<T>& other) const {
  return !(
      this->center.x - this->half_side > other.center.x + other.half_side ||
      this->center.x + this->half_side < other.center.x - other.half_side ||
      this->center.y - this->half_side > other.center.y + other.half_side ||
      this->center.y + this->half_side < other.center.y - other.half_side ||
      this->center.z - this->half_side > other.center.z + other.half_side ||
      this->center.z + this->half_side < other.center.z - other.half_side);
}

template <typename T>
bool Cube<T>::intersects(const RectangularCuboid<T>& other) const {
  return !(this->center.x - this->half_side > other.center.x + other.width ||
           this->center.x + this->half_side < other.center.x - other.width ||
           this->center.y - this->half_side > other.center.y + other.height ||
           this->center.y + this->half_side < other.center.y - other.height ||
           this->center.z - this->half_side > other.center.z + other.depth ||
           this->center.z + this->half_side < other.center.z - other.depth);
}
