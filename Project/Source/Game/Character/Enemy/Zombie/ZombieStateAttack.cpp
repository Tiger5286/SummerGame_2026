#include "ZombieStateAttack.h"
#include "Zombie.h"
#include "../../Player/Player.h"
#include "Game/Character/Attack.h"
#include "Utility/Matrix4x4.h"

#include "ZombieStateIdle.h"
#include "ZombieStateMove.h"

namespace
{
	const std::wstring kAttackAnimName = L"Zombie|Attack";
	constexpr float kStartColTimeRate = 0.38f;
	constexpr float kEndColTimeRate = 0.45f;

	const Vector3 kColliderOffset = Vector3(0, 100, -60);
	const MyLib::AttackData kAttackData = {
		60.0f,
		40,
		MyLib::CharacterType::Player
	};
}

void ZombieStateAttack::OnEnter()
{
	m_pZombie = std::dynamic_pointer_cast<Zombie>(m_pOwner.lock());
	auto zombie = m_pZombie.lock();
	zombie->m_anim.ChangeAnim(kAttackAnimName, MyLib::kDefaultAnimSpeed,false);
	// プレイヤーの方を向く
	Vector3 toPlayerVec = m_pZombie.lock()->m_pPlayer->GetPos() - m_pZombie.lock()->m_pos;
	float toPlayerAngle = toPlayerVec.Angle();
	zombie->m_angle = toPlayerAngle;
}

void ZombieStateAttack::Update()
{
	auto zombie = m_pZombie.lock();
	float animRate = zombie->m_anim.GetAnimRate();
	// 当たり判定処理
	// 当たり判定開始	当たり判定開始の時間、かつまだ当たり判定をONにしていないなら
	if (animRate > kStartColTimeRate && !m_isOnCollider)
	{
		m_pAtk = std::make_shared<Attack>();
		m_pAtk->SetData(kAttackData,shared_from_this());
		m_pAtk->Init();
		m_isOnCollider = true;
	}
	// 当たり判定終了
	if (animRate > kEndColTimeRate && !m_isOffCollider)
	{
		m_pAtk = nullptr;
		m_isOffCollider = true;
	}
	// 当たり判定の移動
	if (m_pAtk != nullptr)
	{
		Vector3 colPos = zombie->m_pos + (kColliderOffset * Matrix4x4::GetRotY(zombie->m_angle));
		m_pAtk->SetPos(colPos);
		m_pAtk->Update();
	}

	// 攻撃アニメーションが終わった時
	if (zombie->m_anim.IsEnd())
	{
		// 攻撃の範囲内なら
		if (zombie->IsPlayerInAttackDist())
		{
			// もう一度攻撃する
			zombie->m_anim.ChangeAnim(kAttackAnimName, 0.5f, false);
			m_isOnCollider = false;
			m_isOffCollider = false;
			// プレイヤーの方を向く
			Vector3 toPlayerVec = m_pZombie.lock()->m_pPlayer->GetPos() - m_pZombie.lock()->m_pos;
			float toPlayerAngle = toPlayerVec.Angle();
			zombie->m_angle = toPlayerAngle;
		}
		else	// 攻撃の範囲外なら
		{
			// プレイヤーの検知範囲内ならmoveステート
			if (zombie->IsPlayerInCircle() || zombie->IsPlayerInFan())
			{
				ChangeState(std::make_shared<ZombieStateMove>());
			}
			else	// 検知範囲外ならidleステート
			{
				ChangeState(std::make_shared<ZombieStateIdle>());
			}
		}
	}
}

void ZombieStateAttack::Exit()
{
	m_pAtk = nullptr;
}

void ZombieStateAttack::Draw()
{
#ifdef _DEBUG
	if (m_pAtk != nullptr)
	{
		m_pAtk->Draw();
	}
#endif
}
