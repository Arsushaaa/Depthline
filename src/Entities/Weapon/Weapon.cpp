#include "Depthline/Entities/Weapon/Weapon.hpp"

#include <utility>
#include <optional>

Weapon::Weapon(WeaponComponents&& components)
   : attack_pattern_(std::move(components.attack_pattern)),
   limiter_(std::move(components.limiter)),
   visual_(std::move(components.visual))
{}

void Weapon::Update(float dt) {
   limiter_->Update(dt);
   visual_->Update(dt);
}

void Weapon::SetContext(const WeaponUseContext& context) {
   visual_->SetOwnerPosition(context.owner_position);
   visual_->SetAimDirection(context.aim_direction);
}

bool Weapon::CanUse() const {
   return limiter_->CanUse();
}

std::optional<WeaponAction> 
Weapon::TryUse(const WeaponUseContext& context) {
   SetContext(context);

   if (!limiter_->CanUse()) {
      return std::nullopt;
   }

   limiter_->OnUse();
   visual_->OnUse();
   
   return attack_pattern_->BuildAction(context);
}

void Weapon::Render(sf::RenderTarget& target) const {
   visual_->Render(target);
}