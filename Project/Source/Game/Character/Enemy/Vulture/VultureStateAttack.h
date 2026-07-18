#pragma once
#include "VultureStateBase.h"
#include "Utility/Vector3.h"

class Attack;

class VultureStateAttack :
    public VultureStateBase
{
public:
    VultureStateAttack() = default;
    ~VultureStateAttack() = default;

    void OnEnter() override;
    void Update() override;
    void Exit() override;
    void Draw() override;
private:
    int m_frame = 0;
    float m_toPlayerHeightDif = 0.0f;
    Vector3 m_toPlayerDir;
    std::shared_ptr<Attack> m_pAtk = nullptr;
};

