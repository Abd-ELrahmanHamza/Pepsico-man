#include "light.hpp"
#include "../ecs/entity.hpp"
#include "../deserialize-utils.hpp"

namespace our
{
    void LightComponent::deserialize(const nlohmann::json &data)
    {
        // Check if the JSON data is an object
        if (!data.is_object())
            return;

        // Read the "lightType" value from the JSON object or use the default value from the member variable
        lightType = data.value("lightType", lightType);

        // Read the "position" value from the JSON object or use the default value from the member variable
        position = data.value("position", position);

        // Read the "direction" value from the JSON object or use the default value from the member variable
        direction = data.value("direction", direction);

        // Read the "color" value from the JSON object or use the default value from the member variable
        color = data.value("color", color);

        // Read the "attenuation" value from the JSON object or use the default value from the member variable
        attenuation = data.value("attenuation", attenuation);

        // Read the "cone_angles" value from the JSON object or use the default value from the member variable
        cone_angles = data.value("cone_angles", cone_angles);
    }
}