#pragma once
#include "VultureStateBase.h"
#include "Utility/Vector3.h"

class VultureStateAttack :
    public VultureStateBase
{
public:
    VultureStateAttack() = default;
    ~VultureStateAttack() = default;

    void Enter(std::weak_ptr<Vulture> pVulture) override;
    void Update() override;
    void Exit() override;
private:
    int m_frame = 0;
    float m_toPlayerHeightDif = 0.0f;
    Vector3 m_toPlayerDir;
};

