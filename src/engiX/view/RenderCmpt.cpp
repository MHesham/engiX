#include "RenderCmpt.h"
#include "D3dGeneratedMeshNode.h"
#include "GameScene.h"
#include "GeometryGenerator.h"

using namespace engiX;
using namespace std;

shared_ptr<ISceneNode> BoxMeshCmpt::CreateSceneNode(_In_ GameScene* pScene)
{
    GeometryGenerator g;
    GeometryGenerator::MeshData data;

    g.CreateBox(m_props.Width, m_props.Height, m_props.Depth, data);
    data.IsBackfacing = m_props.IsBackfacing;
    data.IsWireframe = m_props.IsWireframe;

    _ASSERTE(pScene);
    return shared_ptr<ISceneNode>(eNEW D3dGeneratedMeshNode(Owner()->Id(), data, m_props.Color, pScene));
}

shared_ptr<ISceneNode> SphereMeshCmpt::CreateSceneNode(_In_ GameScene* pScene)
{
    GeometryGenerator g;
    GeometryGenerator::MeshData data;

    g.CreateGeosphere(m_props.Radius, 2, data);
    data.IsBackfacing = m_props.IsBackfacing;
    data.IsWireframe = m_props.IsWireframe;

    _ASSERTE(pScene);
    return shared_ptr<ISceneNode>(eNEW D3dGeneratedMeshNode(Owner()->Id(), data, m_props.Color, pScene));
}

shared_ptr<ISceneNode> GridMeshCmpt::CreateSceneNode(_In_ GameScene* pScene)
{
    GeometryGenerator g;
    GeometryGenerator::MeshData data;

    g.CreateGrid(m_props.Width, m_props.Depth, 10, 10, data);
    data.IsBackfacing = m_props.IsBackfacing;
    data.IsWireframe = m_props.IsWireframe;

    _ASSERTE(pScene);
    return shared_ptr<ISceneNode>(eNEW D3dGeneratedMeshNode(Owner()->Id(), data, m_props.Color, pScene));
}

shared_ptr<ISceneNode> CylinderMeshCmpt::CreateSceneNode(_In_ GameScene* pScene)
{
    GeometryGenerator g;
    GeometryGenerator::MeshData data;

    g.CreateCylinder(m_props.BottomRadius, m_props.TopRadius, m_props.Height, m_props.SliceCount, m_props.StackCount, data);
    data.IsBackfacing = m_props.IsBackfacing;
    data.IsWireframe = m_props.IsWireframe;

    _ASSERTE(pScene);
    return shared_ptr<ISceneNode>(eNEW D3dGeneratedMeshNode(Owner()->Id(), data, m_props.Color, pScene));
}
