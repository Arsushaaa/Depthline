#pragma once

#include <SFML/System/Vector2.hpp>

#include "Depthline/Entities/Enemy/EnemyComponents.hpp"

struct EnemyConfig {
   int max_hp = 100;
   float move_speed = 100.0f;
};


class Enemy {
private:
   EnemyConfig config_;
   int hp_;
   sf::Vector2f pos_;

   std::unique_ptr<IEnemyBehavior> behavior_;
   std::unique_ptr<IEnemyVisual> visual_;
   std::unique_ptr<IEnemyCollider> collider_;

public:
   Enemy(
      const EnemyConfig& config,
      const sf::Vector2f& position,
      EnemyComponents&& components
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
};
