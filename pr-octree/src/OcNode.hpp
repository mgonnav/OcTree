#pragma once
#include <array>
#include <memory>

#include "Cube.hpp"

template <typename T>
struct OcNode {
  std::array<std::shared_ptr<OcNode>, 8> children;
  std::unique_ptr<Cube<T> > boundary;
  uint depth;
  bool leaf;

  OcNode(const Cube<T>& boundary, uint depth) : depth(depth) {
    this->boundary = std::make_unique<Cube<T> >(boundary);
    this->leaf = false;
  }

  bool isLeaf();
  void subdivide();
};

template <typename T>
bool OcNode<T>::isLeaf() {
  return leaf;
}

template <typename T>
void OcNode<T>::subdivide() {
  const auto& x = boundary->center.x;
  const auto& y = boundary->center.y;
  const auto& z = boundary->center.z;
  const auto& hs = boundary->half_side;

  Cube nwb{Point3D<T>{x - hs / 2, y - hs / 2, z - hs / 2}, hs / 2};
  children[0] = std::make_unique<OcNode<T> >(nwb, depth + 1);

  Cube neb{Point3D<T>{x + hs / 2, y - hs / 2, z - hs / 2}, hs / 2};
  children[1] = std::make_unique<OcNode<T> >(neb, depth + 1);

  Cube nwf{Point3D<T>{x - hs / 2, y - hs / 2, z + hs / 2}, hs / 2};
  children[2] = std::make_unique<OcNode<T> >(nwf, depth + 1);

  Cube nef{Point3D<T>{x + hs / 2, y - hs / 2, z + hs / 2}, hs / 2};
  children[3] = std::make_unique<OcNode<T> >(nef, depth + 1);

  Cube swb{Point3D<T>{x - hs / 2, y + hs / 2, z - hs / 2}, hs / 2};
  children[4] = std::make_unique<OcNode<T> >(swb, depth + 1);

  Cube seb{Point3D<T>{x + hs / 2, y + hs / 2, z - hs / 2}, hs / 2};
  children[5] = std::make_unique<OcNode<T> >(seb, depth + 1);

  Cube swf{Point3D<T>{x - hs / 2, y + hs / 2, z + hs / 2}, hs / 2};
  children[6] = std::make_unique<OcNode<T> >(swf, depth + 1);

  Cube sef{Point3D<T>{x + hs / 2, y + hs / 2, z + hs / 2}, hs / 2};
  children[7] = std::make_unique<OcNode<T> >(sef, depth + 1);
}
