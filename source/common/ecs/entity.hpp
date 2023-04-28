#pragma once

#include "component.hpp"
#include "transform.hpp"
#include <list>
#include <iterator>
#include <string>
#include <glm/glm.hpp>

namespace our
{

    class World; // A forward declaration of the World Class

    class Entity
    {
        World *world;                      // This defines what world own this entity
        std::list<Component *> components; // A list of components that are owned by this entity

        friend World;       // The world is a friend since it is the only class that is allowed to instantiate an entity
        Entity() = default; // The entity constructor is private since only the world is allowed to instantiate an entity
    public:
        std::string name; // The name of the entity. It could be useful to refer to an entity by its name
        Entity *parent;   // The parent of the entity. The transform of the entity is relative to its parent.
        // If parent is null, the entity is a root entity (has no parent).
        Transform localTransform; // The transform of this entity relative to its parent.

        World *getWorld() const { return world; } // Returns the world to which this entity belongs

        glm::mat4
        getLocalToWorldMatrix() const;            // Computes and returns the transformation from the entities local space to the world space
        void deserialize(const nlohmann::json &); // Deserializes the entity data and components from a json object

        // This template method create a component of type T,
        // adds it to the components map and returns a pointer to it
        template <typename T>
        T *addComponent()
        {
            static_assert(std::is_base_of<Component, T>::value, "T must inherit from Component");
            //(Req 8) Create an component of type T, set its "owner" to be this entity, then push it into the component's list
            // Don't forget to return a pointer to the new component

            T *comp = new T();          // create a new component of type T
            comp->owner = this;         // set the owner of the component to be this entity
            components.push_back(comp); // push the component into the components list
            return comp;                // return pointer of new component
        }

        // This template method searhes for a component of type T and returns a pointer to it
        // If no component of type T was found, it returns a nullptr
        template <typename T>
        T *getComponent()
        {
            //(Req 8) Go through the components list and find the first component that can be dynamically cast to "T*".
            // Return the component you found, or return null of nothing was found.

            // for (int i =0;i<components.size();i++)
            // {
            //     T* ptr = dynamic_cast<T*>(components[i]);
            //     if ( ptr )
            //     {
            //         return ptr;
            //     }
            // }
            for (auto it = components.begin(); it != components.end(); ++it)
            {                                    // loop over the components list
                T *ptr = dynamic_cast<T *>(*it); // check if the current component can be casted to type T
                if (ptr)
                {               // if it can be casted
                    return ptr; // return the pointer to the component
                }
            }
            return nullptr; // return null if nothing was found
        }

        // This template method dynami and returns a pointer to it
        // If no component of type T was found, it returns a nullptr
        template <typename T>
        T *getComponent(size_t index)
        {
            auto it = components.begin();
            std::advance(it, index);
            if (it != comp onents.end())
                return dynamic_cast<T *>(*it);
            return nullptr;
        }

        // This template method searhes for a component of type T and deletes it
        template <typename T>
        void deleteComponent()
        {
            //(Req 8) Go through the components list and find the first component that can be dynamically cast to "T*".
            // If found, delete the found component and remove it from the components list

            // for (int i =0;i<components.size();i++)
            // {
            //     T* ptr = dynamic_cast<T*>(components[i]);
            //     if ( ptr )
            //     {
            //         delete ptr;
            //         components.erase(i);
            //         break;
            //     }
            // }
            for (auto it = components.begin(); it != components.end(); ++it)
            { // loop over the components list
                if (dynamic_cast<T *>(*it))
                {                         // check if the current component can be casted to type T
                    delete *it;           // delete the component
                    components.erase(it); // remove the component from the components list
                    break;                // break the loop
                }
            }
        }

        // This template method searhes for a component of type T and deletes it
        void deleteComponent(size_t index)
        {
            auto it = components.begin();
            std::advance(it, index); // advance the iterator to the index
            if (it != components.end())
            {                         // if the iterator is not at the end of the list (found the component to be del)
                delete *it;           // delete the component
                components.erase(it); // remove the component from the components list
            }
        }

        // This template method searhes for the given component and deletes it
        template <typename T>
        void deleteComponent(T const *component)
        {
            //(Req 8) Go through the components list and find the given component "component".
            // If found, delete the found component and remove it from the components list
            for (auto it = components.begin(); it != components.end(); ++it)
            { // loop over the components list
                if (component == dynamic_cast<T *>(*it))
                {                         // check if the current component can be casted to type T
                    delete *it;           // delete the component
                    components.erase(it); // remove the component from the components list
                    // break;
                }
            }
        }

        // Since the entity owns its components, they should be deleted alongside the entity
        ~Entity()
        {
            //(Req 8) Delete all the components in "components".
            for (auto it = components.begin(); it != components.end(); ++it)
            {               // loop over the components list
                delete *it; // delete the component
            }
            components.clear(); // clear the components list
        }

        // Entities should not be copyable
        Entity(const Entity &) = delete;

        Entity &operator=(Entity const &) = delete;
    };

}