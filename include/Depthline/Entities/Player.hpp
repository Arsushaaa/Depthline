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

   const sf::Vector2f GetPosition() const;
   void SetPosition(const sf::Vector2f& position);
   sf::Vector2f CalculateMovement(const PlayerCommand& command, float dt) const;
   sf::FloatRect GetBoundsAt(const sf::Vector2f& position) const;
   void Render(sf::RenderTarget& target) const;

};