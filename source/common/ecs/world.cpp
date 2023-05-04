#include "world.hpp"

namespace our {

    // This will deserialize a json array of entities and add the new entities to the current world
    // If parent pointer is not null, the new entities will be have their parent set to that given pointer
    // If any of the entities has children, this function will be called recursively for these children
    void World::deserialize(const nlohmann::json &data, Entity *parent) {
        if (!data.is_array())
            return;
        for (const auto &entityData: data) {
            //(Req 8) Create an entity, make its parent "parent" and call its deserialize with "entityData".
            Entity *newEntity = add();           // create a new entity using the add function in world.hpp
            newEntity->parent = parent;          // set the parent of the new entity to the given parent
            newEntity->deserialize(entityData);  // deserialize the new entity using the given entityData
            if (entityData.contains("children")) // check if the entity has children
            {
                //(Req 8) Recursively call this world's "deserialize" using the children data
                //  and the current entity as the parent
                this->deserialize(entityData["children"], newEntity); // deserialize the children of the current entity
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
            if (entityData.contains("duplicates")) {
                for (int i = 1; i < 1000; ++i) {
                    Entity *newEntity = add();           // create a new entity using the add function in world.hpp
                    newEntity->parent = parent;          // set the parent of the new entity to the given parent
                    newEntity->deserialize(entityData);  // deserialize the new entity using the given entityData
                    newEntity->localTransform.position.x += -i * 5;
                }

            }
        }
    }

}