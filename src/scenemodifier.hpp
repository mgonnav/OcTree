#pragma once

#include <Qt3DCore/qentity.h>
#include <Qt3DCore/qtransform.h>

#include <Qt3DExtras/QCuboidMesh>
#include <Qt3DExtras/QPhongMaterial>
#include <QtCore/QObject>

#include "Point3D.hpp"

class SceneModifier : public QObject {
  Q_OBJECT

 public:
  explicit SceneModifier(Qt3DCore::QEntity *rootEntity, Point3D<double> point,
                         double scale);
  ~SceneModifier();

 private:
  Qt3DCore::QEntity *m_rootEntity;
  Qt3DCore::QEntity *m_cuboidEntity;
};
