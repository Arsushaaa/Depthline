#pragma once

#include <vector>

#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics.hpp>

struct WeaponUseContext {
   sf::Vector2f owner_position;
   sf::Vector2f aim_direction;
};

struct WeaponHitbox {
   sf::FloatRect bounds;
   int damage;
};


struct WeaponAction {
   std::vector<WeaponHitbox> hitboxes;
   // std::vector<ProjectileSpawn> projectiles;
};


class IWeaponAttackPattern {
public:
   virtual ~IWeaponAttackPattern() = default;

   virtual WeaponAction BuildAction(
      const WeaponUseContext& context
   ) const = 0;
};

class IWeaponUseLimiter {
public:
   virtual ~IWeaponUseLimiter() = default;

   virtual void Update(float dt) = 0;
   virtual bool CanUse() const = 0;
   virtual void OnUse() = 0;
};

class IWeaponVisual {
public:
   virtual ~IWeaponVisual() = default;

   virtual void SetOwnerPosition(const sf::Vector2f& position) = 0;
   virtual void SetAimDirection(const sf::Vector2f&) {}
   virtual void Update(float) {}
   virtual void OnUse() {}
   virtual void Render(sf::RenderTarget& target) const = 0;
};


struct WeaponComponents {
   std::unique_ptr<IWeaponAttackPattern> attack_pattern;
   std::unique_ptr<IWeaponUseLimiter> limiter;
   std::unique_ptr<IWeaponVisual> visual;
};


// ----------------------------------------------------------------
//                         реализации
// ----------------------------------------------------------------

class BaseSwordPattern : public IWeaponAttackPattern {
private:
   int damage_;
   float range_;
   sf::Vector2f hitbox_size_;

public:
   BaseSwordPattern(
      int damage, 
      float range, 
      sf::Vector2f hitbox_size
   );

   WeaponAction BuildAction(
      const WeaponUseContext& context
   ) const override;
};

class CooldownLimiter : public IWeaponUseLimiter {
private:
   float cooldown_;
   float timer_ = 0.0f;

public:
   CooldownLimiter(float cooldown);

   void Update(float dt) override;
   bool CanUse() const override;
   void OnUse() override;
};

class BaseSwordVisual : public IWeaponVisual {
private:
   std::vector<sf::RectangleShape> rectangles_;
   sf::Vector2f owner_position_;
   sf::Vector2f aim_direction_;
   sf::Vector2f hitbox_size_;
   float visible_timer_;

   void RebuildRectangles();

public:
   BaseSwordVisual(
      sf::Vector2f hitbox_size,
      sf::Color color
   );

   void SetOwnerPosition(const sf::Vector2f& position) override;
   void SetAimDirection(const sf::Vector2f& direction) override;
   void Update(float dt) override;
   void OnUse() override;
   void Render(sf::RenderTarget& target) const override;
};
