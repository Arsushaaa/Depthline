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
   visual_->SetContext(context);
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

   WeaponAction action = attack_pattern_->BuildAction(context);

   limiter_->OnUse();
   visual_->OnUse(action);
   
   return action;
}

void Weapon::Render(sf::RenderTarget& target) const {
   visual_->Render(target);
}