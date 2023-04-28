#include "entity.hpp"
#include "../deserialize-utils.hpp"
#include "../components/component-deserializer.hpp"

#include <glm/gtx/euler_angles.hpp>

namespace our
{

    // This function returns the transformation matrix from the entity's local space to the world space
    // Remember that you can get the transformation matrix from this entity to its parent from "localTransform"
    // To get the local to world matrix, you need to combine this entities matrix with its parent's matrix and
    // its parent's parent's matrix and so on till you reach the root.
    glm::mat4 Entity::getLocalToWorldMatrix() const
    {
        //(Req 8) Write this function
        glm::mat4 finalMat = localTransform.toMat4(); // get the local to world matrix of this entity
        Entity *Parent = parent;
        while (Parent)
        {                                                          // check if there is a parent
            finalMat = parent->localTransform.toMat4() * finalMat; // multiply the parent's local to world matrix with the current matrix
            Parent = Parent->parent;                               // get parent of parent
        }
        return finalMat; // return the final matrix
    }

    // Deserializes the entity data and components from a json object
    void Entity::deserialize(const nlohmann::json &data)
    {
        if (!data.is_object())
            return;
        name = data.value("name", name);
        localTransform.deserialize(data);
        if (data.contains("components"))
        {
            if (const auto &components = data["components"]; components.is_array())
            {
                for (auto &component : components)
                {
                    deserializeComponent(component, this);
                }
            }
        }
    }

}