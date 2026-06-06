#pragma once


#include <vector>
#include <SFML/Graphics.hpp>


#include "../Entities/Player.hpp"
#include "../Input/PlayerCommand.hpp"
#include "../World/TileMap.hpp"
#include "../Entities/Enemy/Enemy.hpp"

class World {
private:
   Player player_;
   TileMap map_;

   std::vector<Enemy> enemies_;

   template <typename Entity>
   sf::Vector2f GetResolvedPosition(
      const Entity& entity,
      const sf::Vector2f& movement
   ) const;

public:
   World();

   const Player& GetPlayer();
   void Update(const PlayerCommand& command, float dt);
   void Render(sf::RenderTarget& target);
};