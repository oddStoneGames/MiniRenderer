// Handling of all the Events

#pragma once

#include "KeyEvents.h"
#include "MouseEvents.h"
#include "WindowEvents.h"
#include <memory>

namespace MiniRenderer
{
    class EventHandler
    {
    public:
        EventHandler() : WindowEventDispatcher(), MouseEventDispatcher(), KeyEventDispatcher() {}
        static EventHandler* GetInstance();

        // Window Events
        EventDispatcher<WindowEvents> WindowEventDispatcher;
        // Mouse Events
        EventDispatcher<MouseEvents> MouseEventDispatcher;
        // Keyboard Events
        EventDispatcher<KeyEvents> KeyEventDispatcher;
    private:
        static std::unique_ptr<EventHandler> s_Instance;
    };
}