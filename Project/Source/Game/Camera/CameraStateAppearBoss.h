#pragma once
#include "CameraStateBase.h"
class CameraStateAppearBoss :
    public CameraStateBase
{
public:
    CameraStateAppearBoss() = default;
    ~CameraStateAppearBoss() = default;

    void Enter(std::weak_ptr<Camera> pCamera) override;
    void Update() override;
    void Exit() override;

private:
    int m_frame = 0;
};

