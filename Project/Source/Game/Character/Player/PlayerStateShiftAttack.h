#pragma once
#include "PlayerStateBase.h"
class PlayerStateShiftAttack :
    public PlayerStateBase
{
public:
    PlayerStateShiftAttack() = default;
    ~PlayerStateShiftAttack() = default;
    
    void OnEnter() override;
    void Update() override;
    void Exit() override;
private:

};

