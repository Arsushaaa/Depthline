#pragma once

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Clock.hpp>

#include "../World/World.hpp"

class Game {
private:
   sf::RenderWindow window_;
   sf::Clock clock_;
   sf::View camera_;
   
   PlayerControls player_controls_;
   World world_;

   void ProcessEvents();
   void Update(float dt);
   void Render();

public:
   Game();

   int Run();
};