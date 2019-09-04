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
    std::cout << "Only one input file." << '\n';
    return 0;
  }
  viewer v(file, fmax);
  viewer v1(file, fmax);
  return application.exec();
}
