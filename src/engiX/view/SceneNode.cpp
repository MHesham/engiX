#include "SceneNode.h"
#include <memory>
#include <algorithm>
#include "GameScene.h"
#include "WinGameApp.h"
#include "TransformComponent.h"

using namespace engiX;
using namespace std;
using namespace DirectX;

SceneNode::SceneNode(ActorID actorId, GameScene* pScene) :
    m_pScene(pScene),
    m_actorId(actorId),
    m_pParent(nullptr)
{
    XMStoreFloat4x4(&m_toParentWorldTsfm, XMMatrixIdentity());
}

HRESULT SceneNode::OnPreRender()
{
    m_pScene->PushTransformation(m_toParentWorldTsfm);

    return S_OK;
}

void SceneNode::OnPostRender()
{
    m_pScene->PopTransformation();
}

void SceneNode::RenderChildren()
{
    for (auto pChild : m_children)
    {
        if (SUCCEEDED(pChild->OnPreRender()))
        {
            pChild->OnRender();
            pChild->OnPostRender();
        }

        pChild->RenderChildren();
    }
}

void SceneNode::OnUpdate(_In_ const Timer& time)
{
    if (m_actorId != NullActorID)
    {
        _ASSERTE(g_pApp->Logic());
        StrongActorPtr pActor((g_pApp->Logic()->FindActor(m_actorId)));

        shared_ptr<TransformComponent> pTransformCmpt(pActor->GetComponent<TransformComponent>());
        _ASSERTE(pTransformCmpt);

        m_toParentWorldTsfm = pTransformCmpt->Transform();
    }
}

bool SceneNode::AddChild(_In_ shared_ptr<ISceneNode> pChild)
{
    return m_children.insert(static_pointer_cast<SceneNode>(pChild)).second;
}

bool SceneNode::RemoveChild(_In_ ActorID actor)
{
    auto where = find_if(m_children.begin(), m_children.end(), 
        [actor](const shared_ptr<SceneNode>& child){ return child->m_actorId == actor; });

    if (where == m_children.end())
        return false;

    m_children.erase(where);
    return true;
}