#include "Physics.h"
#include "Entity.h"
#include "World.h"

namespace bb {
    Physics::Physics(World& world) : m_world(world) {
    }

    void Physics::update(std::map<int, Entity*>& entities) {
        for(auto& entityA : entities) {
            auto* eA = entityA.second;
            if(!eA->m_isMovable) continue;
            sf::Vector2f newCoord = eA->m_coord;
            if(entityA.first != 0) {
                newCoord.x += eA->m_velocity.x;
                newCoord.y += eA->m_velocity.y;
                if(eA->m_velocity.x > 0) {
                    eA->m_velocity.x -= 0.1F;
                    if(eA->m_velocity.x < 0)
                        eA->m_velocity.x = 0;
                } else if(eA->m_velocity.x < 0) {
                    eA->m_velocity.x += 0.1F;
                    if(eA->m_velocity.x > 0)
                        eA->m_velocity.x = 0;
                }
                if(newCoord.y + eA->m_hitbox.top / 64.0F > 0) {
                    eA->m_velocity.y -= 0.05F;
                    eA->m_isOnGround = false;
                    if(eA->m_velocity.y < -3.0F) eA->m_velocity.y = -3.0F;
                } else if(newCoord.y + eA->m_hitbox.top / 64.0F <= 0) {
                    eA->m_velocity.y = 0;
                    newCoord.y = -eA->m_hitbox.top / 64.0F;
                    eA->m_isOnGround = true;
                }
            }
            sf::FloatRect hitboxA = {64 * newCoord.x + eA->m_hitbox.left, 540 - 64 * newCoord.y
                - eA->m_hitbox.top - eA->m_hitbox.height, eA->m_hitbox.width, eA->m_hitbox.height};
            m_world.getDebug().addLine(std::to_string(entityA.first) + ": " + std::to_string(hitboxA.left)
                + " " + std::to_string(hitboxA.top) + " " + std::to_string(hitboxA.width)
                + " " + std::to_string(hitboxA.height));
            m_world.getDebug().addLine(eA->m_isOnGround ? "On ground" : "Not on ground");
            for(auto& entityB : entities) {
                if(entityA.first == entityB.first) continue;
                auto* eB = entityB.second;
                sf::FloatRect hitboxB = {64 * eB->m_coord.x + eB->m_hitbox.left, 540 - 64 * eB->m_coord.y
                    - eB->m_hitbox.top - eB->m_hitbox.height, eB->m_hitbox.width, eB->m_hitbox.height};
                if(!(hitboxA.intersects(hitboxB))) continue;

                float bottomA = hitboxA.top + hitboxA.height;
                float bottomB = hitboxB.top + hitboxB.height;
                float rightA = hitboxA.left + hitboxA.width;
                float rightB = hitboxB.left + hitboxB.width;

                float bCol = bottomA - hitboxB.top;
                float tCol = bottomB - hitboxA.top;
                float lCol = rightB - hitboxA.left;
                float rCol = rightA - hitboxB.left;

                if(tCol < bCol && tCol < lCol && tCol < rCol) { //Top collision
                    newCoord.y = eB->m_coord.y + (eB->m_hitbox.top - eA->m_hitbox.height
                        - eA->m_hitbox.top) / 64.0F;
                    eA->m_velocity.y = 0.0F;
                } else if(bCol < tCol && bCol < lCol && bCol < rCol) { //bottom collision
                    newCoord.y = eB->m_coord.y + (eB->m_hitbox.top + eB->m_hitbox.height
                        - eA->m_hitbox.top) / 64.0F;
                    eA->m_velocity.y = 0.0F;
                    eA->m_isOnGround = true;
                } else if(lCol < rCol && lCol < tCol && lCol < bCol) { //Left collision
                    newCoord.x = eB->m_coord.x + (eB->m_hitbox.left + eB->m_hitbox.width
                        - eA->m_hitbox.left) / 64.0F;
                    eA->m_velocity.x = 0.0F;
                } else if(rCol < lCol && rCol < tCol && rCol < bCol) { //Right collision
                    newCoord.x = eB->m_coord.x + (eB->m_hitbox.left - eA->m_hitbox.width
                        - eA->m_hitbox.left) / 64.0F;
                    eA->m_velocity.x = 0.0F;
                }
            }
            eA->m_coord = newCoord;
        }
    }
}