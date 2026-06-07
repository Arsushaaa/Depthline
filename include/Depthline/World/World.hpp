#pragma once


#include <vector>
#include <SFML/Graphics.hpp>


#include "../Entities/Player.hpp"
#include "../Input/PlayerCommand.hpp"
#include "../World/TileMap.hpp"
#include "../Entities/Enemy/Enemy.hpp"
#include "../Entities/Weapon/Weapon.hpp"

class World {
private:
   Player player_;
   Weapon player_weapon_;
   TileMap map_;

   std::vector<Enemy> enemies_;

   template <typename Entity>
   sf::Vector2f GetResolvedPosition(
      const Entity& entity,
      const sf::Vector2f& movement
   ) const;

   template <typename Entity>
   sf::FloatRect GetBounds(const Entity& entity) const;

   sf::Vector2f GetResolvedEnemyPosition(
      const Enemy& enemy,
      const sf::Vector2f& movement
   ) const;

public:
   World();

   const Player& GetPlayer();
   const sf::Vector2f& GetFacingDirection() const;

   void Update(const PlayerCommand& command, float dt);
   void Render(sf::RenderTarget& target);
};