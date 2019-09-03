#pragma once

#include <QOpenGLWidget>
#include <glm/vec3.hpp>
#include <vector>

#include <iostream>
#include <string>

class viewer : public QOpenGLWidget {
  Q_OBJECT

public:
  viewer(std::string input, QOpenGLWidget *parent = nullptr);
  ~viewer() = default;

protected:
  void initializeGL() override;
  void resizeGL(int width, int height) override;
  void update_color();
  void paintGL() override;

  void mouseMoveEvent(QMouseEvent *event) override;
  void wheelEvent(QWheelEvent *event) override;
  void keyPressEvent(QKeyEvent *event) override;

private:
  void compute_look_at();
  float pixel_size() const noexcept;
  void move_origin_on_screen(int x_difference, int y_difference) noexcept;
  void rotate_on_screen(int x_difference, int y_difference) noexcept;
  void zoom(float scale) noexcept;

private:
  static constexpr float pi_ = 3.141592654;

  // camera/frustum state
  float vertical_fov_{0.25 * pi_};
  float min_distance_ = 1e-3f;
  float max_distance_ = 1000;
  glm::vec3 origin_{};
  float eye_distance_{10};
  float eye_azimuth_{};
  float eye_altitude_{};
  glm::vec3 up_{};
  glm::vec3 right_{};
  glm::vec3 direction_{};

  // cache
  int old_mouse_x_{};
  int old_mouse_y_{};
  std::string file;

  std::vector<float> data;
  std::vector<float> data_color;
  int dim = 0;
  int len = 0;
  int peel = 0;
  float set_to_zero_at = 1e-16f;
  float fmax;
};
