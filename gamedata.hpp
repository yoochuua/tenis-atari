#ifndef GAMEDATA_HPP_
#define GAMEDATA_HPP_

#include <bitset>

enum class Input {Up, Down, Fire, Up2, Down2, Fire2};
enum class State { Menu,Playing, GameOver};

struct GameData {
  State m_state{State::Playing};
  std::bitset<6> m_input;  // [Up, Down, Fire]
};

#endif