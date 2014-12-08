#pragma once

#include <memory>
#include "Geometry.h"
#include "D3dShader.h"
#include "Actor.h"
#include "GeometryGenerator.h"

namespace engiX
{
    class GameScene;

    class RenderCmpt : public ActorCmpt
    {
    public:
        void OnUpdate(_In_ const Timer& time) {}
        bool Init() { return true; }
        virtual std::shared_ptr<ISceneNode> CreateSceneNode(_In_ GameScene* pScene) = 0;
        void SceneNode(std::weak_ptr<ISceneNode> sceneNode) { m_sceneNode = sceneNode; }
        std::weak_ptr<ISceneNode> SceneNode() { return m_sceneNode; }

    protected:
        std::weak_ptr<ISceneNode> m_sceneNode;
    };

    class MeshCmptProperties
    {
    public:
        MeshCmptProperties() :
            Color(Color3(0.0, 0.0, 0.0)),
            IsWireframe(true),
            IsBackfacing(false)
        {}

        Color3 Color;
        bool IsWireframe;
        bool IsBackfacing;
    };

    class MeshCmpt : public RenderCmpt
    {
    public:
        DECLARE_COMPONENT(MeshCmpt, 0x6211EC48);
    };

    class BoxMeshCmpt : public MeshCmpt
    {
    public:
        class Properties : public MeshCmptProperties
        {
        public:
            Properties() :
                Width(0.0),
                Height(0.0),
                Depth(0.0)
            {}
            real Width;
            real Height;
            real Depth;
        };

        BoxMeshCmpt(_In_ const Properties& prop) :
            m_props(prop) {}

        const Properties& Props() const { return m_props; }
        std::shared_ptr<ISceneNode> CreateSceneNode(_In_ GameScene* pScene);

    protected:
        Properties m_props;
    };

    class SphereMeshCmpt : public MeshCmpt
    {
    public:
        class Properties : public MeshCmptProperties
        {
        public:
            Properties() :
            Radius(0.0)
            {}
            real Radius;
        };

        SphereMeshCmpt(_In_ const Properties& prop) :
            m_props(prop) {}

        const Properties& Props() const { return m_props; }
        std::shared_ptr<ISceneNode> CreateSceneNode(_In_ GameScene* pScene);

    protected:
        Properties m_props;
    };

    class GridMeshCmpt : public MeshCmpt
    {
    public:
        class Properties : public MeshCmptProperties
        {
        public:
            Properties() :
                Width(0.0),
                Depth(0.0)
            {}
            real Width;
            real Depth;
        };

        GridMeshCmpt(_In_ const Properties& prop) :
            m_props(prop) {}

        const Properties& Props() const { return m_props; }
        std::shared_ptr<ISceneNode> CreateSceneNode(_In_ GameScene* pScene);

    protected:
        Properties m_props;
    };


    class CylinderMeshCmpt : public MeshCmpt
    {
    public:
        class Properties : public MeshCmptProperties
        {
        public:
            Properties() :
                TopRadius(0.0),
                BottomRadius(0.0),
                Height(0.0),
                SliceCount(0),
                StackCount(0)
            {}
            real TopRadius;
            real BottomRadius;
            real Height;
            unsigned SliceCount;
            unsigned StackCount;
        };

        CylinderMeshCmpt(_In_ const Properties& prop) :
            m_props(prop) {}

        const Properties& Props() const { return m_props; }
        std::shared_ptr<ISceneNode> CreateSceneNode(_In_ GameScene* pScene);

    protected:
        Properties m_props;
    };
}