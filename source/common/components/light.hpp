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
        int lightType;
        // for spot light we use both position and direction
        glm::vec3 direction;                                                                              // for directional
        glm::vec3 position;                                                                               // for point
        glm::vec3 color;                                                                                  // ambient, specular
        glm::vec3 attenuation;                                                                            // 1.0 / vec3(d*d, d, 1.0)                                                                         // da ehna bn3mlo 34an el light yed3f kol ma neb3d 3n el source
        glm::vec2 cone_angles = glm::vec2(0.5f * glm::quarter_pi<float>(), 0.5f * glm::half_pi<float>()); // this is used for the spot lighting, bara el cone hwa b 0, gowaha byzed l7d ma ywsl lel maximum 3la el cone axis.
        // el cone vec2 34an enta 3n

        // The ID of this component type is "Movement"
        static std::string getID() { return "Light"; }

        // Reads linearVelocity & angularVelocity from the given json object
        void deserialize(const nlohmann::json &data) override;
    };

}