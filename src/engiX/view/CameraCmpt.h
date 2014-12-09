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
        ~CameraCmpt();

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