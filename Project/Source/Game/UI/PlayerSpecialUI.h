#pragma once
#include "PlayerUIBase.h"
#include <vector>

class PlayerSpecialUI : public PlayerUIBase
{
public:
    PlayerSpecialUI();
    ~PlayerSpecialUI();

    void Init() override;
    void Update() override;
    void Draw() override;

private:
    std::vector<int> m_handles;
};