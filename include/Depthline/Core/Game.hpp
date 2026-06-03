#pragma once

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Clock.hpp>

#include "../World/TileMap.hpp"
#include "../Entities/Player.hpp"

class Game {
private:
   sf::RenderWindow window_;
   sf::Clock clock_;
   sf::View camera_;
   

   Player player_;
   PlayerControls player_controls_;
   TileMap map_;

   void ProcessEvents();
   void Update(float dt);
   void Render();

public:
   Game();


   int Run();

};