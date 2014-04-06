#pragma once

#include <DirectXMath.h>
#include <DirectXPackedVector.h>
#include "engiXDefs.h"
#include "Events.h"
#include "Delegate.h"

namespace engiX
{
    class SceneCamera
    {
    public:
        const static real DefaultNearPlane;
        const static real DefaultFarPlane;
        const static real DefaultFovAngle;

        SceneCamera();
        virtual ~SceneCamera() {}

        Mat4x4 ViewProjMatrix() const { return m_viewProjMat; }

        // Place the camera in its own space using spherical coordinates (radius r, inclination Theta, azimuth Phi)
        // Radius r: The radius of the spherical coordinate system
        // Inclination Theta: Rotation angle in radians around Y axis in the XZ plane, Theta = [0, 2Pi]
        // Azimuth Phi: Rotation angle in radians around the axis between the sphere center and the rotate point around the Y axis, Phi = [0, Pi]
        // The method assumes the provided Theta and Phi are within the correct range
        // More on the spherical coordinates here: http://en.wikipedia.org/wiki/Spherical_coordinate_system#Cartesian_coordinates
        void PlaceOnSphere(_In_ real radius, _In_ real theta, _In_ real phi);
    protected:
        void OnDisplaySettingsChanged(EventPtr evt);
        void OnToggleCamera(EventPtr evt);
        void BuildViewProjMatrix();

        Mat4x4 m_viewProjMat;
        Vec3 m_pos;
        real m_nearPlane;
        real m_farPlane;
        real m_fovAngle;
        Delegate1P<SceneCamera, EventPtr> m_displayChangeHandler;
        Delegate1P<SceneCamera, EventPtr> m_toggleCameraHandler;
    };
}