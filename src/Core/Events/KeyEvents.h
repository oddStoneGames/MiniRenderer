#pragma once
#include "Event.h"

namespace MiniRenderer
{
    enum class KeyEvents
    {
        KeyDown,
        KeyUp
    };

    class KeyDownEvent : public Event<KeyEvents>
    {
    public:
        KeyDownEvent() : Event<KeyEvents>(KeyEvents::KeyDown, "KeyDownEvent"){}
        virtual ~KeyDownEvent() = default;
        int keycode = -1;    // The ASCII of the key that was Pressed Down!
    };

    class KeyUpEvent : public Event<KeyEvents>
    {
    public:
        KeyUpEvent() : Event<KeyEvents>(KeyEvents::KeyUp, "KeyUpEvent"){}
        virtual ~KeyUpEvent() = default;
        int keycode = -1;    // The ASCII of the key that was Lifted Up!
    };
}