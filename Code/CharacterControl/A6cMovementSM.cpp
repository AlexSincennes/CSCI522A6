#include "PrimeEngine/APIAbstraction/APIAbstractionDefines.h"

// Inter-Engine includes
#include "PrimeEngine/Lua/LuaEnvironment.h"
#include "PrimeEngine/Events/StandardEvents.h"
#include "PrimeEngine/Scene/Mesh.h"
#include "PrimeEngine/Scene/SceneNode.h"
#include "PrimeEngine/Networking/EventManager.h"
#include "PrimeEngine/Networking/Client/ClientNetworkManager.h"
#include "CharacterControl/Events/Events.h"
#include "PrimeEngine/GameObjectModel/GameObjectManager.h"
#include "PrimeEngine/Events/StandardKeyboardEvents.h"
#include "PrimeEngine/Events/StandardIOSEvents.h"
#include "PrimeEngine/Events/StandardGameEvents.h"
#include "PrimeEngine/Events/EventQueueManager.h"
#include "PrimeEngine/Events/StandardControllerEvents.h"
#include "PrimeEngine/GameObjectModel/DefaultGameControls/DefaultGameControls.h"
#include "CharacterControl/CharacterControlContext.h"

#include "CharacterControl/Client/ClientSpaceShipControls.h"
#include "CharacterControl/Tank/ClientTank.h"

#include "A6cMovementSM.h"
#include "A6cAnimationSM.h"
#include "A6character.h"
using namespace PE::Components;
using namespace PE::Events;
using namespace CharacterControl::Events;

// SOLDIER NPC CONTROLS CODE PORTED TO CHARACTER CONTROLS

namespace CharacterControl{

    // Events sent by behavior state machine (or other high level state machines)
    // these are events that specify where a soldier should move
    namespace Events{

    }

    namespace Components{

        PE_IMPLEMENT_CLASS1(A6cMovementSM, Component);


        A6cMovementSM::A6cMovementSM(PE::GameContext &context, PE::MemoryArena arena, PE::Handle hMyself)
            : Component(context, arena, hMyself)
            , m_state(STANDING)
        {}

        SceneNode *A6cMovementSM::getParentsSceneNode()
        {
            PE::Handle hParent = getFirstParentByType<Component>();
            if (hParent.isValid())
            {
                // see if parent has scene node component
                return hParent.getObject<Component>()->getFirstComponent<SceneNode>();

            }
            return NULL;
        }

        void A6cMovementSM::addDefaultComponents()
        {
            Component::addDefaultComponents();

            //PE_REGISTER_EVENT_HANDLER(SoldierNPCMovementSM_Event_MOVE_TO, SoldierNPCMovementSM::do_SoldierNPCMovementSM_Event_MOVE_TO);
            //PE_REGISTER_EVENT_HANDLER(SoldierNPCMovementSM_Event_STOP, SoldierNPCMovementSM::do_SoldierNPCMovementSM_Event_STOP);

            PE_REGISTER_EVENT_HANDLER(Event_UPDATE, A6cMovementSM::do_UPDATE);
        }

        void A6cMovementSM::do_UPDATE(PE::Events::Event *pEvt)
        {
            if (m_state == STANDING)
            {
                // no one has modified our state based on TARGET_REACHED callback
                // this means we are not going anywhere right now
                // so can send event to animation state machine to stop
                {
                    Events::A6cAnimSM_Event_STOP evt;

                    A6character *pA6C = getFirstParentByTypePtr<A6character>();
                    pA6C->getFirstComponent<PE::Components::SceneNode>()->handleEvent(&evt);
                }
            }
            if (m_state == RUNNING_TO_TARGET)
            {
                // no one has modified our state based on TARGET_REACHED callback
                // this means we are not going anywhere right now
                // so can send event to animation state machine to stop
                {
                    Events::A6cAnimSM_Event_RUN evt;

                    A6character *pA6C = getFirstParentByTypePtr<A6character>();
                    pA6C->getFirstComponent<PE::Components::SceneNode>()->handleEvent(&evt);
                }
            }
            if (m_state == WALKING_TO_TARGET)
            {
                // no one has modified our state based on TARGET_REACHED callback
                // this means we are not going anywhere right now
                // so can send event to animation state machine to stop
                {
                    Events::A6cAnimSM_Event_WALK evt;

                    A6character *pA6C = getFirstParentByTypePtr<A6character>();
                    pA6C->getFirstComponent<PE::Components::SceneNode>()->handleEvent(&evt);
                }
            }

			if (m_state == SHOOTING)
			{				
				{
					Events::A6cAnimSM_Event_Shoot evt;

					A6character *pA6C = getFirstParentByTypePtr<A6character>();
					pA6C->getFirstComponent<PE::Components::SceneNode>()->handleEvent(&evt);
				}
			}

            if (m_state == SHOOTING_AIM_DOWN) {
                    {
                        Events::A6cAnimSM_Event_Shoot_AimDown evt;

                        A6character *pA6C = getFirstParentByTypePtr<A6character>();
                        pA6C->getFirstComponent<PE::Components::SceneNode>()->handleEvent(&evt);
                    }
            }

            if (m_state == SHOOTING_AIM_IDLE) {
                    {
                        Events::A6cAnimSM_Event_Shoot_AimIdle evt;

                        A6character *pA6C = getFirstParentByTypePtr<A6character>();
                        pA6C->getFirstComponent<PE::Components::SceneNode>()->handleEvent(&evt);
                    }
            }
        }
    }
}

// TANK CONTROLS CODE PORTED TO CHARACTER CONTROLS

using namespace PE::Components;
using namespace PE::Events;
using namespace CharacterControl::Events;

// Arkane Control Values
#define Analog_To_Digital_Trigger_Distance 0.5f
static float Debug_Fly_Speed = 8.0f; //Units per second
#define Debug_Rotate_Speed 2.0f //Radians per second
#define Player_Keyboard_Rotate_Speed 20.0f //Radians per second

namespace CharacterControl {
    namespace Components {




        PE_IMPLEMENT_CLASS1(A6cControls, PE::Components::Component);

        void A6cControls::addDefaultComponents()
        {
            Component::addDefaultComponents();

            PE_REGISTER_EVENT_HANDLER(Event_UPDATE, A6cControls::do_UPDATE);
        }

        void A6cControls::do_UPDATE(PE::Events::Event *pEvt)
        {
            // Process input events (XBOX360 buttons, triggers...)
            PE::Handle iqh = PE::Events::EventQueueManager::Instance()->getEventQueueHandle("input");

            //if no input: stop event
            if (iqh.getObject<PE::Events::EventQueue>()->empty()) {
                PE::Handle h("EVENT", sizeof(Events::Event_A6C_Stop));
                Events::Event_A6C_Stop *stopEvt = new(h)Events::Event_A6C_Stop;
                m_pQueueManager = PE::Events::EventQueueManager::Instance();
                m_pQueueManager->add(h, QT_GENERAL);
            }

            // Process input event -> game event conversion
            while (!iqh.getObject<PE::Events::EventQueue>()->empty())
            {
                PE::Events::Event *pInputEvt = iqh.getObject<PE::Events::EventQueue>()->getFront();
                m_frameTime = ((Event_UPDATE*)(pEvt))->m_frameTime;
                // Have DefaultGameControls translate the input event to GameEvents
                handleKeyboardDebugInputEvents(pInputEvt);
                //handleControllerDebugInputEvents(pInputEvt);
                //handleIOSDebugInputEvents(pInputEvt);

                iqh.getObject<PE::Events::EventQueue>()->destroyFront();
            }

            // Events are destoryed by destroyFront() but this is called every frame just in case
            iqh.getObject<PE::Events::EventQueue>()->destroy();
        }

        void A6cControls::handleKeyboardDebugInputEvents(Event *pEvt)
        {
            m_pQueueManager = PE::Events::EventQueueManager::Instance();
            /*
            if (PE::Events::Event_KEY_A_HELD::GetClassId() == pEvt->getClassId())
            {
            PE::Handle h("EVENT", sizeof(Event_FLY_CAMERA));
            PE::Events::Event_FLY_CAMERA *flyCameraEvt = new(h) PE::Events::Event_FLY_CAMERA ;

            Vector3 relativeMovement(-1.0f,0.0f,0.0f);
            flyCameraEvt->m_relativeMove = relativeMovement * Debug_Fly_Speed * m_frameTime;
            m_pQueueManager->add(h, QT_GENERAL);
            }
            else
            */


            if (Event_KEY_W_HELD::GetClassId() != pEvt->getClassId() && Event_KEY_SHIFT_HELD::GetClassId() != pEvt->getClassId()) {
                PE::Handle h("EVENT", sizeof(Events::Event_A6C_Stop));
                Events::Event_A6C_Stop *stopEvt = new(h)Events::Event_A6C_Stop;

                m_pQueueManager->add(h, QT_GENERAL);
            }

            if (Event_KEY_SPACE_HELD::GetClassId() == pEvt->getClassId()) {
                PE::Handle h("EVENT", sizeof(Events::Event_A6C_Shoot));
                Events::Event_A6C_Shoot *stopEvt = new(h)Events::Event_A6C_Shoot;

                m_pQueueManager->add(h, QT_GENERAL);
            }
            else if (Event_KEY_A_HELD::GetClassId() == pEvt->getClassId()) {
                PE::Handle h("EVENT", sizeof(Events::Event_A6C_Shoot_AimIdle));
                Events::Event_A6C_Shoot_AimIdle *stopEvt = new(h)Events::Event_A6C_Shoot_AimIdle;

                m_pQueueManager->add(h, QT_GENERAL);
            }

            else if (Event_KEY_D_HELD::GetClassId() == pEvt->getClassId()) {
                PE::Handle h("EVENT", sizeof(Events::Event_A6C_Shoot_AimDown));
                Events::Event_A6C_Shoot_AimDown *stopEvt = new(h)Events::Event_A6C_Shoot_AimDown;

                m_pQueueManager->add(h, QT_GENERAL);
            }

            /*
            if (Event_KEY_S_HELD::GetClassId() == pEvt->getClassId())
            {
                PE::Handle h("EVENT", sizeof(Events::Event_A6C_Throttle));
                Events::Event_A6C_Throttle *flyCameraEvt = new(h)Events::Event_A6C_Throttle;

                Vector3 relativeMovement(0.0f, 0.0f, -1.0f);
                flyCameraEvt->m_relativeMove = relativeMovement * Debug_Fly_Speed * m_frameTime;
                m_pQueueManager->add(h, QT_GENERAL);
            }
            */
            /*
            else if (Event_KEY_D_HELD::GetClassId() == pEvt->getClassId())
            {
            PE::Handle h("EVENT", sizeof(Event_FLY_CAMERA));
            Event_FLY_CAMERA *flyCameraEvt = new(h) Event_FLY_CAMERA ;

            Vector3 relativeMovement(1.0f,0.0f,0.0f);
            flyCameraEvt->m_relativeMove = relativeMovement * Debug_Fly_Speed * m_frameTime;
            m_pQueueManager->add(h, QT_GENERAL);
            }*/
            else if(Event_KEY_LEFT_HELD::GetClassId() == pEvt->getClassId())
            {
                PE::Handle h("EVENT", sizeof(Event_A6C_Turn));
                Event_A6C_Turn *rotateCameraEvt = new(h)Event_A6C_Turn;

                Vector3 relativeRotate(-1.0f, 0.0f, 0.0f);
                rotateCameraEvt->m_relativeRotate = relativeRotate * Debug_Rotate_Speed * m_frameTime;
                m_pQueueManager->add(h, QT_GENERAL);
            }
            else if(Event_KEY_RIGHT_HELD::GetClassId() == pEvt->getClassId())
            {
                PE::Handle h("EVENT", sizeof(Event_A6C_Turn));
                Event_A6C_Turn *rotateCameraEvt = new(h)Event_A6C_Turn;

                Vector3 relativeRotate(1.0f, 0.0f, 0.0f);
                rotateCameraEvt->m_relativeRotate = relativeRotate * Debug_Rotate_Speed * m_frameTime;
                m_pQueueManager->add(h, QT_GENERAL);
            }

            else if(Event_KEY_SHIFT_HELD::GetClassId() == pEvt->getClassId()) {
                PE::Handle h("EVENT", sizeof(Events::Event_A6C_Throttle));
                Events::Event_A6C_Throttle *flyCameraEvt = new(h)Events::Event_A6C_Throttle;

                Vector3 relativeMovement(0.0f, 0.0f, 1.0f);
                flyCameraEvt->m_relativeMove = relativeMovement * Debug_Fly_Speed * m_frameTime;
                m_pQueueManager->add(h, QT_GENERAL);
            }
            else if (Event_KEY_W_HELD::GetClassId() == pEvt->getClassId()) {
                PE::Handle h("EVENT", sizeof(Events::Event_A6C_Throttle));
                Events::Event_A6C_Throttle *flyCameraEvt = new(h)Events::Event_A6C_Throttle;

                Vector3 relativeMovement(0.0f, 0.0f, 0.5f);
                flyCameraEvt->m_relativeMove = relativeMovement * Debug_Fly_Speed * m_frameTime;
                m_pQueueManager->add(h, QT_GENERAL);
            }

            /*
            else if (Event_KEY_DOWN_HELD::GetClassId() == pEvt->getClassId())
            {
            PE::Handle h("EVENT", sizeof(Event_ROTATE_CAMERA));
            Event_ROTATE_CAMERA *rotateCameraEvt = new(h) Event_ROTATE_CAMERA ;

            Vector3 relativeRotate(0.0f,-1.0f,0.0f);
            rotateCameraEvt->m_relativeRotate = relativeRotate * Debug_Rotate_Speed * m_frameTime;
            m_pQueueManager->add(h, QT_GENERAL);
            }
            else if (Event_KEY_UP_HELD::GetClassId() == pEvt->getClassId())
            {
            PE::Handle h("EVENT", sizeof(Event_ROTATE_CAMERA));
            Event_ROTATE_CAMERA *rotateCameraEvt = new(h) Event_ROTATE_CAMERA ;

            Vector3 relativeRotate(0.0f,1.0f,0.0f);
            rotateCameraEvt->m_relativeRotate = relativeRotate * Debug_Rotate_Speed * m_frameTime;
            m_pQueueManager->add(h, QT_GENERAL);
            }
            */
            else
            {
                Component::handleEvent(pEvt);
            }
        }

       
        PE_IMPLEMENT_CLASS1(A6cController, Component);

        A6cController::A6cController(PE::GameContext &context, PE::MemoryArena arena,
            PE::Handle myHandle, float speed, Vector3 spawnPos,
            float networkPingInterval, PE::Handle hmovSM)
            : Component(context, arena, myHandle)
            , m_timeSpeed(speed)
            , m_time(0)
            , m_counter(0)
            , m_active(0)
            , m_networkPingTimer(0)
            , m_networkPingInterval(networkPingInterval)
            , m_overriden(false)
        {
            m_spawnPos = spawnPos;
            hmovementSM = hmovSM;
        }

        void A6cController::addDefaultComponents()
        {
            Component::addDefaultComponents();

            PE_REGISTER_EVENT_HANDLER(PE::Events::Event_UPDATE, A6cController::do_UPDATE);

            // note: these event handlers will be registered only when one tank is activated as client tank (i.e. driven by client input on this machine)
            //PE_REGISTER_EVENT_HANDLER(Event_A6C_Throttle, A6cController::do_A6C_Throttle);
            //PE_REGISTER_EVENT_HANDLER(Event_A6C_Turn, A6cController::do_A6C_Turn);

        }

        void A6cController::do_A6C_Throttle(PE::Events::Event *pEvt)
        {
            Event_A6C_Throttle *pRealEvent = (Event_A6C_Throttle *)(pEvt);

            A6cMovementSM* pMovSM = hmovementSM.getObject<A6cMovementSM>();

            if (pRealEvent->m_relativeMove.length() > 0.1f)
                    pMovSM->m_state = A6cMovementSM::RUNNING_TO_TARGET;
            else
                if (pMovSM->m_state == A6cMovementSM::RUNNING_TO_TARGET)
                    return;
                else
                    pMovSM->m_state = A6cMovementSM::WALKING_TO_TARGET;

            PE::Handle hFisrtSN = getFirstComponentHandle<SceneNode>();
            if (!hFisrtSN.isValid())
            {
                assert(!"wrong setup. must have scene node referenced");
                return;
            }

            SceneNode *pFirstSN = hFisrtSN.getObject<SceneNode>();

            pFirstSN->m_base.moveForward(pRealEvent->m_relativeMove.getZ());
            pFirstSN->m_base.moveRight(pRealEvent->m_relativeMove.getX());
            pFirstSN->m_base.moveUp(pRealEvent->m_relativeMove.getY());
        }

        void A6cController::do_A6C_Turn(PE::Events::Event *pEvt)
        {
            Event_A6C_Turn *pRealEvent = (Event_A6C_Turn *)(pEvt);

            PE::Handle hFisrtSN = getFirstComponentHandle<SceneNode>();
            if (!hFisrtSN.isValid())
            {
                assert(!"wrong setup. must have scene node referenced");
                return;
            }

            SceneNode *pFirstSN = hFisrtSN.getObject<SceneNode>();

            //pcam->m_base.turnUp(pRealEvent->m_relativeRotate.getY());
            pFirstSN->m_base.turnLeft(-pRealEvent->m_relativeRotate.getX());

        }

        void A6cController::do_A6C_Stop(PE::Events::Event *pEvt) {
            A6cMovementSM* pMovSM = hmovementSM.getObject<A6cMovementSM>();
            pMovSM->m_state = A6cMovementSM::STANDING;
        }


        void A6cController::do_A6C_Shoot(PE::Events::Event *pEvt)
        {
            A6cMovementSM* pMovSM = hmovementSM.getObject<A6cMovementSM>();
            pMovSM->m_state = A6cMovementSM::SHOOTING;
			RootSceneNode::Instance()->BulletCount++;

        }

        void A6cController::do_A6C_Shoot_AimDown(PE::Events::Event *pEvt) {
            A6cMovementSM* pMovSM = hmovementSM.getObject<A6cMovementSM>();
            pMovSM->m_state = A6cMovementSM::SHOOTING_AIM_DOWN;

        }

        void A6cController::do_A6C_Shoot_AimIdle(PE::Events::Event *pEvt) {
            A6cMovementSM* pMovSM = hmovementSM.getObject<A6cMovementSM>();
            pMovSM->m_state = A6cMovementSM::SHOOTING_AIM_IDLE;

        }

        void A6cController::do_UPDATE(PE::Events::Event *pEvt)
        {
            PE::Events::Event_UPDATE *pRealEvt = (PE::Events::Event_UPDATE *)(pEvt);

            if (m_active)
            {
                m_time += pRealEvt->m_frameTime;
                m_networkPingTimer += pRealEvt->m_frameTime;
            }

            PE::Handle hFisrtSN = getFirstComponentHandle<SceneNode>();
            if (!hFisrtSN.isValid())
            {
                assert(!"wrong setup. must have scene node referenced");
                return;
            }

            SceneNode *pFirstSN = hFisrtSN.getObject<SceneNode>();

             Vector3 pos = RootSceneNode::Instance()->MichaelCam;
			 float x = pos.m_x;
             float y = pos.m_y + 1.0f;
            float z = pos.m_z-2.0f;

            // note we could have stored the camera reference in this object instead of searching for camera scene node
            if (CameraSceneNode *pCamSN = pFirstSN->getFirstComponent<CameraSceneNode>())
            {
                pCamSN->m_base.setPos(Vector3(x,y,z));
            }

			A6cMovementSM* pMovSM = hmovementSM.getObject<A6cMovementSM>();
			if( pMovSM->m_state == A6cMovementSM::SHOOTING)
			{
				Vector3 fpos = RootSceneNode::Instance()->MichaelFrontCam;
				 float xf = fpos.m_x+0.25f;
				float yf = fpos.m_y + 0.25f;
				float zf = fpos.m_z+0.37f;
				 if (CameraSceneNode *pCamSN = pFirstSN->getFirstComponent<CameraSceneNode>())
				{
					pCamSN->m_base.setPos(Vector3(xf,yf,zf));
				}
			}
			
            if (!m_overriden)
            {
                /*
                if (m_time > 2.0f*PrimitiveTypes::Constants::c_Pi_F32)
                {
                m_time = 0;
                if (m_counter)
                {
                m_counter = 0;
                m_center = Vector2(0,0);
                }
                else
                {
                m_counter = 1;
                m_center = Vector2(10.0f, 0);
                }
                }

                Vector3 pos = Vector3(m_center.m_x, 0, m_center.m_y);
                pos.m_x += (float)cos(m_time) * 5.0f * (m_counter ? -1.0f : 1.0f);
                pos.m_z += (float)sin(m_time) * 5.0f;
                pos.m_y = 0;

                Vector3 fwrd;
                fwrd.m_x = -(float)sin(m_time)  * (m_counter ? -1.0f : 1.0f);
                fwrd.m_z = (float)cos(m_time);
                fwrd.m_y = 0;

                Vector3 right;
                right.m_x = (float)cos(m_time) * (m_counter ? -1.0f : 1.0f) * (m_counter ? -1.0f : 1.0f);
                right.m_z = (float)sin(m_time) * (m_counter ? -1.0f : 1.0f);
                right.m_y = 0;


                pFirstSN->m_base.setPos(m_spawnPos + pos);
                pFirstSN->m_base.setN(fwrd);
                pFirstSN->m_base.setU(right);
                */
            }
            else
            {
                pFirstSN->m_base = m_transformOverride;
            }
        }

        void A6cController::overrideTransform(Matrix4x4 &t)
        {
            m_overriden = true;
            m_transformOverride = t;
        }

        void A6cController::activate()
        {
            m_active = true;

            // this function is called on client tank. since this is client tank and we have client authoritative movement
            // we need to register event handling for movement here.
            // We have 6 tanks total. we activate tank controls controller (in GOM Addon) that will process input events into tank movement events
            // but we want only one tank to process those events. One way to do it is to dynamically add event handlers
            // to only one tank controller. this is what we do here.
            // another way to do this would be to only hass one tank controller, and have it grab one of tank scene nodes when activated
            PE_REGISTER_EVENT_HANDLER(Event_A6C_Throttle, A6cController::do_A6C_Throttle);
            PE_REGISTER_EVENT_HANDLER(Event_A6C_Turn, A6cController::do_A6C_Turn);
			PE_REGISTER_EVENT_HANDLER(Event_A6C_Shoot, A6cController::do_A6C_Shoot);
            PE_REGISTER_EVENT_HANDLER(Event_A6C_Shoot_AimDown, A6cController::do_A6C_Shoot_AimDown);
            PE_REGISTER_EVENT_HANDLER(Event_A6C_Shoot_AimIdle, A6cController::do_A6C_Shoot_AimIdle);
            PE_REGISTER_EVENT_HANDLER(Event_A6C_Stop, A6cController::do_A6C_Stop);
			

            PE::Handle hFisrtSN = getFirstComponentHandle<SceneNode>();
            if (!hFisrtSN.isValid())
            {
                assert(!"wrong setup. must have scene node referenced");
                return;
            }

            //create camera
            PE::Handle hCamera("Camera", sizeof(Camera));
            Camera *pCamera = new(hCamera)Camera(*m_pContext, m_arena, hCamera, hFisrtSN);
            pCamera->addDefaultComponents();
            CameraManager::Instance()->setCamera(CameraManager::PLAYER, hCamera);

            CameraManager::Instance()->selectActiveCamera(CameraManager::PLAYER);

            //disable default camera controls

            m_pContext->getDefaultGameControls()->setEnabled(false);
            m_pContext->get<CharacterControlContext>()->getSpaceShipGameControls()->setEnabled(false);
            
            //disable tank controls
            m_pContext->get<CharacterControlContext>()->getTankGameControls()->setEnabled(false);

            //enable A6C controls
            m_pContext->get<CharacterControlContext>()->getA6cControls()->setEnabled(true);
        }
    }
}