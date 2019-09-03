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
  if (argc == 2) {
    std::istringstream inbuf1(argv[1]);
    inbuf1 >> file;
  }
  if (argc > 2) {
    std::cout << "Only one input file." << '\n';
    return 0;
  }
  viewer v(file);
  return application.exec();
}
