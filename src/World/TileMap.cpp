#include "Depthline/World/TileMap.hpp"

#include <cstddef>
#include <vector>
#include <string>
#include <stdexcept>

#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RectangleShape.hpp>

TileMap::TileMap(float tile_size)
   : tile_size_(tile_size),
   player_spawn_({0.0f, 0.0f})
{}

void TileMap::LoadFromStrings(std::vector<std::string> tiles) {
   if (tiles.empty()) {
      throw std::runtime_error("TileMap is empty");
   }

   const std::size_t width = tiles.front().size();

   if (width == 0) {
      throw std::runtime_error("TileMap row is empty");
   }

   for (const std::string& row : tiles) {
      if (row.size() != width) {
         throw std::runtime_error("TileMap rows must have equal width");
      }
   }

   map_.clear();
   enemies_spawn_.clear();
   map_.reserve(tiles.size());

   int player_spawn_count = 0;

   for (std::size_t y = 0; y < tiles.size(); ++y) {
      std::string line;
      line.reserve(width);

      for (std::size_t x = 0; x < tiles[y].size(); ++x) {
         const char tile = tiles[y][x];

         if (tile == 'P') {
            ++player_spawn_count;

            player_spawn_ = {
               static_cast<float>(x) * tile_size_ + tile_size_ / 2.f,
               static_cast<float>(y) * tile_size_ + tile_size_ / 2.f
            };

            line.push_back('.');
            continue;
         }

         if (tile == 'E') {
            enemies_spawn_.push_back({
               static_cast<float>(x) * tile_size_ + tile_size_ / 2.f,
               static_cast<float>(y) * tile_size_ + tile_size_ / 2.f
            });

            line.push_back('.');
            continue;
         }

         if (tile != '.' && tile != '#') {
            throw std::runtime_error("TileMap contains unknown tile");
         }

         line.push_back(tile);
      }

      map_.push_back(std::move(line));
   }

   if (player_spawn_count == 0) {
      throw std::runtime_error("TileMap must contain player spawn 'P'");
   }

   if (player_spawn_count > 1) {
      throw std::runtime_error("TileMap must contain exactly one player spawn 'P'");
   }
}


bool TileMap::IsWallAt(const sf::Vector2f& world_position) const {
   const int tile_x = static_cast<int>(std::floor(world_position.x / tile_size_));
   const int tile_y = static_cast<int>(std::floor(world_position.y / tile_size_));

   return IsWallTile(tile_x, tile_y);
}

bool TileMap::IsWallTile(int tile_x, int tile_y) const {
   if (tile_y < 0 || tile_y >= static_cast<int>(map_.size())) {
      return true;
   }

   if (tile_x < 0 || tile_x >= static_cast<int>(map_[tile_y].size())) {
      return true;
   }

   return map_[tile_y][tile_x] == '#';
}


sf::Vector2f TileMap::GetPlayerSpawn() const {
   return player_spawn_;
}


void TileMap::Render(sf::RenderTarget& target) const {
   sf::RectangleShape wall({tile_size_, tile_size_});
   sf::RectangleShape floor({tile_size_, tile_size_});

   wall.setFillColor(sf::Color(20,20,30));
   floor.setFillColor(sf::Color(35,35,70));


   for (size_t i = 0; i < map_.size(); ++i) {
      for (size_t j = 0; j < map_[i].size(); ++j) {
         const sf::Vector2f cur_position{
            static_cast<float>(j) * tile_size_,
            static_cast<float>(i) * tile_size_
         };

         const char tile = map_[i][j];

         if (tile == '#') {
            wall.setPosition(cur_position);
            target.draw(wall);
         }
         else {
            floor.setPosition(cur_position);
            target.draw(floor);
         }
      }
   }
}
