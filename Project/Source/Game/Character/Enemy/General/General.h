#pragma once
#include "../EnemyBase.h"
class General :
    public EnemyBase
{
public:
    General() = default;
    ~General() = default;

    void Init() override;
    void End() override;
    void Update() override;
    void Draw() override;

private:

};

