#include "SceneNode.h"
#include <memory>
#include <algorithm>
#include "GameScene.h"
#include "WinGameApp.h"
#include "TransformCmpt.h"
#include "GameLogic.h"

using namespace engiX;
using namespace std;
using namespace DirectX;

SceneNode::SceneNode(_In_ ActorID actorId, _In_ GameScene* pScene) :
m_pScene(pScene),
m_pParent(nullptr),
m_actorId(actorId)
{
    XMStoreFloat4x4(&m_worldTsfm, XMMatrixIdentity());
}

HRESULT SceneNode::OnPreRender()
{
    m_pScene->PushTransformation(m_worldTsfm);
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
    auto& a = g_pApp->Logic()->GetActor(m_actorId);

    if (a.IsNull())
        return;

    m_worldTsfm = a.Get<TransformCmpt>().Transform();

    for (auto pChild : m_children)
        pChild->OnUpdate(time);
}

HRESULT SceneNode::OnConstruct()
{
    for (auto pChild : m_children)
        CHRRHR(pChild->OnConstruct());

    return S_OK;
}

bool SceneNode::AddChild(_In_ shared_ptr<ISceneNode> pChild)
{
    if (m_children.insert(static_pointer_cast<SceneNode>(pChild)).second)
    {
        pChild->Parent(this);
        return true;
    }

    return false;
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