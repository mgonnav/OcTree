#pragma once
#include <array>
#include <memory>
#include <vector>

#include "Cube.hpp"
#include "Point3D.hpp"

class Octree {
  std::array<std::unique_ptr<Octree>, 8> octants;
  std::vector<std::shared_ptr<Point3D> > points;
  std::unique_ptr<Cube> boundary;
  const uint capacity;
  bool divided;

  void subdivide();

 public:
  int total_points;
  Octree(Cube boundary, uint capacity) : capacity(capacity) {
    this->boundary = std::make_unique<Cube>(boundary);
    this->divided = false;
    this->total_points = 0;
  }
  ~Octree() {}
  bool insert(const Point3D& point);
  std::vector<std::shared_ptr<Point3D> > query(
      const RectangularCuboid& query_volume) const;
  bool remove(const RectangularCuboid& range);
  // bool remove(const Point3D& point);
};

bool Octree::insert(const Point3D& point) {
  if (!boundary->contains(point)) return 0;

  if (!divided) {
    if (points.size() < capacity) {
      points.push_back(std::make_unique<Point3D>(point));
      ++total_points;
      return 1;
    } else {
      subdivide();
      divided = true;

      for (const auto& p : points)
        for (const auto& octant : octants) octant->insert(*p);

      points.clear();
    }
  }

  for (const auto& octant : octants)
    if (octant->insert(point)) {
      ++total_points;
      return 1;
    }

  return 0;
}

void Octree::subdivide() {
  const auto& x = boundary->center.x;
  const auto& y = boundary->center.y;
  const auto& z = boundary->center.z;
  const auto& hs = boundary->half_side;

  Cube nwb{Point3D{x - hs / 2, y - hs / 2, z - hs / 2}, hs / 2};
  octants[0] = std::make_unique<Octree>(nwb, capacity);

  Cube neb{Point3D{x + hs / 2, y - hs / 2, z - hs / 2}, hs / 2};
  octants[1] = std::make_unique<Octree>(neb, capacity);

  Cube nwf{Point3D{x - hs / 2, y - hs / 2, z + hs / 2}, hs / 2};
  octants[2] = std::make_unique<Octree>(nwf, capacity);

  Cube nef{Point3D{x + hs / 2, y - hs / 2, z + hs / 2}, hs / 2};
  octants[3] = std::make_unique<Octree>(nef, capacity);

  Cube swb{Point3D{x - hs / 2, y + hs / 2, z - hs / 2}, hs / 2};
  octants[4] = std::make_unique<Octree>(swb, capacity);

  Cube seb{Point3D{x + hs / 2, y + hs / 2, z - hs / 2}, hs / 2};
  octants[5] = std::make_unique<Octree>(seb, capacity);

  Cube swf{Point3D{x - hs / 2, y + hs / 2, z + hs / 2}, hs / 2};
  octants[6] = std::make_unique<Octree>(swf, capacity);

  Cube sef{Point3D{x + hs / 2, y + hs / 2, z + hs / 2}, hs / 2};
  octants[7] = std::make_unique<Octree>(sef, capacity);
}

std::vector<std::shared_ptr<Point3D> > Octree::query(
    const RectangularCuboid& query_volume) const {

  std::vector<std::shared_ptr<Point3D> > points_found{};
  if (!boundary->intersects(query_volume)) return points_found;

  if (!divided) {
    for (const auto& p : points)
      if (query_volume.contains(*p)) points_found.push_back(p);
  } else {
    for (const auto& octant : octants) {
      auto child_points = octant->query(query_volume);
      points_found.insert(points_found.end(), child_points.begin(),
                          child_points.end());
    }
  }

  return points_found;
}

bool Octree::remove(const RectangularCuboid& range) {
  if (!divided) {
    for (auto it = points.begin(); it != points.end(); ++it)
      if (range.contains(**it)) {
        --total_points;
        points.erase(it);
        return 1;
      }
  } else {
    for (const auto& octant : octants)
      if (octant->remove(range)) {
        --total_points;
        return 1;
      }
  }

  return 0;
}

