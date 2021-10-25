#ifndef GAMEDATA_HPP_
#define GAMEDATA_HPP_

#include <bitset>

enum class Input {Up, Down, Fire};
enum class State { Playing, GameOver};

struct GameData {
  State m_state{State::Playing};
  std::bitset<3> m_input;  // [Up, Down, Fire]
};

#endif