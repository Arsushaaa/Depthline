#include "Depthline/Entities/Weapon/WeaponComponents.hpp"

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




BaseSwordVisual::BaseSwordVisual(
   sf::Vector2f hitbox_size,
   sf::Color color
)
   : owner_position_({1.0f, 0.0f}),
   aim_direction_({1.0f, 0.0f}), 
   hitbox_size_(hitbox_size),
   visible_timer_(0.0f)
{
   rectangles_.resize(5);

   for (auto& rect : rectangles_) {
      rect.setSize(hitbox_size_);
      rect.setOrigin(hitbox_size_ / 2.0f);
      rect.setFillColor(color);
   }
}

void BaseSwordVisual::RebuildRectangles() {
   const sf::Vector2f forward = aim_direction_;

   const sf::Vector2f right{
      -forward.y,
      forward.x
   };

   const std::vector<sf::Vector2f> local_cells = {
      {-1.0f, 0.0f},
      {1.0f, 0.0f},
      {1.0f, 1.0f},
      {-1.0f, 1.0f},
      {0.0f, 1.0f}
   };

   for (std::size_t i = 0; i < local_cells.size(); ++i) {
      const sf::Vector2f& cell = local_cells[i];

      const sf::Vector2f offset =
         right * cell.x * hitbox_size_.x +
         forward * cell.y * hitbox_size_.y;

      rectangles_[i].setPosition(owner_position_ + offset);
   }
}

void BaseSwordVisual::SetOwnerPosition(const sf::Vector2f& position) {
   owner_position_ = position;
   RebuildRectangles();
}

void BaseSwordVisual::SetAimDirection(const sf::Vector2f& direction) {
   if (direction != sf::Vector2f{0.0f, 0.0f}) {
      aim_direction_ = direction.normalized();
   }

   RebuildRectangles();
}

void BaseSwordVisual::Update(float dt) {
   if (visible_timer_ > 0.0f) {
      visible_timer_ -= dt;
   }
}

void BaseSwordVisual::OnUse() {
   visible_timer_ = 0.08f;
}

void BaseSwordVisual::Render(sf::RenderTarget& target) const {
   if (visible_timer_ <= 0.0f) {
      return;
   }

   for (const auto& rect : rectangles_) {
      target.draw(rect);
   }
}
