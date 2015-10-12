#ifndef _PE_A6_CHAR_ANIMATION_SM_H_
#define _PE_A6_CHAR_ANIMATION_SM_H_


#include "PrimeEngine/Events/Component.h"
#include "PrimeEngine/Scene/DefaultAnimationSM.h"


#include "CharacterControl/Events/Events.h"

namespace CharacterControl{

    // events that can be sent to this state machine
    namespace Events
    {
        // sent by movement state machine when a soldier has to stop
        struct A6cAnimSM_Event_STOP : public PE::Events::Event {
            PE_DECLARE_CLASS(A6cAnimSM_Event_STOP);

            A6cAnimSM_Event_STOP() {}
        };

        // sent by movement state machine when a soldier has to walk
        struct A6cAnimSM_Event_WALK : public PE::Events::Event {
            PE_DECLARE_CLASS(A6cAnimSM_Event_WALK);

            A6cAnimSM_Event_WALK() {}
        };

        // sent by movement state machine when a soldier has to run
        struct A6cAnimSM_Event_RUN : public PE::Events::Event {
            PE_DECLARE_CLASS(A6cAnimSM_Event_RUN);

            A6cAnimSM_Event_RUN() {}
        };

        // todo add events for shooting (sent by weapons state machine

		struct A6cAnimSM_Event_Shoot : public PE::Events::Event {
            PE_DECLARE_CLASS(A6cAnimSM_Event_Shoot);

            A6cAnimSM_Event_Shoot() {}
        };

        struct A6cAnimSM_Event_Shoot_AimIdle : public PE::Events::Event {
            PE_DECLARE_CLASS(A6cAnimSM_Event_Shoot_AimIdle);

            A6cAnimSM_Event_Shoot_AimIdle() {}
        };

        struct A6cAnimSM_Event_Shoot_AimDown : public PE::Events::Event {
            PE_DECLARE_CLASS(A6cAnimSM_Event_Shoot_AimDown);

            A6cAnimSM_Event_Shoot_AimDown() {}
        };

    };

    namespace Components {

        struct A6cAnimationSM : public PE::Components::DefaultAnimationSM
        {
            PE_DECLARE_CLASS(A6cAnimationSM);

            enum AnimId
            {
                /*
                NONE = -1,
                STAND = 20,
                WALK = 5,
                RUN = 18,
                STAND_AIM = 1,
                STAND_SHOOT = 16,
                */
               /* NONE = -1,
                STAND = 7,
                WALK = 6,
                RUN = 0,
                SHOOT = 1,
                AIM_IDLE = 2,
                AIM_DOWN = 3,
                HOLDING_RIFLE = 3,
                STAND_POSE = 5*/

				NONE = -1,
				RUN =0,
				SHOOT = 1,
				AIM_DOWN = 2,
				AIM_IDLE = 3,
				WALK = 4,
				STAND = 5


            };

            A6cAnimationSM(PE::GameContext &context, PE::MemoryArena arena, PE::Handle hMyself);

            // event handling
            virtual void addDefaultComponents();
            PE_DECLARE_IMPLEMENT_EVENT_HANDLER_WRAPPER(do_A6cAnimSM_Event_STOP)
                virtual void do_A6cAnimSM_Event_STOP(PE::Events::Event *pEvt);
            PE_DECLARE_IMPLEMENT_EVENT_HANDLER_WRAPPER(do_A6cAnimSM_Event_WALK)
                virtual void do_A6cAnimSM_Event_WALK(PE::Events::Event *pEvt);
            PE_DECLARE_IMPLEMENT_EVENT_HANDLER_WRAPPER(do_A6cAnimSM_Event_RUN)
                virtual void do_A6cAnimSM_Event_RUN(PE::Events::Event *pEvt);
			PE_DECLARE_IMPLEMENT_EVENT_HANDLER_WRAPPER(do_A6cAnimSM_Event_Shoot)
				virtual void do_A6cAnimSM_Event_Shoot(PE::Events::Event *pEvt);
            PE_DECLARE_IMPLEMENT_EVENT_HANDLER_WRAPPER(do_A6cAnimSM_Event_Shoot_AimDown)
                virtual void do_A6cAnimSM_Event_Shoot_AimDown(PE::Events::Event *pEvt);
            PE_DECLARE_IMPLEMENT_EVENT_HANDLER_WRAPPER(do_A6cAnimSM_Event_Shoot_AimIdle)
                virtual void do_A6cAnimSM_Event_Shoot_AimIdle(PE::Events::Event *pEvt);


            AnimId m_curId;

            PE::Handle hSkeletonInstance;
        };

    };
};


#endif


