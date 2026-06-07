#pragma once

#include <memory>
#include <optional>

#include <Depthline/Entities/Weapon/WeaponComponents.hpp>

class Weapon {
private:
   std::unique_ptr<IWeaponAttackPattern> attack_pattern_;
   std::unique_ptr<IWeaponUseLimiter> limiter_;
   std::unique_ptr<IWeaponVisual> visual_;

public:
   Weapon(WeaponComponents&& components);

   Weapon(const Weapon&) = delete;
   Weapon& operator=(const Weapon&) = delete;
   Weapon(Weapon&&) noexcept = default;
   Weapon& operator=(Weapon&&) noexcept = default;

   void Update(float dt);
   void SetContext(const WeaponUseContext& context);

   bool CanUse() const;
   std::optional<WeaponAction> 
   TryUse(const WeaponUseContext& context);

   void Render(sf::RenderTarget& target) const;
};