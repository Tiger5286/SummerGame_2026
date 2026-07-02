#pragma once
#include "PlayerUIBase.h"
class PlayerSkillUI :
    public PlayerUIBase
{
public:
    PlayerSkillUI() = default;
    ~PlayerSkillUI();

    void Init() override;
    void Update() override;
    void Draw() override;

private:
    int m_handle = -1;
};

