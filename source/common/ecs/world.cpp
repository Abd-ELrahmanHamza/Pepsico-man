#include "world.hpp"

namespace our
{

    // This will deserialize a json array of entities and add the new entities to the current world
    // If parent pointer is not null, the new entities will be have their parent set to that given pointer
    // If any of the entities has children, this function will be called recursively for these children
    void World::deserialize(const nlohmann::json &data, Entity *parent)
    {
        if (!data.is_array())
            return;
        for (const auto &entityData : data)
        {
            // TODO: (Req 8) Create an entity, make its parent "parent" and call its deserialize with "entityData".
            Entity *newEntity = add();
            newEntity->parent = parent;
            newEntity->deserialize(entityData);
            if (entityData.contains("children"))
            {
                // TODO: (Req 8) Recursively call this world's "deserialize" using the children data
                //  and the current entity as the parent
                this->deserialize(entityData["children"], newEntity);
                // if (const auto &children = entityData["children"]; children.is_array())
                // {
                //     for (auto &child : children)
                //     {
                //         this->deserialize(child, newEntity);
                //     }
                // }
                // else
                // {
                //     this->deserialize(entityData["children"], newEntity);
                // }
            }
        }
    }

}