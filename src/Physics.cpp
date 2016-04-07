//#include "Physics.h"
//#include "Entity.h"
//#include "World.h"
//
//namespace bb {
//    Physics::Physics(World& world) : m_world(world) {
//        m_hitboxSprite.setFillColor(sf::Color::Transparent);
//        m_hitboxSprite.setOutlineColor(sf::Color::Magenta);
//        m_hitboxSprite.setOutlineThickness(1.0F);
//    }
//
//    void Physics::update(std::map<int, Entity*>& entities) {
//        for(auto& entityA : entities) {
//            auto* eA = entityA.second;
//            if(!eA->m_isMovable) continue;
//            sf::Vector2f newCoord = eA->m_coord;
//            if(entityA.first != 0) {
//                newCoord.x += eA->m_velocity.x;
//                newCoord.y += eA->m_velocity.y;
//                if(eA->m_velocity.x > 0) {
//                    eA->m_velocity.x -= 0.1F;
//                    if(eA->m_velocity.x < 0)
//                        eA->m_velocity.x = 0;
//                } else if(eA->m_velocity.x < 0) {
//                    eA->m_velocity.x += 0.1F;
//                    if(eA->m_velocity.x > 0)
//                        eA->m_velocity.x = 0;
//                }
//                if(newCoord.y + eA->m_hitbox.top / 64.0F > 0) {
//                    eA->m_velocity.y -= 0.05F;
//                    eA->m_isOnGround = false;
//                    if(eA->m_velocity.y < -3.0F) eA->m_velocity.y = -3.0F;
//                } else if(newCoord.y + eA->m_hitbox.top / 64.0F <= 0) {
//                    eA->m_velocity.y = 0;
//                    newCoord.y = -eA->m_hitbox.top / 64.0F;
//                    eA->m_isOnGround = true;
//                }
//            }
//            sf::FloatRect hitboxA = getHitbox(eA);
//            m_world.getDebug().addLine(std::to_string(entityA.first) + ": " + std::to_string(hitboxA.left)
//                + " " + std::to_string(hitboxA.top) + " " + std::to_string(hitboxA.width)
//                + " " + std::to_string(hitboxA.height));
//            m_world.getDebug().addLine(eA->m_isOnGround ? "On ground" : "Not on ground");
//            for(auto& entityB : entities) {
//                if(entityA.first == entityB.first) continue;
//                auto* eB = entityB.second;
//                sf::FloatRect hitboxB = getHitbox(eB);
//                if(!(hitboxA.intersects(hitboxB))) continue;
//
//                float bottomA = hitboxA.top + hitboxA.height;
//                float bottomB = hitboxB.top + hitboxB.height;
//                float rightA = hitboxA.left + hitboxA.width;
//                float rightB = hitboxB.left + hitboxB.width;
//
//                float bCol = bottomA - hitboxB.top;
//                float tCol = bottomB - hitboxA.top;
//                float lCol = rightB - hitboxA.left;
//                float rCol = rightA - hitboxB.left;
//
//                if(eA->m_velocity.x > 0) { // right movement
//                    if(eA->m_velocity.y > 0) { // right & up movement
//                        if(tCol < bCol && tCol < lCol && tCol < rCol) { //top collision
//                            newCoord.y = eB->m_coord.y + (eB->m_hitbox.top - eA->m_hitbox.height
//                                - eA->m_hitbox.top) / 64.0F;
//                            eA->m_velocity.y = 0.0F;
//                        } else if(rCol < lCol && rCol < tCol && rCol < bCol) { //right collision
//                            newCoord.x = eB->m_coord.x + (eB->m_hitbox.left - eA->m_hitbox.width
//                                - eA->m_hitbox.left) / 64.0F;
//                            eA->m_velocity.x = 0.0F;
//                        }
//                    } else if(eA->m_velocity.y < 0) { // right & down movement
//                        if(bCol < tCol && bCol < lCol && bCol < rCol) { //bottom collision
//                            newCoord.y = eB->m_coord.y + (eB->m_hitbox.top + eB->m_hitbox.height
//                                - eA->m_hitbox.top) / 64.0F;
//                            eA->m_isOnGround = true;
//                            eA->m_velocity.y = 0.0F;
//                        } else if(rCol < lCol && rCol < tCol && rCol < bCol) { // right collision
//                            newCoord.x = eB->m_coord.x + (eB->m_hitbox.left - eA->m_hitbox.width
//                                - eA->m_hitbox.left) / 64.0F;
//                            eA->m_velocity.x = 0.0F;
//                        }
//                    } else { // right collision
//                        newCoord.x = eB->m_coord.x + (eB->m_hitbox.left - eA->m_hitbox.width
//                            - eA->m_hitbox.left) / 64.0F;
//                        eA->m_velocity.x = 0.0F;
//                    }
//                } else if(eA->m_velocity.x < 0) { //left movement
//                    if(eA->m_velocity.y > 0) { // left & up movement
//                        if(tCol < bCol && tCol < lCol && tCol < rCol) { // top collision
//                            newCoord.y = eB->m_coord.y + (eB->m_hitbox.top - eA->m_hitbox.height
//                                - eA->m_hitbox.top) / 64.0F;
//                            eA->m_velocity.y = 0.0F;
//                        } else if(lCol < rCol && lCol < tCol && lCol < bCol) { // left collision
//                            newCoord.x = eB->m_coord.x + (eB->m_hitbox.left + eB->m_hitbox.width
//                                - eA->m_hitbox.left) / 64.0F;
//                            eA->m_velocity.x = 0.0F;
//                        }
//                    } else if(eA->m_velocity.y < 0) { // left & down movement
//                        if(bCol < tCol && bCol < lCol && bCol < rCol) { // bottom collision
//                            newCoord.y = eB->m_coord.y + (eB->m_hitbox.top + eB->m_hitbox.height
//                                - eA->m_hitbox.top) / 64.0F;
//                            eA->m_isOnGround = true;
//                            eA->m_velocity.y = 0.0F;
//                        } else if(lCol < rCol && lCol < tCol && lCol < bCol) { // left collision
//                            newCoord.x = eB->m_coord.x + (eB->m_hitbox.left + eB->m_hitbox.width
//                                - eA->m_hitbox.left) / 64.0F;
//                            eA->m_velocity.x = 0.0F;
//                        }
//                    } else { // left collision
//                        newCoord.x = eB->m_coord.x + (eB->m_hitbox.left + eB->m_hitbox.width
//                            - eA->m_hitbox.left) / 64.0F;
//                        eA->m_velocity.x = 0.0F;
//                    }
//                } else { // up or down movement
//                    if(eA->m_velocity.y > 0) { // top collision
//                        newCoord.y = eB->m_coord.y + (eB->m_hitbox.top - eA->m_hitbox.height
//                            - eA->m_hitbox.top) / 64.0F;
//                        eA->m_velocity.y = 0.0F;
//                    } else if(eA->m_velocity.y < 0) { // bottom collision
//                        newCoord.y = eB->m_coord.y + (eB->m_hitbox.top + eB->m_hitbox.height
//                            - eA->m_hitbox.top) / 64.0F;
//                        eA->m_isOnGround = true;
//                        eA->m_velocity.y = 0.0F;
//                    }
//                }
//
//
//                //m_world.getDebug().addLine(std::to_string(std::abs(eA->m_velocity.x)) + " "
//                //    + std::to_string(std::abs(eA->m_velocity.y)));
//                //if(std::abs(eA->m_velocity.x) > std::abs(eA->m_velocity.y)) {
//                //    if(eA->m_velocity.x < 0) { // Leftward movement
//
//                //    } else if(eA->m_velocity.x > 0) { // Rightward movement
//                //
//                //    }
//                //} else if(std::abs(eA->m_velocity.x) < std::abs(eA->m_velocity.y)) {
//                //    if(eA->m_velocity.y < 0) { // Downward movement
//                //
//                //    } else if(eA->m_velocity.y > 0) { // Upward movement
//                //
//                //    }
//                //
//                //}
//
//                //if(tCol < bCol && tCol < lCol && tCol < rCol) { //Top collision
//
//                //} else if(bCol < tCol && bCol < lCol && bCol < rCol) { //bottom collision
//
//                //} else if(lCol < rCol && lCol < tCol && lCol < bCol) { //Left collision
//
//                //} else if(rCol < lCol && rCol < tCol && rCol < bCol) { //Right collision
//
//                //}
//            }
//            eA->m_coord = newCoord;
//        }
//    }
//
//    void Physics::drawHitboxes(sf::RenderWindow& window, std::map<int, Entity*>& entities) {
//        for(auto& entity : entities) {
//            auto* e = entity.second;
//            auto hitbox = getHitbox(e);
//            m_hitboxSprite.setPosition({hitbox.left, hitbox.top});
//            m_hitboxSprite.setSize({hitbox.width, hitbox.height});
//            window.draw(m_hitboxSprite);
//        }
//    }
//
//    sf::FloatRect Physics::getHitbox(Entity * e) {
//        return{64 * e->m_coord.x + e->m_hitbox.left, 540 - 64 * e->m_coord.y
//            - e->m_hitbox.top - e->m_hitbox.height, e->m_hitbox.width, e->m_hitbox.height};
//    }
//}