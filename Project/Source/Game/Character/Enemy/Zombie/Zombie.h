#pragma once
#include "../EnemyBase.h"
#include "System/Animator.h"

class Player;

class ZombieStateBase;
class ZombieStateIdle;
class ZombieStateMove;
class ZombieStateAttack;
class ZombieStateHit;

class Zombie : public EnemyBase, public std::enable_shared_from_this<Zombie>
{
public:
	Zombie();
	virtual ~Zombie() override;

	void Init() override;
	void End() override;
	void Update() override;
	void Draw() override;

private:
	void CheckChangeState();	// ステートの切り替え処理

	/// <summary>
	/// プレイヤーが検知範囲の扇の中にいるか判定する
	/// </summary>
	/// <returns>true:扇の中にいる / false:扇の外にいる</returns>
	bool IsPlayerInFan();

	/// <summary>
	/// プレイヤーが検知範囲の円の中にいるか判定する
	/// </summary>
	/// <returns>true:円の中にいる / false:円の外にいる</returns>
	bool IsPlayerInCircle();

	/// <summary>
	/// プレイヤーを攻撃する距離内かどうか
	/// </summary>
	/// <returns>true:範囲内 / false:範囲外</returns>
	bool IsPlayerInAttackDist();

private:
	Animator m_anim;
	float m_angle = 0.0f;

	bool m_isFighting = false;

	std::shared_ptr<ZombieStateBase> m_pState = nullptr;

	friend ZombieStateBase;
	friend ZombieStateIdle;
	friend ZombieStateMove;
	friend ZombieStateAttack;
	friend ZombieStateHit;
};

