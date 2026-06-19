#pragma once
#include "../EnemyBase.h"
#include "System/Animator.h"

class GeneralStateBase;
class GeneralStateWalk;

class General :
    public EnemyBase,public std::enable_shared_from_this<General>
{
public:
    General() = default;
    ~General();

    void Init() override;
    void End() override;
    void Update() override;
    void Draw() override;

private:
    void CheckChangeState();

private:
    int m_swordModelHandle = -1;
    Animator m_anim;

    std::shared_ptr<GeneralStateBase> m_pState = nullptr;

private:
    friend GeneralStateWalk;
};