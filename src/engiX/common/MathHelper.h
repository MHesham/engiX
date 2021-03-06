//***************************************************************************************
// MathHelper.h by Frank Luna (C) 2011 All Rights Reserved.
//
// Helper math class.
//***************************************************************************************

#pragma  once

#include <Windows.h>
#include <DirectXMath.h>
#include <DirectXPackedVector.h>
#include <crtdbg.h>
#include "engiXDefs.h"

namespace engiX
{
    class Math
    {
    public:
        // Returns random real in [0, 1).
        static real RandF()
        {
            return (real)(rand()) / (real)RAND_MAX;
        }

        // Returns random real in [a, b).
        static real RandF(real a, real b)
        {
            return a + RandF()*(b-a);
        }

        template<typename T>
        static T Min(const T& a, const T& b)
        {
            return a < b ? a : b;
        }

        template<typename T>
        static T Max(const T& a, const T& b)
        {
            return a > b ? a : b;
        }

        template<typename T>
        static T Lerp(const T& a, const T& b, real t)
        {
            return a + (b-a)*t;
        }

        template<typename T>
        static T Clamp(T x, T low, T high)
        {
            return x < low ? low : (x > high ? high : x); 
        }

        // Returns the polar angle of the point (x,y) in [0, 2*PI).
        static real AngleFromXY(real x, real y);

        static DirectX::XMMATRIX InverseTranspose(DirectX::CXMMATRIX M)
        {
            // Inverse-transpose is just applied to normals.  So zero out 
            // translation row so that it doesn't get into our inverse-transpose
            // calculation--we don't want the inverse-transpose of the translation.
            DirectX::XMMATRIX A = M;
            A.r[3] = DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);

            DirectX::XMVECTOR det = XMMatrixDeterminant(A);
            return DirectX::XMMatrixTranspose(DirectX::XMMatrixInverse(&det, A));
        }

        // Spherical Coordinates (radius r, inclination Theta, azimuth Phi)
        // Radius r: The radius of the spherical coordinate system
        // Inclination Theta: Rotation angle in radians around Y axis in the XZ plane, Theta = [0, 2Pi]
        // Azimuth Phi: Rotation angle in radians around the axis between the sphere center and the rotate point around the Y axis, Phi = [0, Pi]
        // The method assumes the provided Theta and Phi are within the correct range
        // Conversion formulas: http://en.wikipedia.org/wiki/Spherical_coordinate_system#Cartesian_coordinates
        //
        // In a LH coordinate system like DX, the rotation angle is counter clock wise and the reference angle for
        // rotating around the Y axis in the XZ plane is the counter clock wise angle with the vector (1.0, 0.0, 0.0)
        //
        //
        static void ConvertSphericalToCartesian(_In_ const real& sphericalRadius, _In_ const real& sphericalTheta, _In_ const real& sphericalPhi, _Out_ DirectX::XMFLOAT3& cartesianXyz)
        {
            _ASSERTE(sphericalRadius >= 0.0f);
            _ASSERTE(sphericalTheta >= 0.0f);
            _ASSERTE(sphericalTheta <= DirectX::g_XMTwoPi.f[0]);
            _ASSERTE(sphericalPhi >= 0.0f);
            _ASSERTE(sphericalPhi <= DirectX::g_XMPi.f[0]);

            cartesianXyz.x = sphericalRadius * real_sin(sphericalPhi) * real_cos(sphericalTheta);
            cartesianXyz.z = sphericalRadius * real_sin(sphericalPhi) * real_sin(sphericalTheta);
	        cartesianXyz.y = sphericalRadius * real_cos(sphericalPhi);
        }

        static void TransformSetPosition(_Inout_ Mat4x4& m, _In_ const Vec3& p)
        {
            m._41 = p.x;
            m._42 = p.y;
            m._43 = p.z;
        }

        static void TransformZeroPosition(_Inout_ Mat4x4& m)
        {
            TransformSetPosition(m, Vec3(0.0f, 0.0f, 0.0f));
        }

        // res = res + vec * scale
        static void Vec3ScaledAdd(_In_ const Vec3& vec, _In_ real scale, _Inout_ Vec3& res)
        {
            // res = res + vec * scale
            DirectX::XMStoreFloat3(&res,
                DirectX::XMVectorMultiplyAdd(
                DirectX::XMLoadFloat3(&vec), DirectX::XMVectorReplicate(scale), DirectX::XMLoadFloat3(&res)));
        }

        // res = res + a^b
        static void Vec3AddPow(_In_ const real& a, _In_ real b, _Inout_ Vec3& res)
        {
            // res = res + a^b
            DirectX::XMStoreFloat3(&res,
                DirectX::XMVectorMultiply(DirectX::XMLoadFloat3(&res), DirectX::XMVectorReplicate(real_pow(a, b))));
        }

        static Vec3 Vec3RotTransform(_In_ const Vec3& v0, _In_ const Mat4x4& tsfm)
        {
            Mat4x4 rotTsfm = tsfm;
            Math::TransformZeroPosition(rotTsfm);

            Vec3 v;
            DirectX::XMStoreFloat3(&v,
                DirectX::XMVector3Transform(
                DirectX::XMLoadFloat3(&v0),
                DirectX::XMLoadFloat4x4(&rotTsfm)));

            return v;
        }

        static void Vec3Accumulate(_Inout_ Vec3& v, _In_ const Vec3& acc)
        {
            DirectX::XMStoreFloat3(&v,
                DirectX::XMVectorAdd(DirectX::XMLoadFloat3(&v), DirectX::XMLoadFloat3(&acc)));
        }

        static DirectX::XMVECTOR RandUnitVec3();
        static DirectX::XMVECTOR RandHemisphereUnitVec3(DirectX::XMVECTOR n);

        static const real Infinity;
        static const real Pi;
    };

#define POW2(X) ((X) * (X))
}