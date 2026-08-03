#pragma once
#include "ZombieBossStateBase.h"
class ZombieBossStateAppear :
    public ZombieBossStateBase
{
public:
    ZombieBossStateAppear() = default;
    ~ZombieBossStateAppear() = default;

    void OnEnter() override;
    void Update() override;
    void Exit() override;
private:
    int m_frame = 0;
};

