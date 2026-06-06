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

void World::Update(const PlayerCommand& command, float dt) {
   const sf::Vector2f movement = player_.CalculateMovement(command, dt);
   player_.SetPosition(GetResolvedPosition(player_, movement));

   for (Enemy& enemy : enemies_) {
      const EnemyIntent intent = 
         enemy.DecideIntent(player_.GetPosition(), dt);

      const sf::Vector2f movement = enemy.CalculateMovement(intent, dt);

      enemy.SetPosition(GetResolvedPosition(enemy, movement));

      enemy.Update(dt);
   }
}

void World::Render(sf::RenderTarget& target) {
   map_.Render(target);
   player_.Render(target);

   for (Enemy& enemy : enemies_) {
      enemy.Render(target);
   }
}