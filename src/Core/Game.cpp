#include "Depthline/Core/Game.hpp"

#include <algorithm>
#include <SFML/System/Clock.hpp>
#include <SFML/Window/Event.hpp>

constexpr float maxDt = 0.5f;

Game::Game()
   : window_(
      sf::VideoMode::getDesktopMode(),
      "Depthline",
      sf::State::Fullscreen
   ),
   camera_(sf::FloatRect({0.f, 0.f}, {1280.f, 720.f}))
{
   camera_.setCenter(world_.GetPlayer().GetPosition());
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

   world_.Render(window_);

   window_.display();
}

void Game::Update(float dt) {
   PlayerCommand command = ReadPlayerInput(player_controls_);

   const sf::Vector2i mouse_pixel = 
      sf::Mouse::getPosition(window_);

   command.aim_world_position = 
      window_.mapPixelToCoords(mouse_pixel, camera_);

   world_.Update(command, dt);

   camera_.setCenter(world_.GetPlayer().GetPosition());
}

void Game::ProcessEvents() {
   while (const std::optional event = window_.pollEvent()) {
      if (event->is<sf::Event::Closed>()) {
         window_.close();
      }
   }
}
