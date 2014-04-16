#pragma once

#include "SceneNode.h"
#include "D3dShader.h"

namespace engiX
{
    class D3dGeneratedMeshNode : public SceneNode
    {
    public:
        D3dGeneratedMeshNode(ActorID actorId, GameScene* pScene);
        HRESULT OnPreRender();
        HRESULT OnConstruct();
        void OnRender();

    protected:
        D3dShader m_shader;
    };
}