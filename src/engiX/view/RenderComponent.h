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

        static const ComponentID TypeID = 0x6211EC48;

        BoxMeshComponent(_In_ const Properties& prop) :
            m_props(prop) {}

        ComponentID TypeId() const { return TypeID; }
        const wchar_t* Typename() const { return L"BoxMeshComponent"; }
        const Properties& Props() const { return m_props; }
        std::shared_ptr<ISceneNode> CreateSceneNode(_In_ GameScene* pScene);

    protected:
        Properties m_props;
    };
}