#pragma once

#include <memory>
#include "Geometry.h"
#include "D3dShader.h"
#include "Actor.h"
#include "GeometryGenerator.h"

namespace engiX
{
    class GameScene;

    class RenderComponent : public ActorComponent
    {
    public:
        static const ComponentID TypeID = 0x6211EC48;
        ComponentID TypeId() const { return TypeID; }
        const wchar_t* Typename() const { return L"RenderComponent"; }

        void OnUpdate(_In_ const Timer& time) {}
        bool Init() { return true; }
        virtual std::shared_ptr<ISceneNode> CreateSceneNode(_In_ GameScene* pScene) = 0;
    };

    class BoxMeshComponent : public RenderComponent
    {
    public:
        struct Properties
        {
            real Width;
            real Height;
            real Depth;
            Color3 Color;
        };

        BoxMeshComponent(_In_ const Properties& prop) :
            m_props(prop) {}

        const Properties& Props() const { return m_props; }
        std::shared_ptr<ISceneNode> CreateSceneNode(_In_ GameScene* pScene);

    protected:
        Properties m_props;
    };

    class SphereMeshComponent : public RenderComponent
    {
    public:
        struct Properties
        {
            real Radius;
            Color3 Color;
        };

        SphereMeshComponent(_In_ const Properties& prop) :
            m_props(prop) {}

        const Properties& Props() const { return m_props; }
        std::shared_ptr<ISceneNode> CreateSceneNode(_In_ GameScene* pScene);

    protected:
        Properties m_props;
    };

    class GridMeshComponent : public RenderComponent
    {
    public:
        struct Properties
        {
            real Width;
            real Depth;
            Color3 Color;
        };

        GridMeshComponent(_In_ const Properties& prop) :
            m_props(prop) {}

        const Properties& Props() const { return m_props; }
        std::shared_ptr<ISceneNode> CreateSceneNode(_In_ GameScene* pScene);

    protected:
        Properties m_props;
    };
}