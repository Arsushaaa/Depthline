#pragma once

#include <SFML/Graphics.hpp>

#include "../Input/PlayerCommand.hpp"
#include "../Entities/Weapon/Weapon.hpp"

class Player {
private:
   // временно персонаж - прямоугольник
   sf::RectangleShape player_;
   sf::Vector2f pos_;
   sf::Vector2f facing_direction_;
   float speed_;
   int hp_;

   Weapon weapon_;

   WeaponUseContext GetWeaponContext() const;

public:
   Player();

   const sf::Vector2f& GetPosition() const;
   void SetPosition(const sf::Vector2f& position);
   const sf::Vector2f& GetFacingDirection() const;

   void AimAt(const sf::Vector2f& target_position);

   int GetHp() const;
   bool IsAlive() const;
   void ApplyDamage(int damage);

   sf::Vector2f CalculateMovement(const PlayerCommand& command, float dt);
   sf::FloatRect GetBoundsAt(const sf::Vector2f& position) const;
   void Render(sf::RenderTarget& target) const;

   
   void UpdateWeapon(float dt);

   std::optional<WeaponAction> 
   TryAttack();
};