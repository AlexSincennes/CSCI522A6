#ifndef _PE_A6_CHAR_MOVEMENT_SM_H_
#define _PE_A6_CHAR_MOVEMENT_SM_H_


#include "PrimeEngine/Events/Component.h"
#include "PrimeEngine/Scene/SceneNode.h"
#include "CharacterControl/Events/Events.h"
#include "PrimeEngine/Math/Vector3.h"

namespace CharacterControl{
    // events that can be sent to this state machine or sent by this state machine (like TARGET_REACHED)
    namespace Events
    {
        /*
        // sent by behavior state machine when a soldier has to go somewhere
        struct SoldierNPCMovementSM_Event_MOVE_TO : public PE::Events::Event {
            PE_DECLARE_CLASS(SoldierNPCMovementSM_Event_MOVE_TO);

            SoldierNPCMovementSM_Event_MOVE_TO(Vector3 targetPos = Vector3());

            Vector3 m_targetPosition;
        };

        struct SoldierNPCMovementSM_Event_STOP : public PE::Events::Event {
            PE_DECLARE_CLASS(SoldierNPCMovementSM_Event_STOP);

            SoldierNPCMovementSM_Event_STOP()
            {}
        };

        // sent by this state machine to its components. probably to behavior state machine
        struct SoldierNPCMovementSM_Event_TARGET_REACHED : public PE::Events::Event {
            PE_DECLARE_CLASS(SoldierNPCMovementSM_Event_TARGET_REACHED);

            SoldierNPCMovementSM_Event_TARGET_REACHED()
            {}
        };
        */
    };
    namespace Components {

        // movement state machine talks to associated animation state machine
        struct A6cMovementSM : public PE::Components::Component
        {
            PE_DECLARE_CLASS(A6cMovementSM);

            enum States
            {
                STANDING,
                RUNNING_TO_TARGET,
                WALKING_TO_TARGET,
				SHOOTING,
                SHOOTING_AIM_DOWN,
                SHOOTING_AIM_IDLE
            };


            A6cMovementSM(PE::GameContext &context, PE::MemoryArena arena, PE::Handle hMyself);

            //////////////////////////////////////////////////////////////////////////
            // utility
            //////////////////////////////////////////////////////////////////////////
            PE::Components::SceneNode *getParentsSceneNode();

            //////////////////////////////////////////////////////////////////////////
            // Component API and Event Handlers
            //////////////////////////////////////////////////////////////////////////
            //
            virtual void addDefaultComponents();
            //
            /*
            PE_DECLARE_IMPLEMENT_EVENT_HANDLER_WRAPPER(do_SoldierNPCMovementSM_Event_MOVE_TO)
                virtual void do_SoldierNPCMovementSM_Event_MOVE_TO(PE::Events::Event *pEvt);
            //
            PE_DECLARE_IMPLEMENT_EVENT_HANDLER_WRAPPER(do_SoldierNPCMovementSM_Event_STOP)
                virtual void do_SoldierNPCMovementSM_Event_STOP(PE::Events::Event *pEvt);
            //
            */
            PE_DECLARE_IMPLEMENT_EVENT_HANDLER_WRAPPER(do_UPDATE)
                virtual void do_UPDATE(PE::Events::Event *pEvt);
            
            //////////////////////////////////////////////////////////////////////////
            // Member Variables
            //////////////////////////////////////////////////////////////////////////
            PE::Handle m_hAnimationSM;
            //
            // State
            Vector3 m_targetPostion;
            States m_state;
        };

    };
};

// TANK CONTROLS CODE PORTED TO CHARACTER CONTROLS

namespace PE {
    namespace Events{
        struct EventQueueManager;
    }
}

namespace CharacterControl {
    namespace Components {

        struct A6cControls : public PE::Components::Component
        {
            PE_DECLARE_CLASS(A6cControls);
        public:

            A6cControls(PE::GameContext &context, PE::MemoryArena arena, PE::Handle hMyself)
                : PE::Components::Component(context, arena, hMyself)
            {
            }

            virtual ~A6cControls(){}
            // Component ------------------------------------------------------------

            PE_DECLARE_IMPLEMENT_EVENT_HANDLER_WRAPPER(do_UPDATE);
            virtual void do_UPDATE(PE::Events::Event *pEvt);

            virtual void addDefaultComponents();

            //Methods----------------
            void handleKeyboardDebugInputEvents(PE::Events::Event *pEvt);
            // implement later
            //void handleKeyboardDebugInputEvents(PE::Events::Event *pEvt);
            //void handleControllerDebugInputEvents(PE::Events::Event *pEvt);

            PE::Events::EventQueueManager *m_pQueueManager;

            PrimitiveTypes::Float32 m_frameTime;
        };

        struct A6cController : public PE::Components::Component
        {
            // component API
            PE_DECLARE_CLASS(A6cController);

            A6cController(PE::GameContext &context, PE::MemoryArena arena,
                PE::Handle myHandle, float speed,
                Vector3 spawnPos, float networkPingInterval, PE::Handle hmovSM); // constructor

            virtual void addDefaultComponents(); // adds default children and event handlers


            PE_DECLARE_IMPLEMENT_EVENT_HANDLER_WRAPPER(do_UPDATE);
            virtual void do_UPDATE(PE::Events::Event *pEvt);

            PE_DECLARE_IMPLEMENT_EVENT_HANDLER_WRAPPER(do_A6C_Throttle);
            virtual void do_A6C_Throttle(PE::Events::Event *pEvt);

            PE_DECLARE_IMPLEMENT_EVENT_HANDLER_WRAPPER(do_A6C_Turn);
            virtual void do_A6C_Turn(PE::Events::Event *pEvt);

            PE_DECLARE_IMPLEMENT_EVENT_HANDLER_WRAPPER(do_A6C_Stop);
            virtual void do_A6C_Stop(PE::Events::Event *pEvt);

			PE_DECLARE_IMPLEMENT_EVENT_HANDLER_WRAPPER(do_A6C_Shoot);
			virtual void do_A6C_Shoot(PE::Events::Event *pEvt);

            PE_DECLARE_IMPLEMENT_EVENT_HANDLER_WRAPPER(do_A6C_Shoot_AimDown);
            virtual void do_A6C_Shoot_AimDown(PE::Events::Event *pEvt);

            PE_DECLARE_IMPLEMENT_EVENT_HANDLER_WRAPPER(do_A6C_Shoot_AimIdle);
            virtual void do_A6C_Shoot_AimIdle(PE::Events::Event *pEvt);

            void overrideTransform(Matrix4x4 &t);
            void activate();

            float m_timeSpeed;
            float m_time;
            float m_networkPingTimer;
            float m_networkPingInterval;
            Vector2 m_center;
            PrimitiveTypes::UInt32 m_counter;
            Vector3 m_spawnPos;
            bool m_active;
            bool m_overriden;
            Matrix4x4 m_transformOverride;
            PE::Handle hmovementSM;
        };
    }; // namespace Components
}; // namespace CharacterControl

#endif

