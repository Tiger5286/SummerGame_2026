#pragma once
#include "UIBase.h"
#include <vector>

class EnemyBase;

class BossHpBar :
    public UIBase
{
public:
    BossHpBar();
    ~BossHpBar();

    void Init() override;
    void Update() override;
    void Draw() override;

    // Initの前に実行する
    void SetInfo(std::shared_ptr<EnemyBase> pOwner) { m_pOwner = pOwner; }

private:
    std::weak_ptr<EnemyBase> m_pOwner;
    std::vector<int> m_handles;
};

