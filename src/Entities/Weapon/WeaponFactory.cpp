#include "Depthline/Entities/Weapon/WeaponFactory.hpp"

#include <memory>
#include <stdexcept>

#include <SFML/System/Vector2.hpp>

#include "Depthline/Entities/Weapon/Weapon.hpp"

Weapon WeaponFactory::Create(WeaponType type) {
   switch (type) {
      case WeaponType::SwordBase: {
         return Weapon(
            WeaponComponents {
               .attack_pattern = std::make_unique<BaseSwordPattern>(
                  25,
                  35.0f,
                  sf::Vector2f{35.0f, 35.0f}
               ),
               .limiter = std::make_unique<CooldownLimiter>(0.5f),
               .visual = std::make_unique<BaseSwordVisual>(
                  sf::Vector2f{35.0f, 35.0f},
                  sf::Color(255, 255, 255, 50)
               )
            }
         );
      }
   }

   throw std::runtime_error("Unknown weapon type");
}