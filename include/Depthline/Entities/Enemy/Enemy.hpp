#pragma once

#include <memory>
#include <optional>

#include <SFML/System/Vector2.hpp>

#include "Depthline/Entities/Enemy/EnemyComponents.hpp"
#include "Depthline/Entities/Weapon/Weapon.hpp"

class Enemy {
private:
   EnemyConfig config_;
   int hp_;
   sf::Vector2f pos_;

   std::unique_ptr<IEnemyBehavior> behavior_;
   std::unique_ptr<IEnemyVisual> visual_;
   std::unique_ptr<IEnemyCollider> collider_;

   sf::Vector2f facing_direction_;

   Weapon weapon_;

   WeaponUseContext GetWeaponContext() const;

public:
   Enemy(
      const EnemyConfig& config,
      const sf::Vector2f& position,
      EnemyComponents&& components,
      Weapon weapon
   );

   Enemy(const Enemy&) = delete;
   Enemy& operator=(const Enemy&) = delete;
   Enemy(Enemy&&) noexcept = default;
   Enemy& operator=(Enemy&&) noexcept = default;

   const sf::Vector2f& GetPosition() const;
   int GetHealth() const;
   bool IsAlive() const;

   EnemyIntent DecideIntent(const sf::Vector2f& target_position, float dt);
   sf::Vector2f CalculateMovement(const EnemyIntent& intent, float dt) const;

   void SetPosition(const sf::Vector2f& position);
   sf::FloatRect GetBoundsAt(const sf::Vector2f& position) const;

   void ApplyDamage(int damage);
   void Render(sf::RenderTarget& target) const;

   void Update(float dt);

   // функции связанные с оружием
   std::optional<WeaponAction> TryAttack();
};
