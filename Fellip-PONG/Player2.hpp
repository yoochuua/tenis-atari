#ifndef PLAYER2_HPP
#define PLAYER2_HPP_

#include "abcg.hpp"
#include "gamedata.hpp"

// class Asteroids;
// class Bullets;
class OpenGLWindow;
// class StarLayers;

class Player2 {
 public:
  void initializeGL(GLuint program);
  void paintGL(const GameData &gameData);
  void terminateGL();
  void createPlayer2(GLuint program);

  void update(const GameData &gameData, bool isPlayer);

 private:
  friend OpenGLWindow;

  GLuint m_program{};
  GLint m_translationLoc{};
  GLint m_colorLoc{};
  GLint m_scaleLoc{};

  GLuint m_vao{};
  GLuint m_vbo{};
  GLuint m_ebo{};

  glm::vec4 m_color{1};
  float m_scale{0.125f};
  glm::vec2 m_translation{glm::vec2(0)};
  glm::vec2 m_velocity{glm::vec2(0)};
};
#endif