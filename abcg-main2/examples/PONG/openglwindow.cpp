//Inclui bibliotecas
#include "openglwindow.hpp"
#include <imgui.h>
#include "abcg.hpp"

/*
  Método que lida com os eventos. No caso do jogo, os eventos do teclado.
  Setando o evento de cada tecla que será utilizado a suas respectivas entradas
*/
void OpenGLWindow::handleEvent(SDL_Event &event) {
  // Eventos do Teclado. Seta os imputes as respectivas teclas
  if (event.type == SDL_KEYDOWN) {
    if (event.key.keysym.sym == SDLK_UP)
      m_gameData.m_input.set(static_cast<size_t>(Input::Up1));
    if (event.key.keysym.sym == SDLK_DOWN)
      m_gameData.m_input.set(static_cast<size_t>(Input::Down1));
    if (event.key.keysym.sym == SDLK_w)
      m_gameData.m_input.set(static_cast<size_t>(Input::Up2));
    if (event.key.keysym.sym == SDLK_s)
      m_gameData.m_input.set(static_cast<size_t>(Input::Down2));
    if (event.key.keysym.sym == SDLK_ESCAPE)
      m_gameData.m_input.set(static_cast<size_t>(Input::ESC));
    if (event.key.keysym.sym == SDLK_m)
      m_gameData.m_input.set(static_cast<size_t>(Input::Menu));
  }
  if (event.type == SDL_KEYUP) {
    if (event.key.keysym.sym == SDLK_UP)
      m_gameData.m_input.reset(static_cast<size_t>(Input::Up1));
    if (event.key.keysym.sym == SDLK_DOWN)
      m_gameData.m_input.reset(static_cast<size_t>(Input::Down1));
    if (event.key.keysym.sym == SDLK_w)
      m_gameData.m_input.reset(static_cast<size_t>(Input::Up2));
    if (event.key.keysym.sym == SDLK_s)
      m_gameData.m_input.reset(static_cast<size_t>(Input::Down2));
    if (event.key.keysym.sym == SDLK_ESCAPE)
      m_gameData.m_input.set(static_cast<size_t>(Input::ESC));
    if (event.key.keysym.sym == SDLK_m)
      m_gameData.m_input.set(static_cast<size_t>(Input::Menu));
  }
}

void OpenGLWindow::initializeGL() {
  // Carrega uma nova fonte
  ImGuiIO &io{ImGui::GetIO()};
  auto filename{getAssetsPath() + "Inconsolata-Medium.ttf"};
  m_font = io.Fonts->AddFontFromFileTTF(filename.c_str(), 60.0f);
  if (m_font == nullptr) {
    throw abcg::Exception{abcg::Exception::Runtime("Cannot load font file")};
  }
  // Cria programa para renderizar outros objetos
  m_objectsProgram = createProgramFromFile(getAssetsPath() + "objects.vert",
                                           getAssetsPath() + "objects.frag");

  abcg::glClearColor(0, 0, 0, 1);

#if !defined(__EMSCRIPTEN__)
  abcg::glEnable(GL_PROGRAM_POINT_SIZE);
#endif
  initiate();
}

/*
  Método que inicia os objetos player e coloca o jogo no primeiro estado que é o menu.
*/
void OpenGLWindow::initiate() {
  m_gameData.m_state = State::Menu;
  m_player1.initializeGL(m_objectsProgram);
  m_player2.initializeGL(m_objectsProgram);
  directionX = -1;
  directionY = 0;
  m_ball.initializeGL(m_objectsProgram);
}

/*
  Método de reinicialização do jogo. Ao contrário do método de iniciação, coloca o jogo no estado jogando.
*/
void OpenGLWindow::restart() {
  m_gameData.m_state = State::Playing;
  m_player1.initializeGL(m_objectsProgram);
  m_player2.initializeGL(m_objectsProgram);
  directionX = -1;
  directionY = 0;
  m_ball.initializeGL(m_objectsProgram);
}

/*
  Método que atualiza a janela do jogo.
*/
void OpenGLWindow::update() {
  const float deltaTime{static_cast<float>(getDeltaTime())}; //Constante de tempo

//Se o jogador dar esc o jogo termina
  if (m_gameData.m_input[static_cast<size_t>(Input::ESC)]) {
    terminateGL();
    return;
  }

//Se o jogador clicar m (entrada Menu), abre a tela de menu 
  if (m_gameData.m_input[static_cast<size_t>(Input::Menu)]) {
    m_player1.terminateGL();
    m_player2.terminateGL();
    m_ball.terminateGL();
    initiate();
    return;
  }

  // Verifica se um dos jogadores ganharam e espera 5 segundos para reiniciar
  if (m_gameData.m_state == State::Player1Win &&
      m_restartWaitTimer.elapsed() > 5) {
    restart();
    return;
  }
  if (m_gameData.m_state == State::Player2Win &&
      m_restartWaitTimer.elapsed() > 5) {
    restart();
    return;
  }

  // Wait 2 seconds before restarting
  //Aqui
  if ((m_gameData.m_state == State::ScoreP1 ||
       m_gameData.m_state == State::ScoreP2) &&
      m_restartWaitTimer.elapsed() > 2) {
    restart();
    return;
  }

//Se o estado for Playing, chama os métodos update, checa colisão da bola e as condições de vitória
  if (m_gameData.m_state == State::Playing) {
    m_player1.update(m_gameData, m_ball.m_translation.y, isPlayer1);
    m_player2.update(m_gameData, m_ball.m_translation.y, isPlayer2);
    m_ball.update(deltaTime, directionX, directionY);
    checkCollisions();
    checkWinCondition();
  }
}

/*
  Método que desenha objetos na tela, puxando os metodos de desenho das classes que são utilizadas no jogo.
*/
void OpenGLWindow::paintGL() {
  update();

  abcg::glClear(GL_COLOR_BUFFER_BIT);
  abcg::glViewport(0, 0, m_viewportWidth, m_viewportHeight);

  m_player1.paintGL(m_gameData);
  m_player2.paintGL(m_gameData);
  m_ball.paintGL(m_gameData);
}

/*
  Método onde fica as funções de desenho de widgets
*/
void OpenGLWindow::paintUI() {
  abcg::OpenGLWindow::paintUI();
  {
    if (m_gameData.m_state == State::Playing) {
      const auto size{ImVec2(600, 100)};
      const auto position{ImVec2((m_viewportHeight - size.x) / 2.0f, 0.8f)};
      ImGui::SetNextWindowPos(position);
      ImGui::SetNextWindowSize(size);
      ImGuiWindowFlags flags{ImGuiWindowFlags_NoBackground |
                             ImGuiWindowFlags_NoTitleBar |
                             ImGuiWindowFlags_NoInputs};
      ImGui::Begin(" ", nullptr, flags);
      ImGui::Text("Controls: W|S                   ");
      ImGui::SameLine();
      ImGui::PushFont(m_font);
      ImGui::Text("%d:%d", player2Score, player1Score);
      ImGui::PopFont();
      ImGui::SameLine();
      ImGui::Text("             Controls: UP|DOWN");
      ImGui::Text(
          "____________________________________________________________________"
          "__________");
      ImGui::PushFont(m_font);
    }
    //Caso o estado seja Player1Win, exibe na tela que o primeiro jogador venceu
     else if (m_gameData.m_state == State::Player1Win) {
      const auto size{ImVec2(300, 170)};
      const auto position{ImVec2((m_viewportWidth - size.x) / 2.0f,
                                 (m_viewportHeight - size.y) / 2.0f)};
      ImGui::SetNextWindowPos(position);
      ImGui::SetNextWindowSize(size);
      ImGuiWindowFlags flags{ImGuiWindowFlags_NoBackground |
                             ImGuiWindowFlags_NoTitleBar |
                             ImGuiWindowFlags_NoInputs};
      ImGui::Begin(" ", nullptr, flags);
      ImGui::PushFont(m_font);
      ImGui::Text("Player 1");
      ImGui::Text("  Wins!");
    }
    //Caso o estado seja Player2Win, exibe na tela que o segundo jogador venceu
    else if (m_gameData.m_state == State::Player2Win) {
      const auto size{ImVec2(300, 170)};
      const auto position{ImVec2((m_viewportWidth - size.x) / 2.0f,
                                 (m_viewportHeight - size.y) / 2.0f)};
      ImGui::SetNextWindowPos(position);
      ImGui::SetNextWindowSize(size);
      ImGuiWindowFlags flags{ImGuiWindowFlags_NoBackground |
                             ImGuiWindowFlags_NoTitleBar |
                             ImGuiWindowFlags_NoInputs};
      ImGui::Begin(" ", nullptr, flags);
      ImGui::PushFont(m_font);
      ImGui::Text("Player 2");
      ImGui::Text("  Wins!");
    }
    //Caso o estado seja ScoreP1 exibe que o primeiro jogador recebeu um ponto
    else if (m_gameData.m_state == State::ScoreP1) {
      const auto size{ImVec2(300, 170)};
      const auto position{ImVec2((m_viewportWidth - size.x) / 2.0f,
                                 (m_viewportHeight - size.y) / 2.0f)};
      ImGui::SetNextWindowPos(position);
      ImGui::SetNextWindowSize(size);
      ImGuiWindowFlags flags{ImGuiWindowFlags_NoBackground |
                             ImGuiWindowFlags_NoTitleBar |
                             ImGuiWindowFlags_NoInputs};
      ImGui::Begin(" ", nullptr, flags);
      ImGui::PushFont(m_font);
      ImGui::Text("Point for");
      ImGui::Text("Player 1");
    } else if (m_gameData.m_state == State::ScoreP2) {
      const auto size{ImVec2(300, 170)};
      const auto position{ImVec2((m_viewportWidth - size.x) / 2.0f,
                                 (m_viewportHeight - size.y) / 2.0f)};
      ImGui::SetNextWindowPos(position);
      ImGui::SetNextWindowSize(size);
      ImGuiWindowFlags flags{ImGuiWindowFlags_NoBackground |
                             ImGuiWindowFlags_NoTitleBar |
                             ImGuiWindowFlags_NoInputs};
      ImGui::Begin(" ", nullptr, flags);
      ImGui::PushFont(m_font);
      ImGui::Text("Point for");
      ImGui::Text("Player 2");
    } else if (m_gameData.m_state == State::Menu) {
      const auto size{ImVec2(500, 240)};
      const auto position{ImVec2((m_viewportWidth - size.x) / 2.0f,
                                 (m_viewportHeight - size.y) / 2.0f)};
      ImGui::SetNextWindowPos(position);
      ImGui::Begin(" ", nullptr, ImGuiWindowFlags_NoDecoration);
      ImGui::PushFont(m_font);
      ImGui::Text("       PONG");
      if (ImGui::Button("Player 1 - Right", ImVec2(500, 80))) {
        isPlayer1 = true;
        isPlayer2 = false;
        m_gameData.m_state = State::Playing;
      }
      if (ImGui::Button("Player 2 - Left", ImVec2(500, 80))) {
        isPlayer1 = false;
        isPlayer2 = true;
        m_gameData.m_state = State::Playing;
      }
      if (ImGui::Button("Two Players", ImVec2(500, 80))) {
        isPlayer1 = true;
        isPlayer2 = true;
        m_gameData.m_state = State::Playing;
      }
    }

    ImGui::PopFont();
    ImGui::End();
  }
}
void OpenGLWindow::resizeGL(int width, int height) {
  m_viewportWidth = width;
  m_viewportHeight = height;

  abcg::glClear(GL_COLOR_BUFFER_BIT);
}

void OpenGLWindow::terminateGL() {
  abcg::glDeleteProgram(m_objectsProgram);

  m_player1.terminateGL();
  m_player2.terminateGL();
  m_ball.terminateGL();
}

void OpenGLWindow::checkCollisions() {
  // Variables for better reading
  float ball_X = m_ball.m_translation.x;
  float ball_Y = m_ball.m_translation.y;
  float P1_X = m_player1.m_translation.x;
  float P1_Y = m_player1.m_translation.y;
  float P2_X = m_player2.m_translation.x;
  float P2_Y = m_player2.m_translation.y;

  float distance1 = P1_X - ball_X - 0.05f;
  float distance2 = P2_X - ball_X - 0.10f;

  // Collision with Player1
  if (distance1 <= 0.05f) {
    if (P1_Y + 0.11f >= ball_Y && (P1_Y - 0.16f < ball_Y) && ball_X < 0.90f) {
      directionX = directionX * -1;
      if (P1_Y == ball_Y) {
        directionY = 0;
      } else if (P1_Y > ball_Y) {
        directionY = -1;
      } else if (P1_Y < ball_Y) {
        directionY = 1;
      }
    }
  }
  // Collision with Player2
  else if (distance2 >= -0.05f) {
    if (P2_Y + 0.11f >= ball_Y && (P2_Y - 0.16f < ball_Y) && ball_X > -0.90f) {
      directionX = directionX * -1;
      if (P2_Y == ball_Y) {
        directionY = 0;
      } else if (P2_Y > ball_Y) {
        directionY = -1;
      } else if (P2_Y < ball_Y) {
        directionY = 1;
      }
    }
  }
  if (ball_Y <= -0.99f) directionY = 1;
  if (ball_Y >= 0.67f) directionY = -1;
  if (ball_X >= 1.0f) {
    player2Score++;
    directionX = 0;
    directionY = 0;
    m_gameData.m_state = State::ScoreP2;
  }
  if (ball_X <= -1.0f) {
    player1Score++;
    directionX = 0;
    directionY = 0;
    m_gameData.m_state = State::ScoreP1;
  }
}

void OpenGLWindow::checkWinCondition() {
  if (player1Score == 5) {
    m_gameData.m_state = State::Player1Win;
    player1Score = 0;
    player2Score = 0;
  }
  if (player2Score == 5) {
    m_gameData.m_state = State::Player2Win;
    player1Score = 0;
    player2Score = 0;
  }
  m_restartWaitTimer.restart();
}
