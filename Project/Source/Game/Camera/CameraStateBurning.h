#pragma once
#include "CameraStateBase.h"
#include "Utility/Vector3.h"

class CameraStateBurning :
    public CameraStateBase
{
public:
    CameraStateBurning() = default;
    ~CameraStateBurning() = default;

    void Enter(std::weak_ptr<Camera> pCamera) override;
    void Update() override;
    void Exit() override;
private:
    int m_frame = 0;
    float m_angle = 0.0f;
    Vector3 m_posOffset;
};

