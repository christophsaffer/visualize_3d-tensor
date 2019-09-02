#include <QApplication>

#include <viewer.hpp>

int main(int argc, char** argv) {
  QApplication application(argc, argv);

  QSurfaceFormat fmt;
  fmt.setSamples(10);
  QSurfaceFormat::setDefaultFormat(fmt);

  viewer v{};
  return application.exec();
}