#pragma once

#include "ViewInterfaces.h"
#include "Actor.h"

namespace engiX
{
    class SceneNode : public ISceneNode
    {
    public:
        HRESULT OnPreRender() { return S_OK; }
        void OnPostRender() {}
        void OnRender() {}
        HRESULT OnConstruct() { return S_OK; }
        void OnUpdate(_In_ const Timer& time) {}

    protected:
        ActorID m_actor;
    };
}