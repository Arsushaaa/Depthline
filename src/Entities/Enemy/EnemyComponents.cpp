#include "Depthline/Entities/Enemy/EnemyComponents.hpp"


#include <SFML/System/Vector2.hpp>

// ----------------------------------------------------------------
//                         chaser
// ----------------------------------------------------------------

EnemyIntent ChaserBehavior::Decide(const EnemyBehaviorContext& context) {
   EnemyIntent intent;

   sf::Vector2f direction = 
      context.target_position - context.self_position;

   if (direction != sf::Vector2f{0.0f, 0.0f}) {
      direction = direction.normalized();
   }

   intent.move_direction = direction;
   intent.aim_direction = direction;

   return intent;
}


RectangleEnemyVisual::RectangleEnemyVisual(
   sf::Vector2f size, 
   sf::Vector2f origin, 
   sf::Color color
) {
   shape_.setSize(size);
   shape_.setOrigin(origin);
   shape_.setFillColor(color);
}

void RectangleEnemyVisual::SetPosition(const sf::Vector2f& position) {
   shape_.setPosition(position);
}

void RectangleEnemyVisual::Render(sf::RenderTarget& target) const {
   target.draw(shape_);
}

BoxEnemyCollider::BoxEnemyCollider(
   sf::Vector2f size, sf::Vector2f origin
)
   : size_(size), 
   origin_(origin)
{}

sf::FloatRect
BoxEnemyCollider::GetBoundsAt(const sf::Vector2f& position) const {
   return {
      position - origin_,
      size_
   };
}

// ----------------------------------------------------------------
//                         
// ----------------------------------------------------------------