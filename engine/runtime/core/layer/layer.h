#pragma once
#include <string>
#include "core/events/events.h"

namespace Leaper
{
    class Layer
    {
    public:
        Layer(std::string debug_name)
            : m_debug_name(debug_name) {}

        inline std::string GetLayerName() { return m_debug_name; }

        virtual void OnAttach() {}
        virtual void OnUpdate() {}
        virtual void OnEvent(Leaper::Event& e) {}
        virtual void OnImGuiRender() {}
        virtual void OnDetach() {}
    private:
        std::string m_debug_name;
    };

} // namespace Leaper
