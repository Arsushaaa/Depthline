#include "Depthline/Entities/Player.hpp"

#include <SFML/Graphics.hpp>

Player::Player() 
   : player_({20,40}),
   pos_({0.0,0.0}),
   speed_(200.0f)
{
   player_.setOrigin({10, 20});
   player_.setFillColor(sf::Color::Green);
}

sf::Vector2f Player::GetPosition() {
   return pos_;
}

void Player::SetPosition(const sf::Vector2f& position) {
   pos_ = position;
   player_.setPosition(pos_);
}

void Player::Update(const PlayerCommand& command, float dt) {
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

   pos_ += direction * speed_ * dt;

   player_.setPosition(pos_);
}

void Player::Render(sf::RenderTarget& target) const {
   target.draw(player_);
}