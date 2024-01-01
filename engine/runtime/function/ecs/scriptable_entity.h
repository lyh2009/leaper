#pragma once
#include "entity.h"
#include "systems/script_system/navite_script_system.h"

namespace Leaper
{
    class ScriptableEntity
    {
    public:
        template<typename T>
        T& GetComponent()
        {   
            return m_entity.GetComponent<T>();
        }   
    
    virtual void OnCreate()
    {
        
    }
    virtual void OnUpdate()
    {

    }    
    private:
        Leaper::Entity m_entity;
        friend class Scene;
        friend class NaviteScriptSystem;
    };
}