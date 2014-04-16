#pragma once

#include "ViewInterfaces.h"
#include "Actor.h"
#include <set>

namespace engiX
{
    class GameScene;
    class SceneNode;

    typedef std::set<std::shared_ptr<SceneNode>> NodeList;

    class SceneNode : public ISceneNode
    {
    public:
        SceneNode(ActorID actorId, GameScene* pScene) :
            m_pScene(pScene),
            m_actorId(actorId),
            m_pParent(nullptr)
        {
        }

        HRESULT OnPreRender();
        void OnPostRender();
        HRESULT OnConstruct() { return S_OK; }
        void OnUpdate(_In_ const Timer& time) {}
        bool AddChild(_In_ std::shared_ptr<ISceneNode> pChild);
        bool RemoveChild(_In_ ActorID actor);
        void RenderChildren();

    protected:
        ActorID m_actorId;
        GameScene *m_pScene;
        NodeList m_children;
        SceneNode* m_pParent;
    };
}