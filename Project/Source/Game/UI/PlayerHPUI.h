#pragma once
#include "PlayerUIBase.h"
#include <vector>

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
    std::vector<int> m_handles;
    int m_fontHandle = -1;
};

