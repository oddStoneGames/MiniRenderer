#include "EventHandler.h"

namespace MiniRenderer
{
    std::unique_ptr<EventHandler> EventHandler::s_Instance = nullptr;

    EventHandler* EventHandler::GetInstance()
    {
        if (!s_Instance)
            s_Instance = std::unique_ptr<EventHandler>(new EventHandler());

        return s_Instance.get();
    }
}