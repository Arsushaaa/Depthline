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
               .visual = std::make_unique<HitboxAttackVisual>(
                  sf::Color(255, 255, 255, 50),
                  0.04f
               )
            }
         );
      }
      case WeaponType::ContactBase: {
         return Weapon(
            WeaponComponents{
               .attack_pattern = std::make_unique<ContactAttackPattern>(
                  25,
                  8.0f
               ),
               .limiter = std::make_unique<CooldownLimiter>(1.0f),
               .visual = std::make_unique<HitboxAttackVisual>(
                  sf::Color(255, 0, 255),
                  0.06f
               )
            }
         );
      }
   }

   throw std::runtime_error("Unknown weapon type");
}