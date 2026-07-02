#pragma once
#include "PlayerUIBase.h"

class PlayerSpecialUI : public PlayerUIBase
{
public:
    PlayerSpecialUI();
    ~PlayerSpecialUI();

    void Init() override;
    void Update() override;
    void Draw() override;

private:

};