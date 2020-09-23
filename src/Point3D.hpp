#pragma once
#include <iostream>

struct Point3D {
  double x, y, z;
  Point3D() = default;
  Point3D(double x, double y, double z) : x(x), y(y), z(z) {}
  ~Point3D() {}

  friend std::ostream& operator<<(std::ostream& os, const Point3D& point) {
    os << "(" << point.x << ", " << point.y << ", " << point.z << ")";
    return os;
  }

  friend std::istream& operator>>(std::istream& is, Point3D& point) {
    is >> point.x >> point.y >> point.z;
    return is;
  }
};
