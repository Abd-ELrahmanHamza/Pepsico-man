#pragma once

#include "../ecs/component.hpp"

#include <glm/gtx/euler_angles.hpp>
#include <glm/glm.hpp>

namespace our
{

    // This component denotes that the MovementSystem will move the owning entity by a certain linear and angular velocity.
    // This component is added as a simple example for how use the ECS framework to implement logic.
    // For more information, see "common/systems/movement.hpp"
    // For a more complex example of how to use the ECS framework, see "free-camera-controller.hpp"
    class LightComponent : public Component
    {
    public:
        // data members :
        /*
           1. type : 0 for directional light, 1 for point light, 2 for spot light
        */
        int lightType;         // Type of the light: 0 for directional, 1 for point, 2 for spot
        glm::vec3 direction;   // Direction of the light (for directional light)
        glm::vec3 position;    // Position of the light (for point light)
        glm::vec3 color;       // Ambient and specular color of the light
        glm::vec3 attenuation; // Attenuation factors for the light (controls falloff)
        glm::vec2 cone_angles; // Cone angles for spot lighting (inner and outer angles)

        // The ID of this component type is "Light"
        static std::string getID() { return "Light"; }

        // Reads light component data from the given JSON object
        void deserialize(const nlohmann::json &data) override;
    };

}