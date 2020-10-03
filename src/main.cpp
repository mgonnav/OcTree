#include <Qt3DCore/qaspectengine.h>
#include <Qt3DCore/qentity.h>
#include <Qt3DCore/qtransform.h>
#include <Qt3DExtras/qfirstpersoncameracontroller.h>
#include <Qt3DExtras/qforwardrenderer.h>
#include <Qt3DExtras/qt3dwindow.h>
#include <Qt3DExtras/qtorusmesh.h>
#include <Qt3DRender/qcamera.h>
#include <Qt3DRender/qcameralens.h>
#include <Qt3DRender/qeffect.h>
#include <Qt3DRender/qmaterial.h>
#include <Qt3DRender/qmesh.h>
#include <Qt3DRender/qpointlight.h>
#include <Qt3DRender/qrenderaspect.h>
#include <Qt3DRender/qrenderpass.h>
#include <Qt3DRender/qsceneloader.h>
#include <Qt3DRender/qtechnique.h>
#include <Qt3DRender/qtexture.h>

#include <QGuiApplication>
#include <QtGui/QScreen>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QCommandLinkButton>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QWidget>
#include <fstream>
#include <iostream>
#include <sstream>

#include "Octree.hpp"
#include "scenemodifier.hpp"

#define OBJ_FILE_PATH "../files/rabbit.obj"

template <typename T>
std::vector<Point3D<T> > getPoints(std::string file_path);

void build_tree(Qt3DCore::QEntity *entity);

int main(int argc, char **argv) {
  QApplication app(argc, argv);

  Qt3DExtras::Qt3DWindow *view = new Qt3DExtras::Qt3DWindow();
  view->defaultFrameGraph()->setClearColor(QColor(QRgb(0x4d4d4f)));

  QWidget *container = QWidget::createWindowContainer(view);
  QSize screenSize = view->screen()->size();

  QWidget *widget = new QWidget;
  QHBoxLayout *hLayout = new QHBoxLayout(widget);

  hLayout->addWidget(container, 1);

  widget->setWindowTitle(QStringLiteral("Octree visualization"));

  // Root entity
  Qt3DCore::QEntity *rootEntity = new Qt3DCore::QEntity();

  // Camera
  Qt3DRender::QCamera *cameraEntity = view->camera();

  cameraEntity->lens()->setPerspectiveProjection(45.0f, 16.0f / 9.0f, 0.1f,
                                                 1000.0f);
  cameraEntity->setPosition(QVector3D(10, 10, 10));
  cameraEntity->setViewCenter(QVector3D(-20, -20, -20));

  Qt3DCore::QEntity *lightEntity = new Qt3DCore::QEntity(rootEntity);
  Qt3DRender::QPointLight *light = new Qt3DRender::QPointLight(lightEntity);
  light->setColor("white");
  light->setIntensity(2);
  lightEntity->addComponent(light);
  Qt3DCore::QTransform *lightTransform = new Qt3DCore::QTransform(lightEntity);
  lightTransform->setTranslation(cameraEntity->position());
  lightEntity->addComponent(lightTransform);

  // For camera controls
  Qt3DExtras::QFirstPersonCameraController *camController =
      new Qt3DExtras::QFirstPersonCameraController(rootEntity);
  camController->setCamera(cameraEntity);

  build_tree(rootEntity);

  view->setRootEntity(rootEntity);

  // Show window
  widget->show();
  widget->resize(1200, 800);

  return app.exec();
}

void build_tree(Qt3DCore::QEntity *root_entity) {
  Cube<double> c({3.5, 3.5, 3.5}, 8.5);
  Octree<double> octree(c);

  auto points = getPoints<double>(OBJ_FILE_PATH);

  for (const auto &point : points) octree.insert(point);

  octree.draw(root_entity);
}

template <typename T>
std::vector<Point3D<T> > getPoints(std::string file_path) {
  std::ifstream inFile(file_path);
  std::vector<Point3D<T> > points{};
  std::stringstream ss;
  std::string point_str;
  Point3D<T> tmp(0, 0, 0);
  while (getline(inFile, point_str)) {
    std::stringstream ss(point_str);
    ss >> tmp;
    points.push_back(tmp);
  }

  return points;
}
