#pragma once
#include "VultureStateBase.h"
class VultureStateIdle :
    public VultureStateBase
{
public:
    VultureStateIdle() = default;
    ~VultureStateIdle() = default;

    void OnEnter() override;
    void Update() override;
    void Exit() override;
private:
    int m_frame = 0;
};

