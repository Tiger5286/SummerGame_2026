#pragma once
#include <memory>
#include "../CharacterStateBase.h"

class Player;

class PlayerStateBase : public CharacterStateBase
{
public:
	PlayerStateBase() = default;
	virtual ~PlayerStateBase() = default;

	virtual void Enter(std::weak_ptr<Character> pOwner) override = 0;
	virtual void Update() override = 0;
	virtual void Exit() override = 0;
	virtual void Draw() override {}

	void AddSpecialCharge(int value);

	// 被弾属性(このステートの時どのように被弾するか)
	enum class HitAttribute
	{
		Normal,		// 普通に被弾する
		Invincible,	// 無敵(無敵貫通可)
		PerfectInvincible,	// 完全無敵(無敵貫通不可)
		IgnoreFalter	// 被弾するがひるまない
	};

	/// <summary>
	/// 被弾属性を返す関数
	/// </summary>
	/// <returns>ステートの被弾属性</returns>
	virtual HitAttribute GetHitAttribute() { return HitAttribute::Normal; }

protected:
	std::weak_ptr<Player> m_pPlayer;
	std::shared_ptr<PlayerStateBase> m_pNextStatePlayer = nullptr;
};