#include "ball.hpp"

#include <cppitertools/itertools.hpp>
#include "openglwindow.hpp"

void Balls::initializeGL(GLuint program) {
  terminateGL();

  m_program = program;
  m_colorLoc = abcg::glGetUniformLocation(m_program, "color");
  m_scaleLoc = abcg::glGetUniformLocation(m_program, "scale");
  m_translationLoc = abcg::glGetUniformLocation(m_program, "translation");

  // Create regular polygon
  const auto sides{10};

  std::vector<glm::vec2> positions(0);
  positions.emplace_back(0, 0);
  const auto step{M_PI * 2 / sides};
  for (const auto angle : iter::range(0.0, M_PI * 2, step)) {
    positions.emplace_back(std::cos(angle), std::sin(angle));
  }
  positions.push_back(positions.at(1));

  // Generate VBO of positions
  abcg::glGenBuffers(1, &m_vbo);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
  abcg::glBufferData(GL_ARRAY_BUFFER, positions.size() * sizeof(glm::vec2),
                     positions.data(), GL_STATIC_DRAW);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, 0);

  // Get location of attributes in the program
  const GLint positionAttribute{
      abcg::glGetAttribLocation(m_program, "inPosition")};

  // Create VAO
  abcg::glGenVertexArrays(1, &m_vao);

  // Bind vertex attributes to current VAO
  abcg::glBindVertexArray(m_vao);

  abcg::glEnableVertexAttribArray(positionAttribute);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
  abcg::glVertexAttribPointer(positionAttribute, 2, GL_FLOAT, GL_FALSE, 0,
                              nullptr);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, 0);

  // End of binding to current VAO
  abcg::glBindVertexArray(0);
}

void Balls::paintGL() {
  abcg::glUseProgram(m_program);

  abcg::glBindVertexArray(m_vao);
  abcg::glUniform4f(m_colorLoc, 1, 1, 1, 1);
  abcg::glUniform1f(m_scaleLoc, m_scale);
  for (const auto &ball : m_ball) {
    abcg::glUniform2f(m_translationLoc, ball.m_translation.x,
                      ball.m_translation.y);
    abcg::glDrawArrays(GL_TRIANGLE_FAN, 0, 12);
  }
  abcg::glBindVertexArray(0);

  abcg::glUseProgram(0);
}

void Balls::terminateGL() {
  abcg::glDeleteBuffers(1, &m_vbo);
  abcg::glDeleteVertexArrays(1, &m_vao);
}

int directionY = 1;
int score_p1 = 0;
int score_p2 = 0;

void Balls::update(auto gameData, float deltaTime, int directionX) {
  // Create ball
  if (gameData.m_state == State::Start) {
    const auto inicialPos = glm::vec2{0.0f, 0.0f};
    const auto ballSpeed = glm::vec2{0.5f, 1.5f};
    Ball ball{
        .m_dead = false, .m_translation = inicialPos, .m_velocity = ballSpeed};
    m_ball.push_back(ball);
  }

  for (auto &ball : m_ball) {
    ball.m_translation.y += ball.m_velocity.y * deltaTime * directionY;
    ball.m_translation.x += ball.m_velocity.x * deltaTime * directionX;

    if (ball.m_translation.y < -1.0f) directionY = directionY * -1;
    if (ball.m_translation.y > +1.0f) directionY = directionY * -1;

    // Kill ball if it goes off screen in the X axis
    if (ball.m_translation.x < -1.0f) {
      ball.m_dead = true;
      score_p1 += 1;
    }
    if (ball.m_translation.x > +1.0f) {
      ball.m_dead = true;
      score_p2 += 1;
    }

    if (ball.m_dead) {
      gameData.m_state = State::Start;
    }
  }

  // Reset and score if dead ball
  m_ball.remove_if([](const Ball &p) { return p.m_dead; });
}
