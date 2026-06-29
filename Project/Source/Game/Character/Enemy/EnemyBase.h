#pragma once
#include "../Character.h"
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
	// マップモデルのハンドルを設定する(Initの前に実行する)
	void SetMapHandle(int mapHandle) { m_mapHandle = mapHandle; }

	// プレイヤーの方を向く
	void RotateToPlayer();

	bool IsDead() const { return m_isDead; }
	bool IsDying() const { return m_isDying; }

protected:
	int m_mapHandle = -1;
	std::shared_ptr<Player> m_pPlayer = nullptr;
	bool m_isDead = false;	// 完全に死んだときにtrue
	bool m_isDying = false;	// 死ぬモーションの間true
};

