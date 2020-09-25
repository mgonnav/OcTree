#pragma once
#include "OcNode.hpp"
#include "Point3D.hpp"

template <typename T>
class Octr {
  std::shared_ptr<OcNode<T> > root;
  const int MAX_DEPTH = 0;

  Octr(uint max_depth = 16) { build_tree() }
};
