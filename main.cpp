#include <QApplication>

#include <locale.h>
#include <viewer.hpp>

int main(int argc, char **argv) {
  QApplication application(argc, argv);

  QSurfaceFormat fmt;
  fmt.setSamples(10);
  QSurfaceFormat::setDefaultFormat(fmt);
  setlocale(LC_ALL, "C");

  viewer v{};
  return application.exec();
}
