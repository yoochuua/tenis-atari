#ifndef PLAYER1_HPP
#define PLAYER1_HPP_

#include "abcg.hpp"
#include "gamedata.hpp"

class OpenGLWindow;

class Player1 {
 public:
  void initializeGL(GLuint program);
  void paintGL(const GameData &gameData);
  void terminateGL();
  void createPlayer1(GLuint program);

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
