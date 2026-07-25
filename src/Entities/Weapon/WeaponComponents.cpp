#include "Depthline/Entities/Weapon/WeaponComponents.hpp"

#include <cmath>

#include <SFML/System/Vector2.hpp>

BaseSwordPattern::BaseSwordPattern(
   int damage, 
   float range, 
   sf::Vector2f hitbox_size
) 
   : damage_(damage),
   range_(range),
   hitbox_size_(hitbox_size)
{}

WeaponAction BaseSwordPattern::BuildAction(
   const WeaponUseContext& context
) const {
   WeaponAction action;

   sf::Vector2f forward = context.aim_direction;
   
   if (forward != sf::Vector2f{0.0f, 0.0f}) {
      forward = forward.normalized();
   }
   else {
      forward = {1.0f, 0.0f};
   }

   sf::Vector2f right = {-forward.y, forward.x};

   const std::vector<sf::Vector2f> local_cells = {
      {-1.0f, 0.0f},
      {1.0f, 0.0f},
      {1.0f, 1.0f},
      {-1.0f, 1.0f},
      {0.0f, 1.0f}
   };

   for (const sf::Vector2f& cell : local_cells) {
      const sf::Vector2f offset = 
         right * cell.x * hitbox_size_.x +
         forward * cell.y * hitbox_size_.y;

      
      const sf::Vector2f center = 
         context.owner_position + offset;


      action.hitboxes.push_back({
         sf::FloatRect{
            center - hitbox_size_ / 2.0f,
            hitbox_size_
         },
         damage_
      });
   }

   return action;
}



CooldownLimiter::CooldownLimiter(float cooldown) 
   : cooldown_(cooldown) 
{}

void CooldownLimiter::Update(float dt) {
   if (timer_ > 0.0f) {
      timer_ -= dt;
   }
}

bool CooldownLimiter::CanUse() const {
   return timer_ <= 0.f;
}

void CooldownLimiter::OnUse() {
   timer_ = cooldown_;
}




HitboxAttackVisual::HitboxAttackVisual(
   sf::Color color, 
   float visible_duration
)
   : color_(color),
   visible_duration_(visible_duration),
   visible_timer_(0.0f)
{}

void HitboxAttackVisual::SetContext(
   const WeaponUseContext&
) {}

void HitboxAttackVisual::Update(float dt) {
   if (visible_timer_ > 0.0f) {
      visible_timer_ -= dt;
   }

   if (visible_timer_ < 0.0f) {
      visible_timer_ = 0.0f;
   }
}

void HitboxAttackVisual::OnUse(
   const WeaponAction& action
) {
   rectangles_.clear();
   rectangles_.reserve(action.hitboxes.size());

   for (const WeaponHitbox& hitbox : action.hitboxes) {
      sf::RectangleShape rectangle;

      rectangle.setPosition(hitbox.bounds.position);
      rectangle.setSize(hitbox.bounds.size);
      rectangle.setFillColor(color_);

      rectangles_.push_back(rectangle);
   }

   if (rectangles_.empty()) {
      visible_timer_ = 0.0f;
   } 
   else {
      visible_timer_ = visible_duration_;
   }
}

void HitboxAttackVisual::Render(sf::RenderTarget& target) const {
   if (visible_timer_ <= 0.0f) {
      return;
   }

   for (const auto& rect : rectangles_) {
      target.draw(rect);
   }
}

ContactAttackPattern::ContactAttackPattern (
   int damage, 
   float attack_thickness
)
   : damage_(damage),
   attack_thickness_(attack_thickness)
{}

WeaponAction ContactAttackPattern::BuildAction(
   const WeaponUseContext& context
) const {
   WeaponAction action;
   const sf::Vector2f kDirection = context.aim_direction;
   const sf::FloatRect& kOwner = context.owner_bounds;

   if (kDirection == sf::Vector2f{0.0f, 0.0f}) {
      return action;
   }

   sf::Vector2f hitbox_position;
   sf::Vector2f hitbox_size;
   
   if (std::abs(kDirection.x) > std::abs(kDirection.y)) {
      hitbox_size = {
         attack_thickness_,
         kOwner.size.y
      };

      if (kDirection.x > 0.0f) {
         // Справа
         hitbox_position = {
            kOwner.position.x + kOwner.size.x,
            kOwner.position.y
         };
      } else {
         // Слева
         hitbox_position = {
            kOwner.position.x - attack_thickness_,
            kOwner.position.y
         };
      }
   } 
   else {
      hitbox_size = {
         kOwner.size.x,
         attack_thickness_
      };

      if (kDirection.y > 0.0f) {
         // Снизу
         hitbox_position = {
            kOwner.position.x,
            kOwner.position.y + kOwner.size.y
         };
      } else {
         // Сверху
         hitbox_position = {
            kOwner.position.x,
            kOwner.position.y - attack_thickness_
         };
      }
   }

   action.hitboxes.push_back({
      sf::FloatRect{hitbox_position, hitbox_size},
      damage_
   });

   return action;
}

