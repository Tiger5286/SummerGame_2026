#include "Attack.h"
#include "Game/Collider/SphereCollider.h"
#include "Singleton/CollisionManager.h"
#include "../Character/Player/PlayerStateBase.h"

namespace
{
	// ヒットストップのフレーム数
	constexpr int kHitStopFrame = 3;
}

void Attack::Init()
{
	m_pCollider = std::make_shared<SphereCollider>(m_data.colliderRadius);
	m_data.attacker = m_pOwner.lock()->GetOwner();
	CollisionManager::GetInstance().Register(shared_from_this());
}

void Attack::End()
{}

void Attack::OnUpdate()
{
	m_pos += m_vel;

	m_pCollider->SetPos(m_pos);

	m_data.pos = m_pos;
}

void Attack::Draw()
{
#ifdef _DEBUG
	m_pCollider->Draw();
#endif
}

void Attack::OnCollision(Character& other)
{
	// 同じIDがあるかどうか探す
	auto it = std::find(m_hitIds.begin(), m_hitIds.end(), other.GetID());
	// 同じIDがなかった(初めて当たった相手)の場合
	if (it == m_hitIds.end())
	{
		// 当たった相手のIDを登録
		m_hitIds.push_back(other.GetID());
		// 当たった相手が設定された相手だったら
		if (other.GetType() == m_data.hitCharacterType)
		{
			// 攻撃が当たった相手が攻撃を食らえないなら状態ならreturn
			if (!other.IsCanHitAttack()) return;

			// 攻撃を当てた相手のOnHitAttack関数を呼ぶ
			other.OnHitAttack(m_data);
			// 攻撃の持ち主と当たった相手にヒットストップをかける
			std::shared_ptr<Character> attacker = m_data.attacker.lock();
			attacker->SetHitStop(kHitStopFrame, false);
			other.SetHitStop(kHitStopFrame, true);

			// 攻撃を当てた時必殺技ゲージを貯める
			auto playerState = std::dynamic_pointer_cast<PlayerStateBase>(m_pOwner.lock());
			if (playerState != nullptr)
			{
				playerState->AddSpecialCharge(m_data.specialCharge);
			}
		}
	}
}

void Attack::SetData(MyLib::AttackData data, std::shared_ptr<CharacterStateBase> pOwner)
{
	m_data = data;
	m_pOwner = pOwner;
}
