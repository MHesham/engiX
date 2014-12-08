#include "SceneCameraNode.h"
#include "MathHelper.h"
#include "WinGameApp.h"
#include "EventManager.h"
#include "GameLogic.h"

using namespace engiX;
using namespace DirectX;

SceneCameraNode::SceneCameraNode(ActorID ownerId, GameScene* pScene) :
SceneNode(ownerId, pScene)
{
}
