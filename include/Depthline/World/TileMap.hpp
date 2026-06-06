#pragma once

#include <string>
#include <vector>

#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Rect.hpp>

#include "../Entities/Enemy/EnemyFactory.hpp"

struct EnemySpawn {
   sf::Vector2f spawn;
   EnemyType type;
};

class TileMap {
private:
   float tile_size_;
   sf::Vector2f player_spawn_;
   std::vector<EnemySpawn> enemies_spawn_;

   std::vector<std::string> map_;

public:
   TileMap(float tile_size = 64.0f);

   void LoadFromStrings(std::vector<std::string> tiles);
   bool IsWallAt(const sf::Vector2f& world_position) const;
   bool IsWallTile(int tile_x, int tile_y) const;
   bool HasCollision(const sf::FloatRect& bounds) const;
   sf::Vector2f GetPlayerSpawn() const;
   const std::vector<EnemySpawn>& GetEnemiesSpawn() const;

   void Render(sf::RenderTarget& target) const;
};