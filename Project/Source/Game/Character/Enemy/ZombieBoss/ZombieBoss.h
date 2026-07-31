#pragma once
#include "../EnemyBase.h"

class ZombieBoss :
    public EnemyBase
{
public:
    ZombieBoss() = default;
    ~ZombieBoss() = default;

    void Init() override;
    void End() override;
    void OnUpdate() override;
    void Draw() override;

private:

};

