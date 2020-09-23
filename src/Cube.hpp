#pragma once
#include "Point3D.hpp"
#include "RectangularCuboid.hpp"

struct Cube {
  Point3D center;
  double half_side;

  Cube(Point3D center, double half_side)
      : center(center), half_side(half_side) {}
  ~Cube() {}
  bool contains(const Point3D& point) const;
  bool intersects(const Cube& other) const;
  bool intersects(const RectangularCuboid& other) const;
};

bool Cube::contains(const Point3D& point) const {
  return (point.x >= center.x - half_side && point.x <= center.x + half_side &&
          point.y >= center.y - half_side && point.y <= center.y + half_side &&
          point.z >= center.z - half_side && point.z <= center.z + half_side);
}

bool Cube::intersects(const Cube& other) const {
  return !(
      this->center.x - this->half_side > other.center.x + other.half_side ||
      this->center.x + this->half_side < other.center.x - other.half_side ||
      this->center.y - this->half_side > other.center.y + other.half_side ||
      this->center.y + this->half_side < other.center.y - other.half_side ||
      this->center.z - this->half_side > other.center.z + other.half_side ||
      this->center.z + this->half_side < other.center.z - other.half_side);
}

bool Cube::intersects(const RectangularCuboid& other) const {
  return !(this->center.x - this->half_side > other.center.x + other.width ||
           this->center.x + this->half_side < other.center.x - other.width ||
           this->center.y - this->half_side > other.center.y + other.height ||
           this->center.y + this->half_side < other.center.y - other.height ||
           this->center.z - this->half_side > other.center.z + other.depth ||
           this->center.z + this->half_side < other.center.z - other.depth);
}
