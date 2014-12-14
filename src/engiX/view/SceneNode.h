#pragma once

#include <set>
#include "ViewInterfaces.h"
#include "Actor.h"
#include "engiXDefs.h"
#include "Actor.h"
#include "TransformCmpt.h"
#include "Geometry.h"

namespace engiX
{
    class GameScene;
    class SceneNode;

    typedef std::set<std::shared_ptr<SceneNode>> NodeList;


    class SceneNode : public ISceneNode
    {
    public:
        typedef std::vector<D3D11Vertex_PositionColored> VertexList;
        typedef std::vector<UINT> IndexList;

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
        RootSceneNode(GameScene* pScene);
        ~RootSceneNode();

        HRESULT OnConstruct();
        void OnRender() {}
        void RenderCoordAxises();

    private:
        VertexList m_dbgCoordsVertices;
        IndexList m_dbgCoordIndices;
        ID3D11Buffer* m_pDbgCoordsVertexBuffer;
        ID3D11Buffer* m_pDbgCoordsIndexBuffer;
    };
}