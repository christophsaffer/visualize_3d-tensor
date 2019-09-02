#include <viewer.hpp>

#include <QApplication>
#include <QKeyEvent>
#include <QMouseEvent>
#include <QWheelEvent>

#include <cmath>
#include <cstdlib>
#include <fstream>
#include <glm/ext.hpp>
#include <iostream>
#include <limits>

#include <glm/ext.hpp>

using namespace std;

viewer::viewer(QOpenGLWidget *parent) : QOpenGLWidget(parent) {
  setMouseTracking(true);
  show();
  viewer::zoom(-0.5f);

  ifstream istrm("../tensor.txt");
  string s;
  float f;
  float fmax = -numeric_limits<double>::infinity();
  istrm >> s;
  dim = stoi(s);
  len = dim * dim * dim;
  data.reserve(len);
  if (!istrm.is_open()) {
    cout << "failed to open file" << '\n';
  } else {
    for (int i = 0; i < len; ++i) {
      istrm >> s;
      s = s.substr(0, s.length() - 2);
      f = stof(s);
      if (abs(f) > fmax)
        fmax = abs(f);
      data.push_back(f);
    }
  }
  for (int i = 0; i < len; ++i) {
    data[i] /= fmax;
  }
}

void viewer::initializeGL() {
  glEnable(GL_MULTISAMPLE);
  glEnable(GL_DEPTH_TEST);
  glClearColor(0, 0, 0, 1);
}

void viewer::resizeGL(int width, int height) {
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glViewport(0, 0, width, height);
  glLoadMatrixf(glm::value_ptr(glm::perspective(
      glm::degrees(vertical_fov_), static_cast<float>(width) / height,
      min_distance_, max_distance_)));
  glMatrixMode(GL_MODELVIEW);
  compute_look_at();
}

void paintCube(int x, int y, int z, float color) {

  float r;
  float g;
  float b;
  float alpha = 1.0f;

  if (color < 0) {
    r = 1.0f + color;
    g = 1.0f + color;
    b = 1.0f;
  } else if (color > 0) {
    r = 1.0f;
    g = 1.0f - color;
    b = 1.0f - color;
  } else {
    r = 1.0f;
    g = 1.0f;
    b = 1.0f;
  }

  glBegin(GL_POLYGON);
  glColor4f(r, g, b, alpha);
  glVertex3f(-z, -y, -x);
  glVertex3f(-z - 1, -y, -x);
  glVertex3f(-z - 1, -y - 1, -x);
  glVertex3f(-z, -y - 1, -x);
  glEnd();

  glBegin(GL_POLYGON);
  glColor4f(r, g, b, alpha);
  glVertex3f(-z - 1, -y, -x);
  glVertex3f(-z - 1, -y, -x - 1);
  glVertex3f(-z - 1, -y - 1, -x - 1);
  glVertex3f(-z - 1, -y - 1, -x);
  glEnd();

  glBegin(GL_POLYGON);
  glColor4f(r, g, b, alpha);
  glVertex3f(-z, -y, -x);
  glVertex3f(-z, -y, -x - 1);
  glVertex3f(-z, -y - 1, -x - 1);
  glVertex3f(-z, -y - 1, -x);
  glEnd();

  glBegin(GL_POLYGON);
  glColor4f(r, g, b, alpha);
  glVertex3f(-z, -y, -x - 1);
  glVertex3f(-z - 1, -y, -x - 1);
  glVertex3f(-z - 1, -y - 1, -x - 1);
  glVertex3f(-z, -y - 1, -x - 1);
  glEnd();

  glBegin(GL_POLYGON);
  glColor4f(r, g, b, alpha);
  glVertex3f(-z, -y, -x);
  glVertex3f(-z - 1, -y, -x);
  glVertex3f(-z - 1, -y, -x - 1);
  glVertex3f(-z, -y, -x - 1);
  glEnd();

  glBegin(GL_POLYGON);
  glColor4f(r, g, b, alpha);
  glVertex3f(-z, -y - 1, -x);
  glVertex3f(-z - 1, -y - 1, -x);
  glVertex3f(-z - 1, -y - 1, -x - 1);
  glVertex3f(-z, -y - 1, -x - 1);
  glEnd();
}

void viewer::paintGL() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  int n = 0;
  float mtc = static_cast<float>(dim) / 2; // move to center
  for (int i = 0; i < dim; ++i) {
    for (int j = 0; j < dim; ++j) {
      for (int k = 0; k < dim; ++k) {
        if ((i > peel - 1) && (i < dim - peel) && (j > peel - 1) &&
            (j < dim - peel) && (k > peel - 1) && (k < dim - peel)) {
          paintCube(k - mtc, j - mtc, i - mtc, data[n]);
        }
        n++;
      }
    }
  }
}

void viewer::mouseMoveEvent(QMouseEvent *event) {
  const auto x_difference = event->x() - old_mouse_x_;
  const auto y_difference = event->y() - old_mouse_y_;

  switch (event->buttons()) {
  case Qt::LeftButton:
    rotate_on_screen(x_difference, y_difference);
    break;

  case Qt::RightButton:
    move_origin_on_screen(x_difference, y_difference);
    break;
  }

  old_mouse_x_ = event->x();
  old_mouse_y_ = event->y();
}

void viewer::wheelEvent(QWheelEvent *event) {
  constexpr float scale_factor = 0.003;
  zoom(scale_factor * event->angleDelta().y());
}

void viewer::keyPressEvent(QKeyEvent *event) {
  if (event->key() == Qt::Key_Escape)
    QApplication::quit();
}

void viewer::compute_look_at() {
  const glm::vec3 world_up{0, 1, 0};
  direction_ = {cos(eye_azimuth_) * cos(eye_altitude_), sin(eye_altitude_),
                sin(eye_azimuth_) * cos(eye_altitude_)};
  up_ = glm::normalize(world_up - glm::dot(direction_, world_up) * direction_);
  right_ = glm::cross(up_, direction_);
  const glm::vec3 position = origin_ + eye_distance_ * direction_;

  makeCurrent();
  glLoadIdentity();
  glLoadMatrixf(glm::value_ptr(glm::lookAt(position, origin_, world_up)));
  update();
}

float viewer::pixel_size() const noexcept {
  return 2 * tan(0.5 * vertical_fov_) / height();
}

void viewer::move_origin_on_screen(int x_difference,
                                   int y_difference) noexcept {
  origin_ += pixel_size() * eye_distance_ *
             (-static_cast<float>(x_difference) * right_ +
              static_cast<float>(y_difference) * up_);
  compute_look_at();
}

void viewer::rotate_on_screen(int x_difference, int y_difference) noexcept {
  constexpr float max_altitude = 0.5 * pi_ - 1e-5;
  constexpr float scale_factor = 0.01;

  eye_azimuth_ += x_difference * scale_factor;
  eye_altitude_ += y_difference * scale_factor;
  eye_altitude_ = clamp(eye_altitude_, -max_altitude, max_altitude);
  compute_look_at();
}

void viewer::zoom(float scale) noexcept {
  eye_distance_ *= exp(-scale);
  eye_distance_ = clamp(eye_distance_, min_distance_, max_distance_);
  compute_look_at();
}
