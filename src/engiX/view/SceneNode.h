#pragma once

#include <set>
#include "ViewInterfaces.h"
#include "Actor.h"
#include "engiXDefs.h"

namespace engiX
{
    class GameScene;
    class SceneNode;

    typedef std::set<std::shared_ptr<SceneNode>> NodeList;

    class SceneNode : public ISceneNode
    {
    public:
        SceneNode(ActorID actorId, GameScene* pScene);
        HRESULT OnPreRender();
        void OnPostRender();
        HRESULT OnConstruct() { return S_OK; }
        void OnUpdate(_In_ const Timer& time);
        bool AddChild(_In_ std::shared_ptr<ISceneNode> pChild);
        bool RemoveChild(_In_ ActorID actor);
        void RenderChildren();

    protected:
        ActorID m_actorId;
        GameScene *m_pScene;
        NodeList m_children;
        SceneNode* m_pParent;
        Mat4x4 m_toParentWorldTsfm;
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