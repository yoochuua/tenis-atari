#ifndef BALL_HPP_
#define BALL_HPP_

#include <list>
#include "abcg.hpp"
#include "gamedata.hpp"

class OpenGLWindow;

class Balls {
 public:
  void initializeGL(GLuint program);
  void paintGL();
  void terminateGL();

  void update(auto gameData, float deltaTime, int directionX);

 private:
  friend OpenGLWindow;

  GLuint m_program{};
  GLint m_colorLoc{};
  GLint m_translationLoc{};
  GLint m_scaleLoc{};

  GLuint m_vao{};
  GLuint m_vbo{};

  struct Ball {
    bool m_dead{};
    glm::vec2 m_translation{glm::vec2(0)};
    glm::vec2 m_velocity{glm::vec2(0)};
  };

  float m_scale{0.015f};
  std::list<Ball> m_ball;
};

#endif
