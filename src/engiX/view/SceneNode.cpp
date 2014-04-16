#include "SceneNode.h"
#include <memory>
#include <algorithm>
#include "GameScene.h"
#include "WinGameApp.h"
#include "TransformComponent.h"

using namespace engiX;
using namespace std;

HRESULT SceneNode::OnPreRender()
{
    _ASSERTE(g_pApp->Logic());
    StrongActorPtr pActor((g_pApp->Logic()->GetActor(m_actorId)));
    _ASSERTE(pActor);

    shared_ptr<TransformComponent> pTransformCmpt(pActor->GetComponent<TransformComponent>());
    _ASSERTE(pTransformCmpt);

    m_pScene->PushTransformation(pTransformCmpt->Transform());

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
            pChild->OnRender();

        pChild->OnPostRender();

        pChild->RenderChildren();
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