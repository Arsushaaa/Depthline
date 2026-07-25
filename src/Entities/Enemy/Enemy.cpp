#include "Depthline/Entities/Enemy/Enemy.hpp"

#include <utility>

#include "Depthline/Entities/Weapon/Weapon.hpp"

WeaponUseContext Enemy::GetWeaponContext() const {
   return {
      .owner_position = GetPosition(),
      .aim_direction = facing_direction_,
      .owner_bounds = GetBoundsAt(GetPosition())
   };
}

Enemy::Enemy(
   const EnemyConfig& config,
   const sf::Vector2f& position,
   EnemyComponents&& components,
   Weapon weapon
)
   : config_(config),
   hp_(config_.max_hp),
   pos_(position),
   behavior_(std::move(components.behavior)),
   visual_(std::move(components.visual)),
   collider_(std::move(components.collider)),
   facing_direction_(sf::Vector2f{1.0f, 0.0f}),
   weapon_(std::move(weapon))
{
   visual_->SetPosition(pos_);
}

const sf::Vector2f& Enemy::GetPosition() const {
   return pos_;
}

int Enemy::GetHealth() const {
   return hp_;
}

bool Enemy::IsAlive() const {
   return hp_ > 0;
}

EnemyIntent Enemy::DecideIntent(const sf::Vector2f& target_position, float dt) {
   EnemyIntent intent = behavior_->Decide({
      pos_,
       target_position,
      dt
   });

   if (intent.aim_direction != sf::Vector2f{0.0f, 0.0f}) {
      facing_direction_ = 
         intent.aim_direction.normalized();

      visual_->SetFacing(facing_direction_);
   }

   return intent;
}

sf::Vector2f Enemy::CalculateMovement(const EnemyIntent& intent, float dt) const {
   sf::Vector2f direction = intent.move_direction;

   if (direction != sf::Vector2f{0.f, 0.f}) {
      direction = direction.normalized();
   }

   return direction 
      * config_.move_speed 
      * intent.speed_multiplier 
      * dt;
}

void Enemy::SetPosition(const sf::Vector2f& position) {
   pos_ = position;
   visual_->SetPosition(pos_);
}

sf::FloatRect Enemy::GetBoundsAt(const sf::Vector2f& position) const {
   return collider_->GetBoundsAt(position);
}

void Enemy::ApplyDamage(int damage) {
   hp_ -= damage;
}
   
void Enemy::Render(sf::RenderTarget& target) const {
   visual_->Render(target);
   weapon_.Render(target);
}

void Enemy::Update(float dt) {
   visual_->Update(dt);

   weapon_.SetContext(GetWeaponContext());
   weapon_.Update(dt);
}


std::optional<WeaponAction> 
Enemy::TryAttack() {
   return weapon_.TryUse(GetWeaponContext());
}