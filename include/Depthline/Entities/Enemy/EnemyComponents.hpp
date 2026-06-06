#pragma once

#include <memory>

#include <SFML/Graphics.hpp>


struct EnemyBehaviorContext {
   sf::Vector2f self_position;
   sf::Vector2f target_position;
   float dt = 0.0f;
};

struct EnemyIntent {
   sf::Vector2f move_direction{0.0f, 0.0f};
   sf::Vector2f aim_direction{0.0f, 0.0f};
   float speed_multiplier = 1.0f;
   bool wants_to_attack = false;
};


class IEnemyBehavior {
public:
   virtual ~IEnemyBehavior() = default;

   virtual EnemyIntent Decide(const EnemyBehaviorContext& context) = 0;
};

class IEnemyVisual {
public:
   virtual ~IEnemyVisual() = default;

   virtual void SetPosition(const sf::Vector2f& position) = 0;
   virtual void SetFacing(const sf::Vector2f&) {};
   virtual void Update(float) {};
   virtual void Render(sf::RenderTarget& target) const = 0;
};

class IEnemyCollider {
public:
   virtual ~IEnemyCollider() = default;

   virtual sf::FloatRect GetBoundsAt(const sf::Vector2f& position) const = 0;
};


struct EnemyComponents {
   std::unique_ptr<IEnemyBehavior> behavior;
   std::unique_ptr<IEnemyVisual> visual;
   std::unique_ptr<IEnemyCollider> collider;
};


// ----------------------------------------------------------------
//                         реализации
// ----------------------------------------------------------------


class ChaserBehavior : public IEnemyBehavior {
public:
   EnemyIntent Decide(const EnemyBehaviorContext& context) override;
};

class RectangleEnemyVisual : public IEnemyVisual {
private:
   sf::RectangleShape shape_;

public:
   RectangleEnemyVisual(sf::Vector2f size, sf::Vector2f origin, sf::Color color);
   
   void SetPosition(const sf::Vector2f& position) override;
   void Render(sf::RenderTarget& target) const override;
};

class BoxEnemyCollider : public IEnemyCollider {
private:
   sf::Vector2f size_;
   sf::Vector2f origin_;

public:
   BoxEnemyCollider(sf::Vector2f size, sf::Vector2f origin);

   sf::FloatRect GetBoundsAt(const sf::Vector2f& position) const override;
};