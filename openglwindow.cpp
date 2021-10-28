#include "openglwindow.hpp"

#include <imgui.h>

#include "abcg.hpp"

bool isPlayer = true; //Verifica se terá um segundo player
int player1 = 0;
int player2 = 0;
void OpenGLWindow::handleEvent(SDL_Event &event) {
  // Keyboard events
  
  if (event.type == SDL_KEYDOWN) {
    if (event.key.keysym.sym == SDLK_SPACE)
      m_gameData.m_input.set(static_cast<size_t>(Input::Fire));
    if (event.key.keysym.sym == SDLK_UP)
      m_gameData.m_input.set(static_cast<size_t>(Input::Up));
    if (event.key.keysym.sym == SDLK_DOWN)
      m_gameData.m_input.set(static_cast<size_t>(Input::Down));
    if(isPlayer){
      if (event.key.keysym.sym == SDLK_q)
        m_gameData.m_input.set(static_cast<size_t>(Input::Fire2));
      if (event.key.keysym.sym == SDLK_w)
        m_gameData.m_input.set(static_cast<size_t>(Input::Up2));
      if (event.key.keysym.sym == SDLK_s)
        m_gameData.m_input.set(static_cast<size_t>(Input::Down2));
    }
  }
  if (event.type == SDL_KEYUP) {
    if (event.key.keysym.sym == SDLK_SPACE)
      m_gameData.m_input.reset(static_cast<size_t>(Input::Fire));
    if (event.key.keysym.sym == SDLK_UP)
      m_gameData.m_input.reset(static_cast<size_t>(Input::Up));
    if (event.key.keysym.sym == SDLK_DOWN)
      m_gameData.m_input.reset(static_cast<size_t>(Input::Down));
    if(isPlayer){
      if (event.key.keysym.sym == SDLK_q)
        m_gameData.m_input.reset(static_cast<size_t>(Input::Fire2));
      if (event.key.keysym.sym == SDLK_w)
        m_gameData.m_input.reset(static_cast<size_t>(Input::Up2));
      if (event.key.keysym.sym == SDLK_s)
        m_gameData.m_input.reset(static_cast<size_t>(Input::Down2));
    }
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

  // Start pseudo-random number generator
  m_randomEngine.seed(
      std::chrono::steady_clock::now().time_since_epoch().count());
    // Create program to render the stars
m_starsProgram = createProgramFromFile(getAssetsPath() + "stars.vert",
                                       getAssetsPath() + "stars.frag");
  restart();
}

void OpenGLWindow::restart() {
  m_gameData.m_state = State::Menu;
  //m_gameData.m_state = State::GameOver;
  //m_gameData.m_state = State::Playing;
  m_ship.initializeGL(m_objectsProgram);
  m_adv.initializeGL(m_objectsProgram);
}

void OpenGLWindow::update() {
  // Wait 5 seconds before restarting
  /*if (m_gameData.m_state != State::Playing &&
      m_restartWaitTimer.elapsed() > 5) {
    restart();
    return;
  }*/
  
  if (m_gameData.m_state == State::Playing) {
      m_ship.update(m_gameData);
      m_adv.update(m_gameData, m_ship.m_translation.y, isPlayer); //mudar depois para seguir a bola
      placar(m_ship.m_translation.y); //mudar depois para o x da bola marcar o ponto
      //checkCollisions();
    }
  

}
void OpenGLWindow::paintGL() {
  update();

  abcg::glClear(GL_COLOR_BUFFER_BIT);
  abcg::glViewport(0, 0, m_viewportWidth, m_viewportHeight);
  

  m_ship.paintGL(m_gameData);
  m_adv.paintGL(m_gameData);
}
void OpenGLWindow::paintUI() {
  abcg::OpenGLWindow::paintUI();

  {

    if (m_gameData.m_state == State::Playing) {
      const auto size{ImVec2(300, 85)};
    const auto position{ImVec2((m_viewportHeight - size.x) / 2.0f, 0.8f)};//(m_viewportWidth - size.x) / 2.0f,
                               //(m_viewportHeight - size.y) / 2.0f)};
    ImGui::SetNextWindowPos(position);
    ImGui::SetNextWindowSize(size);
    ImGuiWindowFlags flags{ImGuiWindowFlags_NoBackground |
                           ImGuiWindowFlags_NoTitleBar |
                           ImGuiWindowFlags_NoInputs};
    ImGui::Begin(" ", nullptr, flags);
    ImGui::PushFont(m_font);
      ImGui::Text("%d:%d", player1, player2);
    }
    else if (m_gameData.m_state == State::GameOver) {
       const auto size{ImVec2(300, 85)};
    const auto position{ImVec2((m_viewportWidth - size.x) / 2.0f,(m_viewportHeight - size.y) / 2.0f)};
    ImGui::SetNextWindowPos(position);
    ImGui::SetNextWindowSize(size);
    ImGuiWindowFlags flags{ImGuiWindowFlags_NoBackground |
                           ImGuiWindowFlags_NoTitleBar |
                           ImGuiWindowFlags_NoInputs};
    ImGui::Begin(" ", nullptr, flags);
    ImGui::PushFont(m_font);
      ImGui::Text("Game Over!");
    }
    else if (m_gameData.m_state == State::Menu) {
      const auto size{ImVec2(300, 85)};
      const auto position{ImVec2((m_viewportWidth - size.x) / 2.0f,(m_viewportHeight - size.y) / 2.0f)};
      ImGui::SetNextWindowPos(position);
      ImGui::Begin(" ", nullptr, ImGuiWindowFlags_NoDecoration);
      ImGui::PushFont(m_font);
      if (ImGui::Button("One Player", ImVec2(300, 80))) {
        isPlayer = false;
        m_gameData.m_state = State::Playing;
      }
      if (ImGui::Button("Two Player", ImVec2(300, 80))) {
        isPlayer = true;
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
void OpenGLWindow::placar(float ball_x){
  if(ball_x >= 0.6){
    player1++;
  }
  else if(ball_x <= -0.6){
    player2++;
  }
}

void OpenGLWindow::terminateGL() {
  abcg::glDeleteProgram(m_starsProgram);
  abcg::glDeleteProgram(m_objectsProgram);
    
  m_ship.terminateGL();
  m_adv.terminateGL();
}
