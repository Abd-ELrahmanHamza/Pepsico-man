#pragma once

#include "../ecs/entity.hpp"
#include "camera.hpp"
#include "mesh-renderer.hpp"
#include "free-camera-controller.hpp"
#include "movement.hpp"
#include "player.hpp"
#include "can.hpp"
#include "collision.hpp"
#include "obstacle.hpp"
#include "repeat.hpp"
#include "energy.hpp"
#include "final-line.hpp"
#include "heart.hpp"

namespace our {

    // Given a json object, this function picks and creates a component in the given entity
    // based on the "type" specified in the json object which is later deserialized from the rest of the json object
    inline void deserializeComponent(const nlohmann::json &data, Entity *entity) {
        std::string type = data.value("type", "");
        Component *component = nullptr;
        // (Req 8) Add an option to deserialize a "MeshRendererComponent" to the following if-else statement
        if (type == CameraComponent::getID()) {
            component = entity->addComponent<CameraComponent>();
        } else if (type == FreeCameraControllerComponent::getID()) {
            component = entity->addComponent<FreeCameraControllerComponent>();
        } else if (type == MovementComponent::getID()) {
            component = entity->addComponent<MovementComponent>();
        } else if (type == MeshRendererComponent::getID()) {    // check if the type is a mesh renderer component
            component = entity->addComponent<MeshRendererComponent>();  // add a mesh renderer component to the entity
        } else if (type == PlayerComponent::getID()) {    // check if the type is a player component
            component = entity->addComponent<PlayerComponent>();  // add a player component to the entity
        } else if (type == CanComponent::getID()) {    // check if the type is a can renderer component
            component = entity->addComponent<CanComponent>();  // add a can component to the entity
        } else if (type == CollisionComponent::getID()) {    // check if the type is a collision renderer component
            component = entity->addComponent<CollisionComponent>();  // add a collision component to the entity
        } else if (type == ObstacleComponent::getID()) {    // check if the type is a Obstacle renderer component
            component = entity->addComponent<ObstacleComponent>();  // add a Obstacle component to the entity
        } else if (type == RepeatComponent::getID()) {    // check if the type is a repeat renderer component
            component = entity->addComponent<RepeatComponent>();  // add a repeat component to the entity
        } else if (type == EnergyComponent::getID()) {    // check if the type is a energy renderer component
            component = entity->addComponent<EnergyComponent>();  // add a energy component to the entity
        }else if (type == FinalLineComponent::getID()) {    // check if the type is a FinalLine component
            component = entity->addComponent<FinalLineComponent>();  // add a FinalLine component to the entity
        }
        else if(type == HeartComponent::getID()){  // check if the type is a Heart component
            component = entity->addComponent<HeartComponent>(); // add a Heart component to the entity
        }
        if (component)
            component->deserialize(data);
    }

}