#include "entitymanager.h"

EntityManager* EntityManager::s_Instance = nullptr;

EntityManager::EntityManager()
{
    // set singleton,only one instanc is allowed
    assert(s_Instance == nullptr);
    s_Instance = this;
}
