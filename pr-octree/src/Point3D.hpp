#pragma once
#include <iostream>

template <typename T>
struct Point3D {
  T x, y, z;
  Point3D() = default;
  Point3D(T x, T y, T z) : x(x), y(y), z(z) {}
  ~Point3D() {}

  friend std::ostream& operator<<(std::ostream& os, const Point3D<T>& point) {
    os << "(" << point.x << ", " << point.y << ", " << point.z << ")";
    return os;
  }

  friend std::istream& operator>>(std::istream& is, Point3D<T>& point) {
    is >> point.x >> point.y >> point.z;
    return is;
  }
};
