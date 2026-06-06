#include "Depthline/Entities/Player.hpp"

#include <SFML/Graphics.hpp>

Player::Player() 
   : player_({20,40}),
   pos_({0.0,0.0}),
   speed_(250.0f)
{
   player_.setOrigin({10, 20});
   player_.setFillColor(sf::Color::Green);
}

const sf::Vector2f Player::GetPosition() const {
   return pos_;
}

void Player::SetPosition(const sf::Vector2f& position) {
   pos_ = position;
   player_.setPosition(pos_);
}

sf::Vector2f Player::CalculateMovement(const PlayerCommand& command, float dt) const {
   sf::Vector2f direction{0.0f, 0.0f};

   if (command.move_up) {
      direction -= {0.0f, 1.0f};
   }

   if (command.move_down) {
      direction += {0.0f, 1.0f};
   }

   if (command.move_left) {
      direction -= {1.0f, 0.0f};
   }

   if (command.move_right) {
      direction += {1.0f, 0.0f};
   }

   if (direction != sf::Vector2f{0.f, 0.f}) {
     direction = direction.normalized();
   }

   return direction * speed_ * dt;
}

sf::FloatRect Player::GetBoundsAt(const sf::Vector2f& position) const {
   return {
      position - player_.getOrigin(),
      player_.getSize()
   };
}

void Player::Render(sf::RenderTarget& target) const {
   target.draw(player_);
}