#pragma once
#include <algorithm>
#include <stack>
#include <vector>

#include "Cube.hpp"
#include "Point3D.hpp"

#define MAX_DEPTH 32

template <typename T>
class Octree {
  struct OcNode;
  OcNode* root;
  bool find(const Point3D<T>& point, std::stack<OcNode*>* node_path,
            OcNode* initial = nullptr) const;

 public:
  explicit Octree(const Cube<T>& boundary) {
    this->root = new OcNode{boundary};
  }
  bool insert(const Point3D<T>& point);
  bool find(const Point3D<T>& point) const;
  bool remove(const Point3D<T>& point);
};

template <typename T>
struct Octree<T>::OcNode {
  Cube<T>* boundary;
  OcNode* children[8];
  std::vector<Point3D<T>*>* points;

  explicit OcNode(const Cube<T>& boundary) {
    this->boundary = new Cube<T>{boundary};
    for (int i = 0; i < 8; ++i) this->children[i] = nullptr;
    this->points = new std::vector<Point3D<T>*>();
  };
  ~OcNode() {
    delete boundary;
    for (int i = 0; i < 8; ++i) delete children[i];
    delete points;
  }
  void subdivide();
  bool isLeaf() const;
  bool isPainted() const;
  bool hasPoint(const Point3D<T>& point) const;
};

template <typename T>
bool Octree<T>::find(const Point3D<T>& point) const {
  std::stack<OcNode*> node_path;
  return find(point, &node_path);
}

template <typename T>
bool Octree<T>::insert(const Point3D<T>& point) {
  std::stack<OcNode*> node_path;
  if (!find(point, &node_path)) {
    while (node_path.size() < MAX_DEPTH) {
      OcNode* current = node_path.top();
      node_path.pop();
      current->subdivide();
      find(point, &node_path, current);
    }

    OcNode* leaf_node = node_path.top();
    node_path.pop();
    leaf_node->points->push_back(new Point3D<T>{point});

    bool should_adjust = true;
    while (!node_path.empty() && should_adjust) {
      OcNode* current = node_path.top();
      node_path.pop();
      for (const auto& child : current->children)
        if (!child->isPainted()) should_adjust = false;

      if (should_adjust) {
        for (int i = 0; i < 8; ++i) {
          auto& child = current->children[i];
          current->points->insert(current->points->end(),
                                  child->points->begin(), child->points->end());
          delete current->children[i];
          current->children[i] = nullptr;
        }
      }
    }
  }

  return 0;
}

template <typename T>
bool Octree<T>::find(const Point3D<T>& point, std::stack<OcNode*>* node_path,
                     OcNode* initial) const {
  OcNode *current, *next;
  current = initial ? initial : root;

  if (!current->boundary->contains(point)) return false;

  for (; current; current = next) {
    next = nullptr;
    for (const auto& child : current->children)
      if (child && child->boundary->contains(point)) next = child;
    node_path->push(current);
  }

  return node_path->top()->hasPoint(point);
}

template <typename T>
bool Octree<T>::OcNode::hasPoint(const Point3D<T>& point) const {
  bool point_found = false;
  for (const auto& p : *points)
    if (*p == point) {
      point_found = true;
      break;
    }
  return point_found;
}

template <typename T>
bool Octree<T>::OcNode::isPainted() const {
  return isLeaf() && (points->size() > 0);
}

template <typename T>
bool Octree<T>::OcNode::isLeaf() const {
  return children[0] == nullptr;
}

template <typename T>
void Octree<T>::OcNode::subdivide() {
  const auto& x = boundary->center.x;
  const auto& y = boundary->center.y;
  const auto& z = boundary->center.z;
  const auto& hs = boundary->half_side;

  Cube<T> nwb{Point3D<double>{x - hs / 2, y - hs / 2, z - hs / 2}, hs / 2};
  children[0] = new OcNode(nwb);

  Cube<T> neb{Point3D<double>{x + hs / 2, y - hs / 2, z - hs / 2}, hs / 2};
  children[1] = new OcNode(neb);

  Cube<T> nwf{Point3D<double>{x - hs / 2, y - hs / 2, z + hs / 2}, hs / 2};
  children[2] = new OcNode(nwf);

  Cube<T> nef{Point3D<double>{x + hs / 2, y - hs / 2, z + hs / 2}, hs / 2};
  children[3] = new OcNode(nef);

  Cube<T> swb{Point3D<double>{x - hs / 2, y + hs / 2, z - hs / 2}, hs / 2};
  children[4] = new OcNode(swb);

  Cube<T> seb{Point3D<double>{x + hs / 2, y + hs / 2, z - hs / 2}, hs / 2};
  children[5] = new OcNode(seb);

  Cube<T> swf{Point3D<double>{x - hs / 2, y + hs / 2, z + hs / 2}, hs / 2};
  children[6] = new OcNode(swf);

  Cube<T> sef{Point3D<double>{x + hs / 2, y + hs / 2, z + hs / 2}, hs / 2};
  children[7] = new OcNode(sef);
}

template <typename T>
bool Octree<T>::remove(const Point3D<T>& point) {
  std::stack<OcNode*> node_path;

  if (find(point, &node_path)) {
    auto current = node_path.top();

    auto points = *current->points;
    current->points->clear();
    for (auto it = points.begin(); it != points.end(); ++it)
      if (**it == point) {
        points.erase(it);
        break;
      }

    for (const auto& point : points) {
      insert(*point);
      delete point;
    }
  }

  return 0;
}
