#ifndef GAMEDATA_HPP_
#define GAMEDATA_HPP_

#include <bitset>

enum class Input { Up1, Down1, Up2, Down2, Space };
enum class State { Playing, GameOver, Start, Win };

struct GameData {
  State m_state{State::Start};
  std::bitset<5> m_input;  // [Up1, Down1, Up2, Down2, Space]
};

#endif