// Handling of all the Events

#pragma once

#include "KeyEvents.h"
#include "MouseEvents.h"
#include "WindowEvents.h"

namespace MiniRenderer
{
    class EventHandler
    {
    public:
        static EventHandler* s_Instance;
        static EventHandler* GetInstance() { if(!s_Instance) s_Instance = new EventHandler(); return s_Instance; }
        EventHandler() : WindowEventDispatcher(), MouseEventDispatcher(), KeyEventDispatcher() {}
        ~EventHandler() { delete s_Instance; }
        // Window Events
        EventDispatcher<WindowEvents> WindowEventDispatcher;
        // Mouse Events
        EventDispatcher<MouseEvents> MouseEventDispatcher;
        // Keyboard Events
        EventDispatcher<KeyEvents> KeyEventDispatcher;
    private:
    };
}