#include <QApplication>
#include <QCommandLineParser>

#include <viewer.hpp>

#include <iostream>
#include <locale.h>
#include <sstream>
#include <string>

int main(int argc, char **argv) {

  QApplication application(argc, argv);

  QSurfaceFormat fmt;
  fmt.setSamples(10);
  QSurfaceFormat::setDefaultFormat(fmt);
  setlocale(LC_ALL, "C");

  std::string file = "../tensor.txt";
  float fmax = 0;
  if (argc == 3) {
    std::istringstream inbuf1(argv[1]);
    inbuf1 >> file;
    std::istringstream inbuf2(argv[2]);
    inbuf2 >> fmax;
  } else if (argc == 2) {
    std::istringstream inbuf1(argv[1]);
    inbuf1 >> file;
  }
  if (argc > 3) {
    std::cout << "Too many command line arguments." << '\n';
    return 0;
  }
  viewer v(file, fmax);

  return application.exec();
}
