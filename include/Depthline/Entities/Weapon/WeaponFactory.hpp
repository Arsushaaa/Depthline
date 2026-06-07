#pragma once

#include "Depthline/Entities/Weapon/Weapon.hpp"

enum class WeaponType {
   SwordBase
};

class WeaponFactory {
public:
   static Weapon Create(WeaponType type);
};