#pragma once
#include "Character.h"
#include <memory>

class Player;

class EnemyBase :
    public Character
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

	bool IsDead() const { return m_isDead; }

protected:
	std::shared_ptr<Player> m_pPlayer = nullptr;
	bool m_isDead = false;
};

