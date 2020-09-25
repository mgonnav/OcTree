#include <fstream>
#include <iostream>
#include <sstream>

#include "Octree.hpp"

int main() {
  std::ifstream inFile("../files/rabbit.obj");
  std::vector<Point3D<float> > points{};
  std::stringstream ss;
  std::string point_str;
  Point3D<float> tmp(0, 0, 0);
  while (getline(inFile, point_str)) {
    std::stringstream ss(point_str);
    ss >> tmp;
    points.push_back(tmp);
  }

  Point3D<double> p{3.5, 3.5, 3.5};
  Cube<float> c{p, 8.5};
  Octree<float> octree(c);

  for (const auto& point : points) octree.insert(point);

  int point_count = 0;
  for (const auto& point : points)
    if (octree.find(point)) {
      std::cout << "Found point " << point << "\n";
      ++point_count;
    }

  std::cout << "Found " << point_count << " points in total.\n";

  for (int i = 0; i < 24; ++i) octree.remove(points[i]);

  point_count = 0;
  for (const auto& point : points)
    if (octree.find(point)) {
      ++point_count;
    }

  std::cout << "After removing 24 points, found " << point_count
            << " points in total.\n";

  return 0;
}
