#pragma once
#include "../../CharacterStateBase.h"
#include <memory>

#include "ZombieBossStateBase.h"

class ZombieBossStateBase :
    public CharacterStateBase
{
public:
    ZombieBossStateBase() = default;
    ~ZombieBossStateBase() = default;

    virtual void OnEnter() = 0;
    virtual void Update() = 0;
    virtual void Exit() = 0;
    virtual void Draw(){}

protected:
    std::weak_ptr<ZombieBossStateBase> m_pZombieBoss;
};

