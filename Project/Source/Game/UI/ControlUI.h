#pragma once
#include "UIBase.h"
#include <vector>

class Player;

class ControlUI :
    public UIBase
{
public:
    ControlUI();
    ~ControlUI();

    void Init() override;
    void Update() override;
    void Draw() override;

    void SetInfo(std::shared_ptr<Player> pPlayer) { m_pPlayer = pPlayer; }

private:
    std::vector<int> m_handles;
    std::vector<float> m_scale;
    int m_fontHandle = -1;

    std::weak_ptr<Player> m_pPlayer;
};

