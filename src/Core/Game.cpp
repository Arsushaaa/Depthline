#include "Depthline/Core/Game.hpp"

#include <algorithm>
#include <SFML/System/Clock.hpp>
#include <SFML/Window/Event.hpp>

#include "Depthline/World/Maps.hpp"

constexpr float maxDt = 0.5f;

Game::Game()
   : window_(sf::VideoMode({1980,1080}), "Depthline"),
   camera_(sf::FloatRect({0.f, 0.f}, {1280.f, 720.f})),
   map_(64.0f)
{
   map_.LoadFromStrings(map1);
   player_.SetPosition(map_.GetPlayerSpawn());
   camera_.setCenter(player_.GetPosition());
}

int Game::Run() {
   while (window_.isOpen()) {
      const float dt = std::min(clock_.restart().asSeconds(), maxDt);

      ProcessEvents();
      Update(dt);
      Render();
   }

   return 0;
}

void Game::Render() {
   window_.clear();

   window_.setView(camera_);

   map_.Render(window_);
   player_.Render(window_);

   window_.display();
}

void Game::Update(float dt) {
   const PlayerCommand command = ReadPlayerInput(player_controls_);

   player_.Update(command, dt);

   camera_.setCenter(player_.GetPosition());
}

void Game::ProcessEvents() {
   while (const std::optional event = window_.pollEvent()) {
      if (event->is<sf::Event::Closed>()) {
         window_.close();
      }
   }
}
