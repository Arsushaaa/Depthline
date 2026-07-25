#include "Depthline/World/World.hpp"

#include <vector>
#include <SFML/System/Vector2.hpp>

#include "Depthline/World/Maps.hpp"
#include "Depthline/Entities/Enemy/EnemyFactory.hpp"

World::World()
   : map_(64.0f)
{
   map_.LoadFromStrings(map1);
   player_.SetPosition(map_.GetPlayerSpawn());
   const std::vector<EnemySpawn>& enemies_spawn = map_.GetEnemiesSpawn();

   for (const EnemySpawn& spawn : enemies_spawn) {
      enemies_.push_back(
         EnemyFactory::Create(
            spawn.type, 
            spawn.spawn
         )
      );
   }
}

const Player& World::GetPlayer() const {
   return player_;
}

template <typename Entity>
sf::Vector2f World::GetResolvedPosition(
   const Entity& entity,
   const sf::Vector2f& movement
) const {
   sf::Vector2f position = entity.GetPosition();

   const sf::Vector2f position_x = {
      position.x + movement.x,
      position.y
   };

   if (!map_.HasCollision(entity.GetBoundsAt(position_x))) {
      position = position_x;
   }

   const sf::Vector2f position_y = {
      position.x,
      position.y + movement.y
   };

   if (!map_.HasCollision(entity.GetBoundsAt(position_y))) {
      position = position_y;
   }

   return position;
}

template <typename Entity>
sf::FloatRect World::GetBounds(const Entity& entity) const {
   return entity.GetBoundsAt(entity.GetPosition());
}


EnemyMovementResult World::ResolveEnemyMovement(
   const Enemy& enemy,
   const sf::Vector2f& movement
) const {
   EnemyMovementResult result{
      .position = enemy.GetPosition()
   };

   const sf::FloatRect player_bounds = GetBounds(player_);

   const sf::Vector2f position_x{
      result.position.x + movement.x,
      result.position.y
   };

   const sf::FloatRect bounds_x =
      enemy.GetBoundsAt(position_x);

   const bool hits_player_x =
      bounds_x.findIntersection(player_bounds).has_value();

   if (hits_player_x) {
      result.collided_with_player = true;
   }

   if (
      !map_.HasCollision(bounds_x) &&
      !hits_player_x
   ) {
      result.position = position_x;
   }

   const sf::Vector2f position_y{
      result.position.x,
      result.position.y + movement.y
   };

   const sf::FloatRect bounds_y =
      enemy.GetBoundsAt(position_y);

   const bool hits_player_y =
      bounds_y.findIntersection(player_bounds).has_value();

   if (hits_player_y) {
      result.collided_with_player = true;
   }

   if (
      !map_.HasCollision(bounds_y) &&
      !hits_player_y
   ) {
      result.position = position_y;
   }

   return result;
}


void World::Update(const PlayerCommand& command, float dt) {
   if (!player_.IsAlive()) {
      return;
   }

   const sf::Vector2f movement = player_.CalculateMovement(command, dt);
   player_.SetPosition(GetResolvedPosition(player_, movement));

   player_.AimAt(command.aim_world_position);

   for (Enemy& enemy : enemies_) {
      const EnemyIntent intent = 
         enemy.DecideIntent(player_.GetPosition(), dt);

      const sf::Vector2f movement =
         enemy.CalculateMovement(intent, dt);

      const EnemyMovementResult movement_result =
         ResolveEnemyMovement(enemy, movement);

      enemy.SetPosition(movement_result.position);

      enemy.Update(dt);

      if (movement_result.collided_with_player) {
         const std::optional<WeaponAction> action =
            enemy.TryAttack();

         if (action) {
            for (const WeaponHitbox& hitbox : action->hitboxes) {
               if (hitbox.bounds.findIntersection(GetBounds(player_))) {
                  player_.ApplyDamage(hitbox.damage);
                  break;
               }
            }
         }
      }
   }


   player_.UpdateWeapon(dt);

   if (command.attack) {
      const std::optional<WeaponAction> action =
         player_.TryAttack();

      if (action) {
         for (Enemy& enemy : enemies_) {
            if (!enemy.IsAlive()) {
               continue;
            }

            for (const WeaponHitbox& hitbox : action->hitboxes) {
               if (hitbox.bounds.findIntersection(GetBounds(enemy))) {
                  enemy.ApplyDamage(hitbox.damage);
                  break;
               }
            }
         }
      }
   }

   std::erase_if(enemies_, [](const Enemy& enemy) {
      return !enemy.IsAlive();
   });
}

void World::Render(sf::RenderTarget& target) {
   map_.Render(target);
   player_.Render(target);

   for (Enemy& enemy : enemies_) {
      enemy.Render(target);
   }
}