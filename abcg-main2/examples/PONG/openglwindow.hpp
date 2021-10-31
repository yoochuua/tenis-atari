#ifndef OPENGLWINDOW_HPP_
#define OPENGLWINDOW_HPP_

#include <imgui.h>

#include <random>

#include "Player1.hpp"
#include "Player2.hpp"
#include "abcg.hpp"
#include "ball.hpp"

class OpenGLWindow : public abcg::OpenGLWindow {
 protected:
  void handleEvent(SDL_Event& event) override;
  void initializeGL() override;
  void paintGL() override;
  void paintUI() override;
  void resizeGL(int width, int height) override;
  void terminateGL() override;

 private:
  GLuint m_objectsProgram{};

  int m_viewportWidth{};
  int m_viewportHeight{};

  GameData m_gameData;

  Player1 m_player1;
  Player2 m_player2;
  Ball m_ball;
  bool isPlayer1 = true;
  bool isPlayer2 = true;
  int directionX = 1;  //-1 esquerda 1 direita
  int directionY = 0;
  int player1Score = 0;
  int player2Score = 0;

  abcg::ElapsedTimer m_restartWaitTimer;

  ImFont* m_font{};

  std::default_random_engine m_randomEngine;

  void restart();
  void initiate();
  void update();
  void checkCollisions();
  void checkWinCondition();
};

#endif