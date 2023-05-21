#pragma once

#include "../ecs/world.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/trigonometric.hpp>
#include <glm/gtx/fast_trigonometry.hpp>
#include "../application.hpp"

namespace our {

    // The repeat system is responsible for repeating the entities.
    class RepeatSystem {
    public:
        Application *app; // The application in which the state runs

        // This should be called every frame to update all entities containing a MovementComponent.
        void update(World *world, float deltaTime,int level);
    };

}
