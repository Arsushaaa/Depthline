#pragma once

#include <vector>

#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics.hpp>

struct WeaponUseContext {
   sf::Vector2f owner_position;
   sf::Vector2f aim_direction;
   sf::FloatRect owner_bounds;
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

   virtual void SetContext(
      const WeaponUseContext& context
   ) = 0;

   virtual void Update(float dt) = 0;

   virtual void OnUse(
      const WeaponAction& action
   ) = 0;

   virtual void Render(
      sf::RenderTarget& target
   ) const = 0;
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

class HitboxAttackVisual : public IWeaponVisual {
private:
   std::vector<sf::RectangleShape> rectangles_;
   sf::Color color_;
   float visible_duration_;
   float visible_timer_;

public:
   HitboxAttackVisual(
      sf::Color color, 
      float visible_duration
   );

   void SetContext(
      const WeaponUseContext&
   ) override;

   void Update(float dt) override;

   void OnUse(const WeaponAction& action) override;

   void Render(sf::RenderTarget& target) const override;
};

class ContactAttackPattern : public IWeaponAttackPattern {
private:
   int damage_;
   float attack_thickness_;

public:
   ContactAttackPattern(
      int damage, 
      float attack_thickness
   );

   WeaponAction BuildAction(
      const WeaponUseContext& context
   ) const override;
};