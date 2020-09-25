#pragma once
#include <math.h>

#include <array>
#include <memory>
#include <vector>

#include "Cube.hpp"
#include "Point3D.hpp"

template <typename T>
class Octree {
  std::array<std::unique_ptr<Octree>, 8> octants;
  std::unique_ptr<Cube<T> > boundary;
  const uint depth;
  uint max_depth;
  bool divided;
  int total_points;

  void subdivide();
  void adjustTree();
  bool remove(const RectangularCuboid<T>& range);

 public:
  std::vector<std::shared_ptr<Point3D<T> > > points;

  Octree(const Cube<T>& boundary, uint max_depth = 0, uint depth = 0)
      : depth(depth) {
    this->boundary = std::make_unique<Cube<T> >(boundary);
    this->divided = false;
    this->total_points = 0;
    this->max_depth =
        max_depth ? max_depth : log2(this->boundary->half_side * 2);
  }
  ~Octree() {}

  bool insert(const Point3D<T>& point);
  bool insert(const std::vector<Point3D<T> >& points);
  bool insert(const std::vector<std::shared_ptr<Point3D<T> > >& points);
  std::vector<std::shared_ptr<Point3D<T> > > query(
      const RectangularCuboid<T>& query_volume) const;

  bool remove(const Point3D<T>& point);
  bool painted() const;
  bool isLeaf() const;
  std::vector<std::shared_ptr<Point3D<T> > > getPoints() const;
  // std::vector<std::shared_ptr<Point3D<T> > >
  // getAllPoints(std::vector<std::shared_ptr<Point3D<T> > >& found=NULL) const;
  uint getPointCount() const;
  uint getLeafCount() const;
};

template <typename T>
bool Octree<T>::insert(const std::vector<Point3D<T> >& points) {
  bool all_inserted = true;

  for (const auto& point : points)
    if (!this->insert(point)) all_inserted = false;

  return all_inserted;
}

template <typename T>
bool Octree<T>::insert(
    const std::vector<std::shared_ptr<Point3D<T> > >& points) {
  bool all_inserted = true;

  for (const auto& point : points)
    if (!this->insert(*point)) all_inserted = false;

  return all_inserted;
}

template <typename T>
bool Octree<T>::insert(const Point3D<T>& point) {
  if (!boundary->contains(point)) return 0;

  if (!divided) {
    if (depth == max_depth) {
      points.push_back(std::make_unique<Point3D<T> >(point));
      ++total_points;
      return 1;
    } else {
      subdivide();

      for (const auto& p : points)
        for (const auto& octant : octants) octant->insert(*p);

      points.clear();
    }
  }

  for (const auto& octant : octants)
    if (octant->insert(point)) {
      adjustTree();
      ++total_points;
      return 1;
    }

  return 0;
}

template <typename T>
void Octree<T>::subdivide() {
  const auto& x = boundary->center.x;
  const auto& y = boundary->center.y;
  const auto& z = boundary->center.z;
  const auto& hs = boundary->half_side;

  Cube<T> nwb{Point3D<double>{x - hs / 2, y - hs / 2, z - hs / 2}, hs / 2};
  octants[0] = std::make_unique<Octree>(nwb, max_depth, depth + 1);

  Cube<T> neb{Point3D<double>{x + hs / 2, y - hs / 2, z - hs / 2}, hs / 2};
  octants[1] = std::make_unique<Octree>(neb, max_depth, depth + 1);

  Cube<T> nwf{Point3D<double>{x - hs / 2, y - hs / 2, z + hs / 2}, hs / 2};
  octants[2] = std::make_unique<Octree>(nwf, max_depth, depth + 1);

  Cube<T> nef{Point3D<double>{x + hs / 2, y - hs / 2, z + hs / 2}, hs / 2};
  octants[3] = std::make_unique<Octree>(nef, max_depth, depth + 1);

  Cube<T> swb{Point3D<double>{x - hs / 2, y + hs / 2, z - hs / 2}, hs / 2};
  octants[4] = std::make_unique<Octree>(swb, max_depth, depth + 1);

  Cube<T> seb{Point3D<double>{x + hs / 2, y + hs / 2, z - hs / 2}, hs / 2};
  octants[5] = std::make_unique<Octree>(seb, max_depth, depth + 1);

  Cube<T> swf{Point3D<double>{x - hs / 2, y + hs / 2, z + hs / 2}, hs / 2};
  octants[6] = std::make_unique<Octree>(swf, max_depth, depth + 1);

  Cube<T> sef{Point3D<double>{x + hs / 2, y + hs / 2, z + hs / 2}, hs / 2};
  octants[7] = std::make_unique<Octree>(sef, max_depth, depth + 1);

  divided = true;
}

template <typename T>
bool bothSameColor(const std::unique_ptr<Octree<T> >& octant1,
                   const std::unique_ptr<Octree<T> >& octant2);

template <typename T>
void Octree<T>::adjustTree() {
  if (std::equal(octants.begin() + 1, octants.end(), octants.begin(),
                 bothSameColor<T>)) {
    for (auto& octant : octants) {
      points.insert(points.end(), (*octant).points.begin(),
                    (*octant).points.end());
      octant.reset();
    }
    divided = false;
  }
}

template <typename T>
bool bothSameColor(const std::unique_ptr<Octree<T> >& octant1,
                   const std::unique_ptr<Octree<T> >& octant2) {
  if (octant1->isLeaf() && octant2->isLeaf())
    return octant1->painted() == octant2->painted();
  else
    return false;
}

template <typename T>
bool Octree<T>::isLeaf() const {
  return !divided;
}
template <typename T>
bool Octree<T>::painted() const {
  return !points.empty();
}

template <typename T>
std::vector<std::shared_ptr<Point3D<T> > > Octree<T>::query(
    const RectangularCuboid<T>& query_volume) const {
  std::vector<std::shared_ptr<Point3D<T> > > points_found{};
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

template <typename T>
bool Octree<T>::remove(const Point3D<T>& point) {
  return remove({{point}, 1e-9, 1e-9, 1e-9});
}
template <>
bool Octree<int>::remove(const Point3D<int>& point) {
  return remove({{point}, 0, 0, 0});
}

template <typename T>
bool Octree<T>::remove(const RectangularCuboid<T>& range) {
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
        if (octant->isLeaf()) {
          auto tmp_points = octant->getPoints();
          octant->points.clear();
          total_points -= tmp_points.size();
          this->insert(tmp_points);
        }

        --total_points;
        return 1;
      }
  }

  return 0;
}
/*
template <typename T>
std::vector<std::shared_ptr<Point3D<T> > >
Octree<T>::getAllPoints(std::vector<std::shared_ptr<Point3D<T> > >& found) const
{ auto pf = found ? found : std::vector<std::shared_ptr<Point3D<T> >
>(total_points); if (isLeaf()) return points;

  for (const auto& octant : octants) {
    auto child_points = octant->getAllPoints();
    found.insert(points.end(), child_points.begin(), child_points.end());
  }

  return points;
}
*/

template <typename T>
std::vector<std::shared_ptr<Point3D<T> > > Octree<T>::getPoints() const {
  return points;
}

template <typename T>
uint Octree<T>::getPointCount() const {
  return total_points;
}

template <typename T>
uint Octree<T>::getLeafCount() const {
  if (isLeaf()) return 1;

  int total_leafs = 0;
  for (const auto& octant : octants) total_leafs += octant->getLeafCount();
  return total_leafs;
}
