#pragma once

#include "RenderCmpt.h"
#include "Events.h"

namespace engiX
{
    class CameraCmpt : public RenderCmpt
    {
    public:
        DECLARE_COMPONENT(CameraCmpt, 0xEC812388);

        const static real DefaultNearPlane;
        const static real DefaultFarPlane;
        const static real DefaultFovAngle;

        CameraCmpt();

        // Place the camera in its own space using spherical coordinates (radius r, inclination Theta, azimuth Phi)
        // Radius r: The radius of the spherical coordinate system
        // Inclination Theta: Rotation angle in radians around Y axis in the XZ plane, Theta = [0, 2Pi]
        // Azimuth Phi: Rotation angle in radians around the axis between the sphere center and the rotate point around the Y axis, Phi = [0, Pi]
        // The method assumes the provided Theta and Phi are within the correct range
        // More on the spherical coordinates here: http://en.wikipedia.org/wiki/Spherical_coordinate_system#Cartesian_coordinates
        void PlaceOnSphere(_In_ real radius, _In_ real theta, _In_ real phi, _In_ Vec3 lookat = Vec3(DirectX::g_XMZero));
        void OnUpdate(_In_ const Timer& time);
        void SetAsThirdPerson(ActorID targetId) { m_targetId = targetId; }
        void OnDisplaySettingsChangedEvt(EventPtr pEvt);
        const Mat4x4& ViewTsfm() const { return m_viewTsfm; }
        const Mat4x4& ProjTsfm() const { return m_projTsfm; }
        std::shared_ptr<ISceneNode> CreateSceneNode(_In_ GameScene* pScene);

    protected:
        Mat4x4 m_viewTsfm;
        Mat4x4 m_projTsfm;
        real m_nearPlane;
        real m_farPlane;
        real m_fovAngle;
        ActorID m_targetId;
    };
}