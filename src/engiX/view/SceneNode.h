#pragma once

#include <set>
#include "ViewInterfaces.h"
#include "Actor.h"
#include "engiXDefs.h"
#include "Actor.h"
#include "TransformCmpt.h"

namespace engiX
{
    class GameScene;
    class SceneNode;

    typedef std::set<std::shared_ptr<SceneNode>> NodeList;

    class SceneNode : public ISceneNode
    {
    public:
        SceneNode(_In_ ActorID actorId, _In_ GameScene* pScene);
        HRESULT OnPreRender();
        void OnPostRender();
        HRESULT OnConstruct();
        void OnUpdate(_In_ const Timer& time);
        bool AddChild(_In_ std::shared_ptr<ISceneNode> pChild);
        bool RemoveChild(_In_ ActorID actor);
        void RenderChildren();
        GameScene* Scene() { return m_pScene; }
        ISceneNode* Parent() const { return m_pParent; }
        void Parent(ISceneNode* pParent) { m_pParent = pParent; }

    protected:
        ActorID m_actorId;
        GameScene *m_pScene;
        NodeList m_children;
        ISceneNode* m_pParent;
        Mat4x4 m_worldTsfm;
    };

    class RootSceneNode : public SceneNode
    {
    public:
        RootSceneNode(GameScene* pScene) :
            SceneNode(NullActorID, pScene)
        {
        }

        void OnRender() {}
    };
}