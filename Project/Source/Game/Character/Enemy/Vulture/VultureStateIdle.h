#pragma once
#include "VultureStateBase.h"
class VultureStateIdle :
    public VultureStateBase
{
public:
    VultureStateIdle() = default;
    ~VultureStateIdle() = default;

    void Enter(std::weak_ptr<Character> pOwner) override;
    void Update() override;
    void Exit() override;
private:
    int m_frame = 0;
};

