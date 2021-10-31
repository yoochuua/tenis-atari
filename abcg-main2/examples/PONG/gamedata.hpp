#ifndef GAMEDATA_HPP_
#define GAMEDATA_HPP_

#include <bitset>

//Define quais serão as entradas do game
enum class Input { Up1, Down1, Up2, Down2, ESC, Menu };

//Define os estados possíveis do game do game
enum class State { Menu, Playing, ScoreP1, ScoreP2, Player1Win, Player2Win };

struct GameData {
  State m_state{State::Playing};
  std::bitset<6> m_input;  // [Up1, Down1, Up2, Down2, ESC, Menu]
};

#endif