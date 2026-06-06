#include "Depthline/Entities/Enemy/Enemy.hpp"

#include <utility>

Enemy::Enemy(
   const EnemyConfig& config,
   const sf::Vector2f& position,
   EnemyComponents&& components
)
   : config_(config),
   hp_(config_.max_hp),
   pos_(position),
   behavior_(std::move(components.behavior)),
   visual_(std::move(components.visual)),
   collider_(std::move(components.collider))
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
   return behavior_->Decide({
      pos_,
       target_position,
      dt
   });
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
}

void Enemy::Update(float dt) {
   visual_->Update(dt);
}