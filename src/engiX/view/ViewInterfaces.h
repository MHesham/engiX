#pragma once

#include <Windows.h>
#include "Timer.h"
#include "Actor.h"

namespace engiX
{
    class GameScene;

    class IGameView
    {
    public:
        virtual ~IGameView() {}
        virtual bool Init() = 0;
        virtual void OnRender() = 0;
        virtual void OnUpdate(_In_ const Timer& time) = 0;
        virtual HRESULT OnConstruct() = 0;
        virtual bool OnMsgProc(_In_ const Timer& time, _In_ UINT uMsg, _In_ WPARAM wParam, _In_ LPARAM lParam) = 0;
    };

    class ISceneNode
    {
    public:
        virtual ~ISceneNode() {}
        virtual HRESULT OnPreRender() = 0;
        virtual void OnPostRender() = 0;
        virtual void OnRender() = 0;
        virtual void RenderChildren() = 0;
        virtual void OnUpdate(_In_ const Timer& time) = 0;
        virtual HRESULT OnConstruct() = 0;
        virtual bool AddChild(_In_ std::shared_ptr<ISceneNode> pChild) = 0;
        virtual bool RemoveChild(_In_ ActorID actor) = 0;
        virtual GameScene* Scene() = 0;
        virtual ISceneNode* Parent() const = 0;
        virtual void Parent(ISceneNode* pParent) = 0;
    };

    class ID3dShader
    {
    public:
        virtual ~ID3dShader() {}
        virtual HRESULT OnPreRender(ISceneNode* pNode) = 0;
        virtual HRESULT OnConstruct() = 0;
    };


}