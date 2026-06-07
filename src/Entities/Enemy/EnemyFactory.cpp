#include "Depthline/Entities/Enemy/EnemyFactory.hpp"

#include <memory>
#include <stdexcept>

#include "Depthline/Entities/Enemy/EnemyComponents.hpp"

Enemy EnemyFactory::Create(EnemyType type, const sf::Vector2f& position) {
   switch (type) {
      case EnemyType::Shooter:
      case EnemyType::Jumper:
      case EnemyType::Chaser: {
         sf::Vector2f size = {25.0f, 18.0f};
         sf::Vector2f origin = {12.5f, 9.0f};
         sf::Color color = sf::Color::Red;

         return Enemy{
            EnemyConfig{
               .max_hp = 100,
               .move_speed = 150.0f,
               .attack_cooldown_ = 0.8f
            },
            position,
            EnemyComponents{
               .behavior = std::make_unique<ChaserBehavior>(),
               .visual = std::make_unique<RectangleEnemyVisual>(
                  size, origin, color
               ),
               .collider = std::make_unique<BoxEnemyCollider>(
                  size, origin
               )
            }
         };
      }
   }

   throw std::runtime_error("Unknown enemy type");
}