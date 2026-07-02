#pragma once
#include "UIBase.h"

class Player;

class PlayerUIBase :
    public UIBase
{
public:
    PlayerUIBase();
    virtual ~PlayerUIBase() = default;

    virtual void Init() override = 0;
    virtual void Update() override = 0;
    virtual void Draw() override = 0;

    void SetInfo(std::shared_ptr<Player> pPlayer) { m_pPlayer = pPlayer; }

protected:
    std::weak_ptr<Player> m_pPlayer;
};