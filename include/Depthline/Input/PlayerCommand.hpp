#pragma once

#include <SFML/Window.hpp>

struct PlayerCommand {
   bool move_up = false;
   bool move_down = false;
   bool move_left = false;
   bool move_right = false;

   bool attack = false;
   bool dash = false;
};

struct PlayerControls {
   sf::Keyboard::Scancode move_up = sf::Keyboard::Scancode::W;
   sf::Keyboard::Scancode move_down = sf::Keyboard::Scancode::S;
   sf::Keyboard::Scancode move_left = sf::Keyboard::Scancode::A;
   sf::Keyboard::Scancode move_right = sf::Keyboard::Scancode::D;

   sf::Keyboard::Scancode attack = sf::Keyboard::Scancode::Space;
   sf::Keyboard::Scancode dash = sf::Keyboard::Scancode::LShift;
};

PlayerCommand ReadPlayerInput(const PlayerControls& controls);