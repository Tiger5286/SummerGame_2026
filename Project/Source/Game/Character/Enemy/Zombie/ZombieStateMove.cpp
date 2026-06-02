#include "ZombieStateMove.h"
#include "Zombie.h"
#include "Utility/Matrix4x4.h"
#include "../../Player/Player.h"

#include "ZombieStateIdle.h"
#include "ZombieStateAttack.h"

namespace
{
	const std::wstring kWalkAnimName = L"Zombie|Walk";

	const Vector3 kDefaultDir = Vector3(0, 0, -1);

	// ゾンビの移動速度
	constexpr float kZombieMoveSpeed = 2.0f;
	// ゾンビの回転速度
	constexpr float kRotateSpeed = 0.01f;

	// プレイヤーを追いかけなくなる距離
	constexpr float kStopChaseDist = 100.0f;
}

void ZombieStateMove::Enter(std::weak_ptr<Zombie> pZombie)
{
	m_pZombie = pZombie;
	m_pZombie.lock()->m_anim.ChangeAnim(kWalkAnimName);
	m_pZombie.lock()->m_isFighting = true;
}

void ZombieStateMove::Update()
{
	// 円の中にいるときは回転させる
	if (m_pZombie.lock()->IsPlayerInCircle())
	{
		RotateToPlayer();
		// 扇の中にいる時のみ追いかける
		if (m_pZombie.lock()->IsPlayerInFan())
		{
			ChasePlayer();
		}
	}	// 円の外、かつ扇の中の時も追いかける
	else if (m_pZombie.lock()->IsPlayerInFan())
	{
		RotateToPlayer();
		ChasePlayer();
	}

	// プレイヤーを攻撃する距離内なら攻撃ステートに切り替える
	if (m_pZombie.lock()->IsPlayerInAttackDist())
	{
		ChangeState(std::make_shared<ZombieStateAttack>());
		return;
	}

	// プレイヤーを見つけていないなら待機ステートに切り替える
	if (!m_pZombie.lock()->IsPlayerInFan() && !m_pZombie.lock()->IsPlayerInCircle())
	{
		ChangeState(std::make_shared<ZombieStateIdle>());
		return;
	}
}

void ZombieStateMove::Exit()
{}

void ZombieStateMove::RotateToPlayer()
{
	// 自分の向きのベクトル
	Vector3 forwardVec = kDefaultDir * Matrix4x4::GetRotY(m_pZombie.lock()->m_angle);
	// 敵からプレイヤーの位置までのベクトル
	Vector3 toPlayerVec = m_pZombie.lock()->m_pPlayer->GetPos() - m_pZombie.lock()->m_pos;

	// プレイヤーを見つけた時の処理
	// 外積の正負でどっちに回転すべきか判定
	auto cross = forwardVec.Cross(toPlayerVec);
	// 回転速度を設定
	float rotSpeed = kRotateSpeed;

	// 回転
	if (cross.y < 0)
	{
		m_pZombie.lock()->m_angle -= rotSpeed;
	}
	else if (cross.y > 0)
	{
		m_pZombie.lock()->m_angle += rotSpeed;
	}
}

void ZombieStateMove::ChasePlayer()
{
	// 自分の向きのベクトル
	Vector3 forwardVec = kDefaultDir * Matrix4x4::GetRotY(m_pZombie.lock()->m_angle);
	// 敵からプレイヤーの位置までのベクトル
	Vector3 toPlayerVec = m_pZombie.lock()->m_pPlayer->GetPos() - m_pZombie.lock()->m_pos;

	// プレイヤーが追いかけなくなる距離の範囲にいるかどうか
	bool isStopChaseDist = toPlayerVec.SquaredLength() < kStopChaseDist * kStopChaseDist;
	// プレイヤーが近すぎるときは追いかけない
	if (!isStopChaseDist)
	{
		Vector3 moveVec = forwardVec * kZombieMoveSpeed;
		m_pZombie.lock()->m_vel = moveVec;
	}
}
