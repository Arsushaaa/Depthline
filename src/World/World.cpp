#include "Depthline/World/World.hpp"

#include <algorithm>
#include <vector>
#include <SFML/System/Vector2.hpp>

#include "Depthline/World/Maps.hpp"
#include "Depthline/Entities/Enemy/EnemyFactory.hpp"
#include "Depthline/Entities/Weapon/WeaponFactory.hpp"

World::World()
   : player_weapon_(WeaponFactory::Create(WeaponType::SwordBase)),
   map_(64.0f)
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

const Player& World::GetPlayer() {
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


sf::Vector2f World::GetResolvedEnemyPosition(
   const Enemy& enemy,
   const sf::Vector2f& movement
) const {
   sf::Vector2f position = enemy.GetPosition();

   const sf::Vector2f position_x = {
      position.x + movement.x,
      position.y
   };

   const sf::FloatRect bounds_x = 
      enemy.GetBoundsAt(position_x);

   if (
      !map_.HasCollision(enemy.GetBoundsAt(position_x)) &&
      !bounds_x.findIntersection(GetBounds(player_))
   ) {
      position = position_x;
   }

   const sf::Vector2f position_y = {
      position.x,
      position.y + movement.y
   };

   const sf::FloatRect bounds_y = 
      enemy.GetBoundsAt(position_y);

   if (
      !map_.HasCollision(enemy.GetBoundsAt(position_y)) &&
      !bounds_y.findIntersection(GetBounds(player_))
   ) {
      position = position_y;
   }

   return position;
}

void World::Update(const PlayerCommand& command, float dt) {
   const sf::Vector2f movement = player_.CalculateMovement(command, dt);
   player_.SetPosition(GetResolvedPosition(player_, movement));

   for (Enemy& enemy : enemies_) {
      const EnemyIntent intent = 
         enemy.DecideIntent(player_.GetPosition(), dt);

      const sf::Vector2f movement = enemy.CalculateMovement(intent, dt);

      enemy.SetPosition(GetResolvedEnemyPosition(enemy, movement));

      enemy.Update(dt);
   }

   WeaponUseContext weapon_context{
      .owner_position = player_.GetPosition(),
      .aim_direction = player_.GetFacingDirection()
   };

   player_weapon_.Update(dt);
   player_weapon_.SetContext(weapon_context);

   if (command.attack) {
      const std::optional<WeaponAction> action =
         player_weapon_.TryUse(weapon_context);

      if (action) {
         for (const WeaponHitbox& hitbox : action->hitboxes) {
            for (Enemy& enemy : enemies_) {
               if (
                  enemy.IsAlive() &&
                  hitbox.bounds.findIntersection(GetBounds(enemy))
               ) {
                  enemy.ApplyDamage(hitbox.damage);
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
   
   player_weapon_.Render(target);
}