#pragma once
#include "VultureStateBase.h"
class VultureStateDown :
    public VultureStateBase
{
public:
    VultureStateDown() = default;
    ~VultureStateDown() = default;

    void Enter(std::weak_ptr<Vulture> pVulture) override;
    void Update() override;
    void Exit() override;
private:
    int m_groundFrame = 0;
};

