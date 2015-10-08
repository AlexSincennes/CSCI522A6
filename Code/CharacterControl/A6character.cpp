#include "PrimeEngine/APIAbstraction/APIAbstractionDefines.h"

#include "PrimeEngine/Lua/LuaEnvironment.h"
#include "PrimeEngine/Scene/SkeletonInstance.h"
#include "PrimeEngine/Scene/MeshInstance.h"
#include "PrimeEngine/Scene/RootSceneNode.h"

#include "A6character.h"
#include "A6cAnimationSM.h"
#include "A6cMovementSM.h"


using namespace PE;
using namespace PE::Components;
using namespace CharacterControl::Events;

namespace CharacterControl{
    namespace Components {

        PE_IMPLEMENT_CLASS1(A6character, Component);

        A6character::A6character(PE::GameContext &context, PE::MemoryArena arena, PE::Handle hMyself, Event_CreateA6Character *pEvt) : Component(context, arena, hMyself)
        {

            // hierarchy of soldier and replated components and variables (note variables are just variables, they are not passed events to)
            // scene
            // +-components
            //   +-soldier scene node
            //   | +-components
            //   |   +-soldier skin
            //   |     +-components
            //   |       +-soldier animation state machine
            //   |       +-soldier weapon skin scene node
            //   |         +-components
            //   |           +-weapon mesh

            // game objects
            // +-components
            //   +-soldier npc
            //     +-variables
            //     | +-m_hMySN = soldier scene node
            //     | +-m_hMySkin = skin
            //     | +-m_hMyGunSN = soldier weapon skin scene node
            //     | +-m_hMyGunMesh = weapon mesh
            //     +-components
            //       +-soldier scene node (restricted to no events. this is for state machines to be able to locate the scene node)
            //       +-movement state machine
            //       +-behavior state machine


            // need to acquire redner context for this code to execute thread-safe
            m_pContext->getGPUScreen()->AcquireRenderContextOwnership(pEvt->m_threadOwnershipMask);

            PE::Handle hmainSN("SCENE_NODE", sizeof(SceneNode));
            SceneNode *pMainSN = new(hmainSN)SceneNode(*m_pContext, m_arena, hmainSN);
            pMainSN->addDefaultComponents();

            pMainSN->m_base.setPos(pEvt->m_pos);
            pMainSN->m_base.setU(pEvt->m_u);
            pMainSN->m_base.setV(pEvt->m_v);
            pMainSN->m_base.setN(pEvt->m_n);


            RootSceneNode::Instance()->addComponent(hmainSN);

            // add the scene node as component of soldier without any handlers. this is just data driven way to locate scnenode for soldier's components
            {
                static int allowedEvts[] = { 0 };
                addComponent(hmainSN, &allowedEvts[0]);
            }

            
            int numskins = 1; // 8
            for (int iSkin = 0; iSkin < numskins; ++iSkin)
            {
                float z = (iSkin / 4) * 1.5f;
                float x = (iSkin % 4) * 1.5f;
                PE::Handle hSN("SCENE_NODE", sizeof(SceneNode));
                SceneNode *pSN = new(hSN)SceneNode(*m_pContext, m_arena, hSN);
                pSN->addDefaultComponents();

                pSN->m_base.setPos(Vector3(x, 0, z));

                // rotation scene node to rotate soldier properly, since soldier from Maya is facing wrong direction
                PE::Handle hRotateSN("SCENE_NODE", sizeof(SceneNode));
                SceneNode *pRotateSN = new(hRotateSN)SceneNode(*m_pContext, m_arena, hRotateSN);
                pRotateSN->addDefaultComponents();

                pSN->addComponent(hRotateSN);

                pRotateSN->m_base.turnLeft(3.1415);

                PE::Handle hA6cAnimSM("A6cAnimationSM", sizeof(A6cAnimationSM));
                A6cAnimationSM *pA6cAnimationSM = new(hA6cAnimSM)A6cAnimationSM(*m_pContext, m_arena, hA6cAnimSM);
                pA6cAnimationSM->addDefaultComponents();

                pA6cAnimationSM->m_debugAnimIdOffset = 0;// rand() % 3;

                PE::Handle hSkeletonInstance("SkeletonInstance", sizeof(SkeletonInstance));
                SkeletonInstance *pSkelInst = new(hSkeletonInstance)SkeletonInstance(*m_pContext, m_arena, hSkeletonInstance,
                    hA6cAnimSM);
                pSkelInst->addDefaultComponents();

                pSkelInst->initFromFiles("MichaelStand_hip.skela", "MichealPackage", pEvt->m_threadOwnershipMask);

                pSkelInst->setAnimSet("MichaelRun_hip.animseta", "MichealPackage");

                PE::Handle hMeshInstance("MeshInstance", sizeof(MeshInstance));
                MeshInstance *pMeshInstance = new(hMeshInstance)MeshInstance(*m_pContext, m_arena, hMeshInstance);
                pMeshInstance->addDefaultComponents();

                pMeshInstance->initFromFile(pEvt->m_meshFilename, pEvt->m_package, pEvt->m_threadOwnershipMask);

                pSkelInst->addComponent(hMeshInstance);

                // add skin to scene node
                pRotateSN->addComponent(hSkeletonInstance);

                /*
                #if !APIABSTRACTION_D3D11
                {
                    PE::Handle hMyGunMesh = PE::Handle("MeshInstance", sizeof(MeshInstance));
                    MeshInstance *pGunMeshInstance = new(hMyGunMesh)MeshInstance(*m_pContext, m_arena, hMyGunMesh);

                    pGunMeshInstance->addDefaultComponents();
                    pGunMeshInstance->initFromFile(pEvt->m_gunMeshName, pEvt->m_gunMeshPackage, pEvt->m_threadOwnershipMask);

                    // create a scene node for gun attached to a joint

                    PE::Handle hMyGunSN = PE::Handle("SCENE_NODE", sizeof(JointSceneNode));
                    JointSceneNode *pGunSN = new(hMyGunSN)JointSceneNode(*m_pContext, m_arena, hMyGunSN, 38);
                    pGunSN->addDefaultComponents();

                    // add gun to joint
                    pGunSN->addComponent(hMyGunMesh);

                    // add gun scene node to the skin
                    pSkelInst->addComponent(hMyGunSN);
                }
                #endif
                */

                pMainSN->addComponent(hSN);
            }

            // now add game objects

            #if 1
            // add movement state machine to character
            PE::Handle ha6cMovementSM("A6cMovementSM", sizeof(A6cMovementSM));
            A6cMovementSM *pa6cMovementSM = new(ha6cMovementSM)A6cMovementSM(*m_pContext, m_arena, ha6cMovementSM);
            pa6cMovementSM->addDefaultComponents();

            // add it to A6c
            addComponent(ha6cMovementSM);
            #endif

            PE::Handle hA6cController("A6cController", sizeof(A6cController));
            A6cController *pA6cController = new(hA6cController)A6cController(*m_pContext, m_arena, hA6cController, 0.05f, pEvt->m_pos, 0.05f, ha6cMovementSM);
            pA6cController->addDefaultComponents();

            // add the same scene node to tank controller
            static int alllowedEventsToPropagate[] = { 0 }; // we will pass empty array as allowed events to propagate so that when we add
            // scene node to the square controller, the square controller doesnt try to handle scene node's events
            // because scene node handles events through scene graph, and is child of square controller just for referencing purposes
            pA6cController->addComponent(hmainSN, &alllowedEventsToPropagate[0]);
            
            addComponent(hA6cController);

            pA6cController->activate();

            m_pContext->getGPUScreen()->ReleaseRenderContextOwnership(pEvt->m_threadOwnershipMask);
        }

        void A6character::addDefaultComponents()
        {
            Component::addDefaultComponents();

            // custom methods of this component
        }

    }; // namespace Components
}; // namespace CharacterControl
