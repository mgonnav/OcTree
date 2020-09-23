#include <fstream>
#include <iostream>
#include <sstream>

#include "Octree.hpp"

int main() {
  std::ifstream inFile("rabbit.obj");
  std::vector<Point3D> points{};
  std::stringstream ss;
  std::string point_str;
  Point3D tmp;
  while (getline(inFile, point_str)) {
    std::stringstream ss(point_str);
    ss >> tmp;
    points.push_back(tmp);
  }

  Point3D p{0, 0, 0};
  Cube c{p, 12};
  Octree octree(c, 4);

  for (const auto& point : points) octree.insert(point);

  RectangularCuboid search_area{{5, 5, 5}, 6, 6, 6};
  auto found_points = octree.query(search_area);

  std::cout << "Found " << found_points.size() << " points.\n";

  std::cout << "There are " << octree.total_points << " points in total\n";

  octree.remove({points[0], 1e-9, 1e-9, 1e-9});
  octree.remove({points[1], 1e-9, 1e-9, 1e-9});
  octree.remove({points[2], 1e-9, 1e-9, 1e-9});
  octree.remove({points[3], 1e-9, 1e-9, 1e-9});
  octree.remove({points[4], 1e-9, 1e-9, 1e-9});

  std::cout << "Now there are " << octree.total_points << " points in total\n";

  return 0;
}
