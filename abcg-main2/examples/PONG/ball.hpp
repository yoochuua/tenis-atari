#ifndef BALL_HPP_
#define BALL_HPP_

//Adiciona bibliotecas 
#include <list>
#include "Player1.hpp"
#include "Player2.hpp"
#include "abcg.hpp"
#include "gamedata.hpp"

class ball;
class OpenGLWindow;

/*
  Definição da classe Ball. Responsável pela movimentação e renderização da bola.
*/
class Ball {
 public:
  void initializeGL(GLuint program);
  void paintGL(const GameData &gameData);
  void terminateGL();

  void update(float deltaTime, int &directionX, int &directionY);
  void setTranslation(glm::vec2 translation) { m_translation = translation; }

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

  float m_scale{0.025f}; //variável escala
  glm::vec2 m_translation{glm::vec2(0)}; //variável vetorial que indica posição da bola
  glm::vec2 m_velocity{glm::vec2(0)}; //Variável vetorial que indica a velocidade da bola
  glm::vec2 m_position{glm::vec2(0)}; //Variável de posição dos pontos

  abcg::ElapsedTimer m_ballMoveTimer;
};

#endif