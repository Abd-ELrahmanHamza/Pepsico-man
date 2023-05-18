#pragma once

#include "../ecs/world.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/trigonometric.hpp>
#include <glm/gtx/fast_trigonometry.hpp>
#include "../application.hpp"

namespace our {

    // The movement system is responsible for moving every entity which contains a MovementComponent.
    // This system is added as a simple example for how use the ECS framework to implement logic.
    // For more information, see "common/components/movement.hpp"
    class RepeatSystem {
    public:
        Application *app; // The application in which the state runs

        // This should be called every frame to update all entities containing a MovementComponent.
        void update(World *world, float deltaTime,int level);
    };

}