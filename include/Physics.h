//#ifndef PHYSICS_H
//#define PHYSICS_H
//
//#include <SFML/Graphics.hpp>
//
//namespace bb {
//    class Entity;
//    class World;
//
//    class Physics {
//    public:
//        Physics(World& world);
//        void update(std::map<int, Entity*>& entities);
//        void drawHitboxes(sf::RenderWindow& window, std::map<int, Entity*>& entities);
//    private:
//        World& m_world;
//        sf::FloatRect getHitbox(Entity* entity);
//        sf::RectangleShape m_hitboxSprite;
//    };
//}
//
//#endif