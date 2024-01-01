#pragma once

#include "function/render/context.h"
#include "core/events/events.h"
#include "core/base.h"

#include <string>
#include <stdint.h>

namespace Leaper
{
    class Window
    {
    public:
        using EventCallbackFunc = std::function<void(Event&)>;

        virtual uint32_t GetWidth() const = 0;
        virtual uint32_t GetHeight() const = 0;
        virtual std::string GetTitle() const = 0;

        virtual void SetEventCallback(const EventCallbackFunc& event) = 0;
        
        virtual void Init() = 0;

        virtual void OnUpdate() = 0;
        virtual void OnClose() = 0;

        virtual void* GetNativeWindow() const = 0;

        virtual void SetVSync(bool enabled) = 0;
        virtual bool IsVSync() const = 0;

        virtual bool IsClose() const = 0;

        static Leaper::Ref<Leaper::Window> Create(uint32_t width, uint32_t height, std::string title);
    };

} // namespace Leaper
