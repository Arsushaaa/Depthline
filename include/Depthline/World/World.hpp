#pragma once


#include <SFML/Graphics.hpp>


#include "../Entities/Player.hpp"
#include "../Input/PlayerCommand.hpp"
#include "../World/TileMap.hpp"

class World {
private:
   Player player_;
   TileMap map_;

public:
   World();

   const Player& GetPlayer();
   void Update(const PlayerCommand& command, float dt);
   void Render(sf::RenderTarget& target);
};