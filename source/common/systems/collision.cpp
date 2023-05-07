//
// Created by Lenovo on 5/5/2023.
//

#include "collision.hpp"

#pragma once

#include "../ecs/world.hpp"
#include "../components/collision.hpp"
#include "../components/player.hpp"
#include "../components/can.hpp"
#include "../components/obstacle.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/trigonometric.hpp>
#include <glm/gtx/fast_trigonometry.hpp>
#include <iostream>

#include "../mesh/mesh-utils.hpp"
#include "../texture/texture-utils.hpp"

#include "../application.hpp"

namespace our {
    void CollisionSystem::update(World *world, float deltaTime) {
        PlayerComponent *player;
        glm::vec3 playerPosition;
        Entity *playerEntity;
        for (auto entity: world->getEntities()) {
            // Get the player component if it exists
            playerEntity = entity;
            player = playerEntity->getComponent<PlayerComponent>();
            // If the player component exists
            if (player) {
                playerPosition =
                        glm::vec3(playerEntity->getLocalToWorldMatrix() *
                                  glm::vec4(playerEntity->localTransform.position, 1.0));
                break;
            }
        }
        if (!player) {
            return;
        }

        glm::vec3 playerStart = playerEntity->getComponent<CollisionComponent>()->start + playerPosition;
        glm::vec3 playerEnd = playerEntity->getComponent<CollisionComponent>()->end + playerPosition;

        // For each entity in the world
        for (auto entity: world->getEntities()) {
            // Get the collision component if it exists
            CollisionComponent *collision = entity->getComponent<CollisionComponent>();
            // If the collision component exists
            if (collision) {
                if (entity->getComponent<CanComponent>()) {
                    auto canPosition = entity->localTransform.position;
                    glm::vec3 canStart = collision->start + canPosition;
                    glm::vec3 canEnd = collision->end + canPosition;
                    bool collided = true;
                    for (int i = 0; i < 3; ++i) {
                        if (playerStart[i] > canEnd[i] || playerEnd[i] < canStart[i]) {
//                            if (app->getKeyboard().isPressed(GLFW_KEY_SPACE)) {
//                                std::cout << i << std::endl;
//                                std::cout << playerStart[i] << " " << playerEnd[i] << std::endl;
//                                std::cout << canStart[i] << " " << canEnd[i] << std::endl;
//                            }
                            collided = false;
                            break;
                        }
                    }
                    if (collided) {
                        world->markForRemoval(entity);
                        world->deleteMarkedEntities();
                        break;
                    }
                }
                else if(entity->getComponent<ObstacleComponent>())
                {
                    auto obstaclePosition = glm::vec3(entity->getLocalToWorldMatrix()* glm::vec4( entity->localTransform.position, 1.0));
                    //auto obstaclePosition = entity->localTransform.position;
                    glm::vec3 obstacleStart = collision->start + obstaclePosition;
                    glm::vec3 obstacleEnd = collision->end + obstaclePosition;
                    bool collided = true;
                    for (int i = 0; i < 3; ++i) {
                        if (playerStart[i] > obstacleEnd[i] || playerEnd[i] < obstacleStart[i]) {
//                            if (app->getKeyboard().isPressed(GLFW_KEY_X)) {
//                                std::cout << i << std::endl;
//                                std::cout << obstaclePosition[0] << " " << obstaclePosition[1] <<" "<< obstaclePosition[2]<< std::endl;
//                                std::cout << playerStart[0] << " " << playerStart[1] <<" "<< playerStart[2]<< std::endl;
//                                std::cout << playerEnd[0] << " " << playerEnd[1] <<" "<< playerEnd[2]<< std::endl;
//
//                                std::cout << obstacleStart[0] << " " << obstacleStart[1] <<" "<< obstacleStart[2]<< std::endl;
//                                std::cout << obstacleEnd[0] << " " << obstacleEnd[1] <<" "<< obstacleEnd[2]<< std::endl;
//                                //std::cout << obstacleStart[i] << " " << obstacleEnd[i] << std::endl;
//                            }
                            collided = false;
                            break;
                        }
                    }
                    if (collided) {
                        world->markForRemoval(entity);
                        world->deleteMarkedEntities();
                        break;
                    }

                }
            }
        }
    }
}