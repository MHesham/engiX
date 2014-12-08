#pragma once

#include <DirectXMath.h>
#include <DirectXPackedVector.h>
#include "engiXDefs.h"
#include "ViewInterfaces.h"
#include "SceneNode.h"

namespace engiX
{
    class GameScene;

    class SceneCameraNode : public SceneNode
    {
    public:
        SceneCameraNode(ActorID ownerId, GameScene* pScene);
        virtual ~SceneCameraNode() {}
        void OnRender() {}
    };
}