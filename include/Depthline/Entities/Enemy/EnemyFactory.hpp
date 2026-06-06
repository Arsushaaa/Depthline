#pragma once

#include <SFML/System/Vector2.hpp>

#include "Depthline/Entities/Enemy/Enemy.hpp"


enum class EnemyType {
   Chaser,
   Shooter,
   Jumper
};

class EnemyFactory {
public:
   static Enemy Create(EnemyType type, const sf::Vector2f& position);
};
