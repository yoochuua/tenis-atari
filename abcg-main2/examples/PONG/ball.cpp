//Inclui bibliotecas	
#include "ball.hpp"	
#include <cppitertools/itertools.hpp>	
#include <glm/gtx/fast_trigonometry.hpp>	
#include <glm/gtx/rotate_vector.hpp>	
#include <iostream>	

using namespace std;	

/*	
  Método de iniciação do estado da bola	
*/	
void Ball::initializeGL(GLuint program) {	
  terminateGL();	

  m_program = program;	
  m_colorLoc = glGetUniformLocation(m_program, "color");	
  m_scaleLoc = glGetUniformLocation(m_program, "scale");	
  m_translationLoc = glGetUniformLocation(m_program, "translation");	

  m_translation = glm::vec2(0.0f, -0.1667f);	
  m_velocity = glm::vec2(0);	

  // Cria poligono regular	
  const auto sides{10};	

  std::vector<glm::vec2> positions(0);	
  positions.emplace_back(0, 0);	
  const auto step{M_PI * 2 / sides};	
  for (const auto angle : iter::range(0.0, M_PI * 2, step)) {	
    positions.emplace_back(std::cos(angle), std::sin(angle));	
  }	
  positions.push_back(positions.at(1));	

  // Gera posições de VBO	
  abcg::glGenBuffers(1, &m_vbo);	
  abcg::glBindBuffer(GL_ARRAY_BUFFER, m_vbo);	
  abcg::glBufferData(GL_ARRAY_BUFFER, positions.size() * sizeof(glm::vec2),	
                     positions.data(), GL_STATIC_DRAW);	
  abcg::glBindBuffer(GL_ARRAY_BUFFER, 0);	

  // Pega a posição dos atributos no programa	
  const GLint positionAttribute{	
      abcg::glGetAttribLocation(m_program, "inPosition")};	

  // Cria VAO	
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

 /*	
  Função que desenha a bola	
 */	
void Ball::paintGL(const GameData &gameData) {	
  if (gameData.m_state != State::Playing) return; //Apenas segue a função quando está no estado Playing	

  glUseProgram(m_program);

  glBindVertexArray(m_vao);	

  glUniform1f(m_scaleLoc, m_scale);	
  glUniform2fv(m_translationLoc, 1, &m_translation.x);

  glUniform4fv(m_colorLoc, 1, &m_color.r);	
  abcg::glDrawArrays(GL_TRIANGLE_FAN, 0, 12);	

  glBindVertexArray(0);	

  glUseProgram(0);	
}	

/*	
  Função que libera o VBO, EBO e VAO da classe Ball	
*/	
void Ball::terminateGL() {	
  glDeleteBuffers(1, &m_vbo);	
  glDeleteBuffers(1, &m_ebo);	
  glDeleteVertexArrays(1, &m_vao);	
}	

/*	
  Função que atualiza o estado da bola	
*/	
void Ball::update(float deltaTime, int &directionX, int &directionY) {	
  //Altera a movimentação da bola de acordo com o tempo e a direção que é imputado na função
  setTranslation(glm::vec2((m_translation.x + deltaTime * directionX),	
                           (m_translation.y + deltaTime * directionY)));                      
}
