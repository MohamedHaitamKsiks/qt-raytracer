#ifndef ENTITYMANAGER_H
#define ENTITYMANAGER_H

#include "entity3d.h"

#include <QHash>
#include <functional>

struct EntityInfo
{
    std::function<Entity3D*(Entity3D*)> create;
};

class EntityManager
{
public:
    EntityManager();

    // register entity
    template<typename EntityType>
    void registerEntity(const QString& entityName)
    {
        // check if entity type is Entity3D
        static_assert(std::is_base_of_v<Entity3D, EntityType>);

        // create info
        EntityInfo info{};
        info.create = [this] (Entity3D* parent)
        {
            return new EntityType(parent);
        };

        // register info
        this->registeredEntities[entityName] = info;
    }

    // get singleton isntance;
    static inline EntityManager* instance()
    {
        return s_Instance;
    }

    // create entity
    inline Entity3D* create(const QString& entityName, Entity3D* parent)
    {
        return this->registeredEntities[entityName].create(parent);
    }

    // get list of registered entities
    inline QList<QString> getRegisteredEntities()
    {
        return this->registeredEntities.keys();
    }


private:
    static EntityManager* s_Instance;
    QHash<QString, EntityInfo> registeredEntities{};
};

#endif // ENTITYMANAGER_H
