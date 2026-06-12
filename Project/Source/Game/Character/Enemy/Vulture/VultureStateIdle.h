#pragma once
#include "VultureStateBase.h"
class VultureStateIdle :
    public VultureStateBase
{
public:
    VultureStateIdle() = default;
    ~VultureStateIdle() = default;

    void Enter(std::weak_ptr<Vulture> pVulture) override;
    void Update() override;
    void Exit() override;
private:

};

