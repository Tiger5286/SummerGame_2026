#pragma once
#include "PlayerUIBase.h"
class PlayerHPUI :
    public PlayerUIBase
{
public:
    PlayerHPUI() = default;
    ~PlayerHPUI();

    void Init() override;
    void Update() override;
    void Draw() override;

private:

};

