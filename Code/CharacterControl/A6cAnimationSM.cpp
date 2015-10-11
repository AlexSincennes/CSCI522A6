#include "PrimeEngine/APIAbstraction/APIAbstractionDefines.h"

#include "PrimeEngine/Lua/LuaEnvironment.h"

#include "A6cAnimationSM.h"
#include "A6character.h"

using namespace PE::Components;
using namespace PE::Events;

namespace CharacterControl{

    namespace Events{
        PE_IMPLEMENT_CLASS1(A6cAnimSM_Event_STOP, Event);

        PE_IMPLEMENT_CLASS1(A6cAnimSM_Event_WALK, Event);

        PE_IMPLEMENT_CLASS1(A6cAnimSM_Event_RUN, Event);

		PE_IMPLEMENT_CLASS1(A6cAnimSM_Event_Shoot, Event);
    }
    namespace Components{

        PE_IMPLEMENT_CLASS1(A6cAnimationSM, DefaultAnimationSM);

        A6cAnimationSM::A6cAnimationSM(PE::GameContext &context, PE::MemoryArena arena, PE::Handle hMyself) : DefaultAnimationSM(context, arena, hMyself)
        {
            m_curId = NONE;
        }

        void A6cAnimationSM::addDefaultComponents()
        {
            DefaultAnimationSM::addDefaultComponents();

            PE_REGISTER_EVENT_HANDLER(Events::A6cAnimSM_Event_STOP, A6cAnimationSM::do_A6cAnimSM_Event_STOP);
            PE_REGISTER_EVENT_HANDLER(Events::A6cAnimSM_Event_WALK, A6cAnimationSM::do_A6cAnimSM_Event_WALK);
            PE_REGISTER_EVENT_HANDLER(Events::A6cAnimSM_Event_RUN, A6cAnimationSM::do_A6cAnimSM_Event_RUN);
			PE_REGISTER_EVENT_HANDLER(Events::A6cAnimSM_Event_Shoot, A6cAnimationSM::do_A6cAnimSM_Event_Shoot);
        }

        void A6cAnimationSM::do_A6cAnimSM_Event_STOP(PE::Events::Event *pEvt)
        {

            if (m_curId != A6cAnimationSM::STAND)
            {
                m_curId = A6cAnimationSM::STAND;

                setAnimation(0, A6cAnimationSM::STAND,
                    0, 0, 1, 1,
                    PE::LOOPING);
            }
        }

        void A6cAnimationSM::do_A6cAnimSM_Event_WALK(PE::Events::Event *pEvt)
        {
            if (m_curId != A6cAnimationSM::WALK)
            {
                m_curId = A6cAnimationSM::WALK;
                setAnimation(0, A6cAnimationSM::WALK,
                    0, 0, 1, 1,
                    PE::LOOPING);
            }
        }

        void A6cAnimationSM::do_A6cAnimSM_Event_RUN(PE::Events::Event *pEvt)
        {
            if (m_curId != A6cAnimationSM::RUN)
            {
                m_curId = A6cAnimationSM::RUN;
                setAnimation(0, A6cAnimationSM::RUN,
                    0, 0, 1, 1,
                    PE::LOOPING | PE::PARTIAL_BODY_ANIMATION);
            }
        }

		 void A6cAnimationSM::do_A6cAnimSM_Event_Shoot(PE::Events::Event *pEvt)
        {
            if (m_curId != A6cAnimationSM::SHOOT)
            {
                m_curId = A6cAnimationSM::SHOOT;
                setAnimation(0, A6cAnimationSM::SHOOT,
                    0, 0, 1, 1,
                    PE::LOOPING | PE::PARTIAL_BODY_ANIMATION);
            }
        }

    }
}




