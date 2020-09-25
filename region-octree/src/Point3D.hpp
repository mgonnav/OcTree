#pragma once
#include <iostream>

template <typename T>
struct Point3D {
  T x, y, z;
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

template <typename T>
bool operator==(const Point3D<T>& p1, const Point3D<T>& p2)
{
  return p1.x == p2.x && p1.y == p2.y && p1.z == p2.z;
}
