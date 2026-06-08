#pragma once
#include "CameraStateBase.h"
class CameraStateFree :
    public CameraStateBase
{
public:
    CameraStateFree() = default;
    ~CameraStateFree() = default;

    void Enter(std::weak_ptr<Camera> pCamera) override;
    void Update() override;
    void Exit() override;
private:

};

