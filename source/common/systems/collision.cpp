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
#include "../components/repeat.hpp"
#include "../components/energy.hpp"
#include "../components/heart.hpp"
#include "../components/gem-heart.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/trigonometric.hpp>
#include <glm/gtx/fast_trigonometry.hpp>
#include <iostream>

#include "../mesh/mesh-utils.hpp"
#include "../texture/texture-utils.hpp"

#include "../application.hpp"


namespace our {
    void CollisionSystem::update(World *world, float deltaTime, int &countPepsi, int &heartCount, bool isSlided,
                                 float &collisionStartTime) {
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
                // auto objectPosition =  glm::vec3(entity->getLocalToWorldMatrix() * glm::vec4(entity->localTransform.position,1.0) );
                auto objectPosition = entity->localTransform.position;
                glm::vec3 objectStart = collision->start + objectPosition;
                glm::vec3 objectEnd = collision->end + objectPosition;
                if (isSlided) {
                    playerStart.y = -1;
                    playerEnd.y = 0.5;
                }
                bool collided = true;
                for (int i = 0; i < 3; ++i) {
                    if (playerStart[i] > objectEnd[i] || playerEnd[i] < objectStart[i]) {
                        collided = false;
                        break;
                    }
                }
                if (collided) {
                    // std::cout << "collision" << std::endl;
                    if (entity->getComponent<ObstacleComponent>()) {
                        if (collisionStartTime == 0)
                            collisionStartTime = deltaTime;
#ifdef USE_SOUND
                        std::cout << "sounnnnnnnnnd\n";
                        if (soundEngine->isCurrentlyPlaying("audio/collision.mp3"))
                            soundEngine->stopAllSounds();
                        soundEngine->play2D("audio/obstacle.mp3");
                        soundEngine->play2D("audio/collision.mp3");
#endif

                        // std::cout << "player position: x = " << playerPosition.x << ", y= " << playerPosition.y << ", z = "<< playerPosition.z <<  std::endl;
                        // std::cout << "ostacle position: x = " << objectPosition.x << ", y= " << objectPosition.y << ", z = "<< objectPosition.z <<  std::endl;
                        // std::cout << "playerStart position: x = " << playerStart.x << ", y= " << playerStart.y << ", z = "<< playerStart.z <<  std::endl;
                        // std::cout << "objectStart position: x = " << objectStart.x << ", y= " << objectStart.y << ", z = "<< objectStart.z <<  std::endl;
                        // std::cout << "playerEnd position: x = " << playerEnd.x << ", y= " << playerEnd.y << ", z = "<< playerEnd.z <<  std::endl;
                        // std::cout << "objectEnd position: x = " << objectEnd.x << ", y= " << objectEnd.y << ", z = "<< objectEnd.z <<  std::endl;

#ifdef USE_SOUND
                        std::cout << "heartCount : " << heartCount << "\n";
                        if (heartCount == 3) {
                            soundEngine->play2D("audio/firstDeath.mp3");
                        } else if (heartCount == 2) {
                            soundEngine->play2D("audio/secondDeath.mp3");
                        }
#endif
                        CollisionSystem::decreaseHearts(world, heartCount);

                        // if (heartCount < 1) {
                        //     std::cout << "game over " << countPepsi << std::endl;
                        //     soundEngine->play2D("audio/death.mp3");
                        //     app->changeState("game-over");
                        // }
//                        countPepsi -= 5;
                        // if (countPepsi < 0) {
                        //     std::cout << "game over " << countPepsi << std::endl;
                        //     soundEngine->play2D("audio/death.mp3");
                        //     app->changeState("game-over");
                        // }
                    } else if (entity->getComponent<CanComponent>()) {
#ifdef USE_SOUND
                        soundEngine->play2D("audio/can.wav");
#endif
                        if (countPepsi < 100) {
                            countPepsi++;
                        }
                    }
                    else if(entity->getComponent<GemHeartComponent>())
                    {
                        if(heartCount < 3)
                        {
                            heartCount++;
                        }
                        
                        entity->localTransform.scale = glm::vec3(0.0f, 0.0f, 0.0f);
                        entity->localTransform.position = glm::vec3(0.0f, 0.0f, 0.0f);
                        for (auto heartEntity: world->getEntities()) {
                            HeartComponent *heart = heartEntity->getComponent<HeartComponent>();
                            if (heart && heart->heartNumber == heartCount) {
                                // rescale energy bar with one unit and move position
                                // std::cout << "heartCount: " << heartCount << std::endl;
                                heartEntity->localTransform.scale.x = 0.0009;
                                heartEntity->localTransform.scale.y = 0.0009;
                                heartEntity->localTransform.scale.z = 0.0009;
                                break;
                            }
                        }
                    }
                    for (auto energybar: world->getEntities()) {
                        EnergyComponent *energy = energybar->getComponent<EnergyComponent>();
                        if (energy) {
                            // rescale energy bar with one unit and move position
                            if (countPepsi < 101) {
                                //countPepsi++;
                                // std::cout << "countPepsi: " << countPepsi << std::endl;
                                energybar->localTransform.scale.x = (double) 0.145 * (double) (countPepsi / 100.0);
                                energybar->localTransform.position.x = -0.142 + 0.145 * (countPepsi / 100.0);
                                // std::cout << "scale: " << energybar->localTransform.scale.x << std::endl;
                            }
                            break;
                        }
                    }
                    RepeatComponent *repeatComponent = entity->getComponent<RepeatComponent>();
                    glm::vec3 &repeatPosition = entity->localTransform.position;
                    // If the player component exists
                    if (repeatComponent) {
                        repeatPosition += repeatComponent->translation;
                    }
                    break;
                }
            }
        }
    }

    void CollisionSystem::decreaseHearts(World *world, int &heartCount) {

        for (auto heartEntity: world->getEntities()) {
            HeartComponent *heart = heartEntity->getComponent<HeartComponent>();
            if (heart && heart->heartNumber == heartCount) {
                // rescale energy bar with one unit and move position
                // std::cout << "heartCount: " << heartCount << std::endl;
                heartEntity->localTransform.scale.x = 0.0;
                heartEntity->localTransform.scale.y = 0.0;
                heartEntity->localTransform.scale.z = 0.0;
                heartCount--;
                break;
            }
        }
    }
}