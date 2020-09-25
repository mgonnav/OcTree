#include <fstream>
#include <iostream>
#include <sstream>

#include "Octree.hpp"

int main() {
  std::ifstream inFile("../files/rabbit.obj");
  std::vector<Point3D<double> > points{};
  std::stringstream ss;
  std::string point_str;
  Point3D<double> tmp;
  while (getline(inFile, point_str)) {
    std::stringstream ss(point_str);
    ss >> tmp;
    points.push_back(tmp);
  }

  Point3D<double> p{0, 0, 0};
  Cube<double> c{p, 16};
  Octree<double> octree(c);

  octree.insert(points);

  RectangularCuboid<double> search_area{{5, 5, 5}, 6, 6, 6};
  auto found_points = octree.query(search_area);

  std::cout << "Found " << found_points.size() << " points.\n";

  std::cout << "There are " << octree.getPointCount() << " points in total\n";

  std::cout << "There are " << octree.getLeafCount() << " leafs.\n" << std::flush;

  for (int i = 0; i < 10000; ++i) octree.remove(points[i]);

  std::cout << "Now there are " << octree.getPointCount()
            << " points in total\n";
  std::cout << "Now there are " << octree.getLeafCount() << " leafs.\n";

  return 0;
}
