#ifndef _CHARACTER_CONTROL_A6_CHAR_
#define _CHARACTER_CONTROL_A6_CHAR_

#include "PrimeEngine/Events/Component.h"
#include "CharacterControl/Events/Events.h"

namespace CharacterControl{

    namespace Components {

        struct A6character : public PE::Components::Component
        {
            PE_DECLARE_CLASS(A6character);

            A6character(PE::GameContext &context, PE::MemoryArena arena, PE::Handle hMyself, Events::Event_CreateA6Character *pEvt);

            virtual void addDefaultComponents();
        };
    }; // namespace Components
}; // namespace CharacterControl
#endif

