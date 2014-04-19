#include "RenderComponent.h"
#include "D3dGeneratedMeshNode.h"
#include "GameScene.h"
#include "GeometryGenerator.h"

using namespace engiX;
using namespace std;

shared_ptr<ISceneNode> BoxMeshComponent::CreateSceneNode(_In_ GameScene* pScene)
{
    GeometryGenerator g;
    GeometryGenerator::MeshData data;

    g.CreateBox(m_props.Width, m_props.Height, m_props.Depth, data);

    _ASSERTE(pScene);
    return shared_ptr<ISceneNode>(eNEW D3dGeneratedMeshNode(Owner(), data, m_props.Color, pScene));
}

shared_ptr<ISceneNode> SphereMeshComponent::CreateSceneNode(_In_ GameScene* pScene)
{
    GeometryGenerator g;
    GeometryGenerator::MeshData data;

    g.CreateGeosphere(m_props.Radius, 2, data);

    _ASSERTE(pScene);
    return shared_ptr<ISceneNode>(eNEW D3dGeneratedMeshNode(Owner(), data, m_props.Color, pScene));
}

shared_ptr<ISceneNode> GridMeshComponent::CreateSceneNode(_In_ GameScene* pScene)
{
    GeometryGenerator g;
    GeometryGenerator::MeshData data;

    g.CreateGrid(m_props.Width, m_props.Depth, 10, 10, data);

    _ASSERTE(pScene);
    return shared_ptr<ISceneNode>(eNEW D3dGeneratedMeshNode(Owner(), data, m_props.Color, pScene));
}

