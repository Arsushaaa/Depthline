#include "Depthline/World/World.hpp"

#include <SFML/System/Vector2.hpp>

#include "Depthline/World/Maps.hpp"

World::World()
   : map_(64.0f)
{
   map_.LoadFromStrings(map1);
   player_.SetPosition(map_.GetPlayerSpawn());
}

const Player& World::GetPlayer() {
   return player_;
}

void World::Update(const PlayerCommand& command, float dt) {
   const sf::Vector2f movement = player_.CalculateMovement(command, dt);
   sf::Vector2f position = player_.GetPosition();

   const sf::Vector2f position_x = {
      position.x + movement.x,
      position.y
   };

   if (!map_.HasCollision(player_.GetBoundsAt(position_x))) {
      position = position_x;
   }

   const sf::Vector2f position_y = {
      position.x,
      position.y + movement.y
   };

   if (!map_.HasCollision(player_.GetBoundsAt(position_y))) {
      position = position_y;
   }

   player_.SetPosition(position);
}

void World::Render(sf::RenderTarget& target) {
   map_.Render(target);
   player_.Render(target);
}