#include "entity.hpp"
#include "../deserialize-utils.hpp"

#include <glm/gtx/euler_angles.hpp>

/// @Author: Abdelaziz Salah Mohammed Abdou.
/// @Date: 24/3/2023
/// @Description: This file contains the implementation of the Transform class.
namespace our
{

    // This function computes and returns a matrix that represents this transform
    // Remember that the order of transformations is: Scaling, Rotation then Translation
    // HINT: to convert euler angles to a rotation matrix, you can use glm::yawPitchRoll
    glm::mat4 Transform::toMat4() const
    {
        // TODO: (Req 3) Write this function
        // first we need to apply the scaling
        glm::mat4 ScalingMat = glm::scale(glm::mat4(1.0f), scale); // scale is defined in the .hpp file.

        // then we need to apply the rotation using euler
        glm::mat4 RotationMat = glm::yawPitchRoll(rotation.y, rotation.x, rotation.z); // rotation is defined in the .hpp file.

        // after that we need to apply the translation
        glm::mat4 TranslationMat = glm::translate(ScalingMat, position); // position is defined in the .hpp file.

        // then we just need to return the Translation as it is the final matrix
        return TranslationMat;
    }

    // Deserializes the entity data and components from a json object
    void Transform::deserialize(const nlohmann::json &data)
    {
        position = data.value("position", position);
        rotation = glm::radians(data.value("rotation", glm::degrees(rotation)));
        scale = data.value("scale", scale);
    }

}