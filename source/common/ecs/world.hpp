#pragma once

#include <unordered_set>
#include "entity.hpp"

namespace our
{

    // This class holds a set of entities
    class World
    {    public:
        std::unordered_set<Entity *> entities;         // These are the entities held by this world
        std::unordered_set<Entity *> markedForRemoval; // These are the entities that are awaiting to be deleted
        // when deleteMarkedEntities is called

        World() = default;

        // This will deserialize a json array of entities and add the new entities to the current world
        // If parent pointer is not null, the new entities will be have their parent set to that given pointer
        // If any of the entities has children, this function will be called recursively for these children
        void deserialize(const nlohmann::json &data, Entity *parent = nullptr);

        // This adds an entity to the entities set and returns a pointer to that entity
        // WARNING The entity is owned by this world so don't use "delete" to delete it, instead, call "markForRemoval"
        // to put it in the "markedForRemoval" set. The elements in the "markedForRemoval" set will be removed and
        // deleted when "deleteMarkedEntities" is called.
        Entity *add()
        {
            //(Req 8) Create a new entity, set its world member variable to this,
            // and don't forget to insert it in the suitable container.
            Entity *newEntity = new Entity(); // create a new entity
            newEntity->world = this;          // set the world of the new entity to this
            entities.insert(newEntity);       // insert the new entity into the entities set
            return newEntity;                 // return the new entity
        }

        // This returns and immutable reference to the set of all entites in the world.
        const std::unordered_set<Entity *> &getEntities()
        {
            return entities;
        }

        // This marks an entity for removal by adding it to the "markedForRemoval" set.
        // The elements in the "markedForRemoval" set will be removed and deleted when "deleteMarkedEntities" is called.
        void markForRemoval(Entity *entity)
        {
            //(Req 8) If the entity is in this world, add it to the "markedForRemoval" set.
            if (entities.find(entity) != entities.end()) // check if the entity is in this world
                this->markedForRemoval.insert(entity);   // add the entity to the markedForRemoval set to be deleted later
        }

        // This removes the elements in "markedForRemoval" from the "entities" set.
        // Then each of these elements are deleted.
        void deleteMarkedEntities()
        {
            //(Req 8) Remove and delete all the entities that have been marked for removal
//             for (auto it = markedForRemoval.begin(); it != markedForRemoval.end(); ++it) {
//                 delete *it;
//             }
            for (auto entity : markedForRemoval)
            {                           // loop over the markedForRemoval set
                entities.erase(entity); // remove the entity from the entities set
                delete entity;          // delete the entity
            }
            markedForRemoval.clear(); // clear the markedForRemoval set
        }

        // This deletes all entities in the world
        void clear()
        {
            //(Req 8) Delete all the entites and make sure that the containers are empty
            // solution 1

            // for (auto it = entities.begin(); it != entities.end(); ++it) {
            //     delete *it;
            // }
            // entities.clear();

            // solution 2

            for (auto it = entities.begin(); it != entities.end(); ++it)
            {                        // loop over the entities set
                markForRemoval(*it); // mark the entity for removal
            }
            deleteMarkedEntities(); // delete the marked entities
            entities.clear();       // clear the entities set
        }

        // Since the world owns all of its entities, they should be deleted alongside it.
        ~World()
        {
            clear();
        }

        // The world should not be copyable
        World(const World &) = delete;

        World &operator=(World const &) = delete;
    };

}