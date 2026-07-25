#include "Depthline/Input/PlayerCommand.hpp"

PlayerCommand ReadPlayerInput(const PlayerControls& controls) {
   PlayerCommand command;

   command.move_up = sf::Keyboard::isKeyPressed(controls.move_up);
   command.move_down = sf::Keyboard::isKeyPressed(controls.move_down);
   command.move_left = sf::Keyboard::isKeyPressed(controls.move_left);
   command.move_right = sf::Keyboard::isKeyPressed(controls.move_right);

   command.attack = sf::Mouse::isButtonPressed(controls.attack);
   command.dash = sf::Keyboard::isKeyPressed(controls.dash);

   return command;
}