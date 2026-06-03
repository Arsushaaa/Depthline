#pragma once

#include <SFML/Graphics.hpp>

#include "../Input/PlayerCommand.hpp"

class Player {
private:
   // временно персонаж - прямоугольник
   sf::RectangleShape player_;
   sf::Vector2f pos_;
   float speed_;

public:
   Player();

   sf::Vector2f GetPosition();
   void SetPosition(const sf::Vector2f& position);
   void Update(const PlayerCommand& command, float dt);
   void Render(sf::RenderTarget& target) const;

};