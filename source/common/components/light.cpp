#include "light.hpp"
#include "../ecs/entity.hpp"
#include "../deserialize-utils.hpp"

namespace our
{
    // Reads linearVelocity & angularVelocity from the given json object
    void LightComponent::deserialize(const nlohmann::json &data)
    {
        if (!data.is_object())
            return;
        lightType = data.value("lightType", lightType);
        position = data.value("position", position);    // da 34an el point light, akno el source position.
        direction = data.value("direction", direction); // el direction elly ell light ray7 feh, da lel directional light
        color = data.value("color", color);
        attenuation = data.value("attenuation", attenuation); // da ehna bn3mlo 34an el light yed3f kol ma neb3d 3n el source
    }
}