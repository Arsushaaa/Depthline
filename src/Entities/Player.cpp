#include "Depthline/Entities/Player.hpp"

#include <algorithm>
#include <SFML/Graphics.hpp>

#include "Depthline/Entities/Weapon/WeaponFactory.hpp"
#include "Depthline/Entities/Weapon/Weapon.hpp"

WeaponUseContext Player::GetWeaponContext() const {
   return {
      .owner_position = GetPosition(),
      .aim_direction = GetFacingDirection(),
      .owner_bounds = GetBoundsAt(GetPosition())
   };
}

Player::Player() 
   : player_({20,40}),
   pos_({0.0,0.0}),
   facing_direction_({1.0f, 0.0f}),
   speed_(250.0f),
   hp_(100),
   weapon_(WeaponFactory::Create(WeaponType::SwordBase))
{
   player_.setOrigin({10, 20});
   player_.setFillColor(sf::Color::Green);
}

const sf::Vector2f& Player::GetPosition() const {
   return pos_;
}

void Player::SetPosition(const sf::Vector2f& position) {
   pos_ = position;
   player_.setPosition(pos_);
}

int Player::GetHp() const {
   return hp_;
}

bool Player::IsAlive() const {
   return hp_ > 0;
}

void Player::ApplyDamage(int damage) {
   if (!IsAlive()) {
      return;
   }

   hp_ = std::max(0, hp_ - damage);
}

const sf::Vector2f& Player::GetFacingDirection() const {
   return facing_direction_;
}

void Player::AimAt(const sf::Vector2f& target_position) {
   const sf::Vector2f direction = target_position - GetPosition();
   if (direction != sf::Vector2f{0.0f, 0.0f}) {
      facing_direction_ = direction.normalized();
   }
}

sf::Vector2f Player::CalculateMovement(const PlayerCommand& command, float dt) {
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
   weapon_.Render(target);
}

void Player::UpdateWeapon(float dt) {
   weapon_.SetContext(GetWeaponContext());
   weapon_.Update(dt);
}

std::optional<WeaponAction> 
Player::TryAttack() {
   return weapon_.TryUse(GetWeaponContext());
}