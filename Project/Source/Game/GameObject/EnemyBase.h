#pragma once
#include "GameObject.h"
#include <memory>

class Player;

class EnemyBase :
    public GameObject
{
public:
    EnemyBase();
	virtual ~EnemyBase() override;

	virtual void Init() override = 0;
	virtual void End() override = 0;
	virtual void Update() override = 0;
	virtual void Draw() override = 0;

	// プレイヤーのポインタを設定する(Initの前に実行する)
	void SetPlayer(const std::shared_ptr<Player> pPlayer) { m_pPlayer = pPlayer; }

protected:
	std::shared_ptr<Player> m_pPlayer = nullptr;
};

