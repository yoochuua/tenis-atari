#include "adv.hpp"

#include <glm/gtx/fast_trigonometry.hpp>
#include <glm/gtx/rotate_vector.hpp>

void Adv::initializeGL(GLuint program) {
  terminateGL();
  createMelon(program);
  m_translation.x = 0.8f; m_translation.y = 0.0f;
}
void Adv::paintGL(const GameData &gameData) {
  if (gameData.m_state != State::Playing) return;

  abcg::glUseProgram(m_program);

  abcg::glBindVertexArray(m_vao);

  abcg::glUniform1f(m_scaleLoc, m_scale);
  abcg::glUniform1f(m_rotationLoc, m_rotation);
  abcg::glUniform2fv(m_translationLoc, 1, &m_translation.x);

  abcg::glUniform4fv(m_colorLoc, 1, &m_color.r);
  abcg::glDrawElements(GL_TRIANGLES, 12 * 3, GL_UNSIGNED_INT, nullptr);

  abcg::glBindVertexArray(0);

  abcg::glUseProgram(0);
  
}
void Adv::terminateGL() {
  abcg::glDeleteBuffers(1, &m_vbo);
  abcg::glDeleteBuffers(1, &m_ebo);
  abcg::glDeleteVertexArrays(1, &m_vao);
}
void Adv::update(const GameData &gameData, float ball_y) {
  if (m_translation.y > ball_y && m_translation.y > -0.87f){
    m_translation.y += -0.001f;
    }
  if (m_translation.y < ball_y && m_translation.y < 0.87f){
    m_translation.y += 0.001f;
    }
}
void Adv::createMelon(GLuint program){
  m_program = program;
  m_colorLoc = abcg::glGetUniformLocation(m_program, "color");
  m_rotationLoc = abcg::glGetUniformLocation(m_program, "rotation");
  m_scaleLoc = abcg::glGetUniformLocation(m_program, "scale");
  m_translationLoc = abcg::glGetUniformLocation(m_program, "translation");

  m_rotation = 0.0f;
  m_translation = glm::vec2(0);
  m_velocity = glm::vec2(0);

  std::array<glm::vec2, 24> positions{
      // Ship body
      glm::vec2{-12.0f,+8.0f}, glm::vec2{-8.0f,+8.0f},
      glm::vec2{-12.0f,-16.0f}, glm::vec2{-8.0f,-16.0f}//,
      /*glm::vec2{-3.98282f,+0.70204f}, glm::vec2{-3.8463f,-0.93619f},
      glm::vec2{+3.79878f,-1.00445f}, glm::vec2{+4.00356f,+0.59965f},
      glm::vec2{+0.18102f, +3.9785f}, glm::vec2{-3.41f,-2.06f},*/
      

      // Cannon left
      /*glm::vec2{3.34f, -2.17f}, glm::vec2{+2.29f, -3.26f},
      glm::vec2{-2.22f, +03.31f}, glm::vec2{-0.62f, -3.93f},*/

      // Cannon right
      //glm::vec2{0.81f, -3.9f}, glm::vec2{+09.5f, +04.0f},
      //glm::vec2{+12.5f, +04.0f}, glm::vec2{+12.5f, +10.5f},
      
      // Thruster trail (left)
      /*glm::vec2{-12.0f, -07.5f}, 
      glm::vec2{-09.5f, -18.0f}, 
      glm::vec2{-07.0f, -07.5f},

      // Thruster trail (right)
      glm::vec2{+07.0f, -07.5f}, 
      glm::vec2{+09.5f, -18.0f}, 
      glm::vec2{+12.0f, -07.5f},*/
      };

  // Normalize
  for (auto &position : positions) {
    position /= glm::vec2{15.5f, 15.5f};
  }

  const std::array indices{0, 1, 2,
                           1, 2, 3,
                           /*1, 7, 14,
                           2, 1, 14,
                           8, 2, 14,
                           3, 8, 14,
                           0, 3, 14,
                           4, 0, 14,*/
                           // Cannons
                           /*5, 4, 14,
                           9, 5, 14,
                           9, 12, 14,
                           12, 13, 14,*/
                           // Thruster trails
                           /*18, 19, 20,
                           21, 22, 23*/};

  // Generate VBO
  abcg::glGenBuffers(1, &m_vbo);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
  abcg::glBufferData(GL_ARRAY_BUFFER, sizeof(positions), positions.data(),
                     GL_STATIC_DRAW);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, 0);

  // Generate EBO
  abcg::glGenBuffers(1, &m_ebo);
  abcg::glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
  abcg::glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices.data(),
                     GL_STATIC_DRAW);
  abcg::glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

  // Get location of attributes in the program
  GLint positionAttribute{abcg::glGetAttribLocation(m_program, "inPosition")};

  // Create VAO
  abcg::glGenVertexArrays(1, &m_vao);

  // Bind vertex attributes to current VAO
  abcg::glBindVertexArray(m_vao);

  abcg::glEnableVertexAttribArray(positionAttribute);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
  abcg::glVertexAttribPointer(positionAttribute, 2, GL_FLOAT, GL_FALSE, 0,
                              nullptr);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, 0);

  abcg::glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);

  // End of binding to current VAO
  abcg::glBindVertexArray(0);
}
