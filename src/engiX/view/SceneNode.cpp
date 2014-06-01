#include "SceneNode.h"
#include <memory>
#include <algorithm>
#include "GameScene.h"
#include "WinGameApp.h"
#include "TransformCmpt.h"

using namespace engiX;
using namespace std;
using namespace DirectX;

SceneNode::SceneNode(_In_ ActorID actorId, _In_ GameScene* pScene) :
    m_pScene(pScene),
    m_pParent(nullptr),
    m_actorId(actorId)
{
    XMStoreFloat4x4(&m_localTsfm, XMMatrixIdentity());
    XMStoreFloat4x4(&m_frmParentWorldTsfm, XMMatrixIdentity());

    m_actor = g_pApp->Logic()->FindActor(actorId);

    if (!m_actor.expired())
    {
        m_actorTsfm = m_actor.lock()->Get<TransformCmpt>();
        _ASSERTE(!m_actorTsfm.expired());
    }
}

HRESULT SceneNode::OnPreRender()
{
    m_pScene->PushTransformation(m_localTsfm);
    
    if (!m_actorTsfm.expired())
    {
        m_actorTsfm.lock()->ToWorldTransform(m_pScene->TopTransformation());
    }

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
    if (!m_actorTsfm.expired())
    {
        m_localTsfm = m_actorTsfm.lock()->LocalTransform();
    }

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