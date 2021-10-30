#include "openglwindow.hpp"

#include <imgui.h>

#include "abcg.hpp"

// Variável para as bolinhas cairem
bool isPlayer1 = true;
bool isPlayer2 = true;
int directionX = 1;
void OpenGLWindow::handleEvent(SDL_Event &event) {
  // Keyboard events
  if (event.type == SDL_KEYDOWN) {
    if (event.key.keysym.sym == SDLK_UP)
      m_gameData.m_input.set(static_cast<size_t>(Input::Up1));
    if (event.key.keysym.sym == SDLK_DOWN)
      m_gameData.m_input.set(static_cast<size_t>(Input::Down1));
    if (event.key.keysym.sym == SDLK_w)
      m_gameData.m_input.set(static_cast<size_t>(Input::Up2));
    if (event.key.keysym.sym == SDLK_s)
      m_gameData.m_input.set(static_cast<size_t>(Input::Down2));
    if (event.key.keysym.sym == SDLK_SPACE)
      m_gameData.m_input.set(static_cast<size_t>(Input::Space));
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
    if (event.key.keysym.sym == SDLK_SPACE)
      m_gameData.m_input.reset(static_cast<size_t>(Input::Space));
  }
}

void OpenGLWindow::initializeGL() {
  // Load a new font
  ImGuiIO &io{ImGui::GetIO()};
  auto filename{getAssetsPath() + "Inconsolata-Medium.ttf"};
  m_font = io.Fonts->AddFontFromFileTTF(filename.c_str(), 60.0f);
  if (m_font == nullptr) {
    throw abcg::Exception{abcg::Exception::Runtime("Cannot load font file")};
  }

  // Create program to render the other objects
  m_objectsProgram = createProgramFromFile(getAssetsPath() + "objects.vert",
                                           getAssetsPath() + "objects.frag");

  abcg::glClearColor(0, 0, 0, 1);

#if !defined(__EMSCRIPTEN__)
  abcg::glEnable(GL_PROGRAM_POINT_SIZE);
#endif
  restart();
}
void OpenGLWindow::restart() {
  m_gameData.m_state = State::Start;
  m_player1.initializeGL(m_objectsProgram);
  m_player2.initializeGL(m_objectsProgram);
  m_ball.initializeGL(m_objectsProgram);
}

void OpenGLWindow::update() {
  const float deltaTime{static_cast<float>(getDeltaTime())};

  // Wait 3 seconds before restarting
  if (m_gameData.m_state != State::Playing &&
      m_gameData.m_state != State::Start && m_restartWaitTimer.elapsed() > 3) {
    restart();
    return;
  }
  if (m_gameData.m_state == State::Start) {
    if (m_gameData.m_input[static_cast<size_t>(Input::Space)]) {
      m_gameData.m_state = State::Playing;
    }
  }

  m_player1.update(m_gameData, isPlayer1);
  m_player2.update(m_gameData, isPlayer2);
  m_ball.update(m_gameData, deltaTime, directionX);

  if (m_gameData.m_state == State::Playing) {
    checkCollisions();
  }
}

void OpenGLWindow::paintGL() {
  update();

  abcg::glClear(GL_COLOR_BUFFER_BIT);
  abcg::glViewport(0, 0, m_viewportWidth, m_viewportHeight);

  m_player1.paintGL(m_gameData);
  m_player2.paintGL(m_gameData);
  m_ball.paintGL();
}

void OpenGLWindow::paintUI() {
  abcg::OpenGLWindow::paintUI();
  {
    if (m_gameData.m_state == State::Playing) {
      const auto size{ImVec2(94, 85)};
      const auto position{ImVec2((m_viewportHeight - size.x) / 2.0f, 0.8f)};
      ImGui::SetNextWindowPos(position);
      ImGui::SetNextWindowSize(size);
      ImGuiWindowFlags flags{ImGuiWindowFlags_NoBackground |
                             ImGuiWindowFlags_NoTitleBar |
                             ImGuiWindowFlags_NoInputs};
      ImGui::Begin(" ", nullptr, flags);
      ImGui::PushFont(m_font);
      ImGui::Text("0:0");
    } else if (m_gameData.m_state == State::GameOver) {
      const auto size{ImVec2(300, 85)};
      const auto position{ImVec2((m_viewportWidth - size.x) / 2.0f,
                                 (m_viewportHeight - size.y) / 2.0f)};
      ImGui::SetNextWindowPos(position);
      ImGui::SetNextWindowSize(size);
      ImGuiWindowFlags flags{ImGuiWindowFlags_NoBackground |
                             ImGuiWindowFlags_NoTitleBar |
                             ImGuiWindowFlags_NoInputs};
      ImGui::Begin(" ", nullptr, flags);
      ImGui::PushFont(m_font);
      ImGui::Text("Game Over!");
    } else if (m_gameData.m_state == State::Start) {
      const auto size{ImVec2(300, 85)};
      const auto position{ImVec2((m_viewportWidth - size.x) / 2.0f,
                                 (m_viewportHeight - size.y) / 2.0f)};
      ImGui::SetNextWindowPos(position);
      ImGui::SetNextWindowSize(size);
      ImGuiWindowFlags flags{ImGuiWindowFlags_NoBackground |
                             ImGuiWindowFlags_NoTitleBar |
                             ImGuiWindowFlags_NoInputs};
      ImGui::Begin(" ", nullptr, flags);
      ImGui::PushFont(m_font);
      ImGui::Text("Press Space to continue");
      /*else if (m_gameData.m_state == State::Win) {
        ImGui::Text("You Win!");
      }*/

      ImGui::PopFont();
      ImGui::End();
    }
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
  // Check collision between ship and asteroids
  for (const auto &ball : m_ball.m_ball) {
    const auto ballTranslation{ball.m_translation};
    const auto distance1{
        glm::distance(m_player1.m_translation, ballTranslation)};
    const auto distance2{
        glm::distance(m_player2.m_translation, ballTranslation)};

    if (distance1 < m_player1.m_scale * 0.9f + 0.015f * 0.5f ||
        distance2 < m_player2.m_scale * 0.9f + 0.015f * 0.5f) {
      directionX = directionX * -1;
    }
  }
}