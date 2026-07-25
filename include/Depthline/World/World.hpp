#pragma once


#include <vector>
#include <SFML/Graphics.hpp>


#include "../Entities/Player.hpp"
#include "../Input/PlayerCommand.hpp"
#include "../World/TileMap.hpp"
#include "../Entities/Enemy/Enemy.hpp"


struct EnemyMovementResult {
   sf::Vector2f position;
   bool collided_with_player = false;
};


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

   template <typename Entity>
   sf::FloatRect GetBounds(const Entity& entity) const;

   EnemyMovementResult ResolveEnemyMovement(
      const Enemy& enemy,
      const sf::Vector2f& movement
   ) const;

public:
   World();

   const Player& GetPlayer() const;

   void Update(const PlayerCommand& command, float dt);
   void Render(sf::RenderTarget& target);
};