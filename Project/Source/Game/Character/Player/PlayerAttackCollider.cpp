#include "PlayerAttackCollider.h"
#include "Game/Collider/SphereCollider.h"

namespace
{
	constexpr float kColliderRadius = 100.0f;
}

void PlayerAttackCollider::Init()
{
	m_pCollider = std::make_shared<SphereCollider>(kColliderRadius);

	m_type = Type::PlayerAttackCol;
}

void PlayerAttackCollider::End()
{

}

void PlayerAttackCollider::Update()
{
	m_pCollider->SetPos(m_pos);
}

void PlayerAttackCollider::Draw()
{
#ifdef _DEBUG
	m_pCollider->Draw();
#endif
}

void PlayerAttackCollider::OnCollision(Character& other)
{
	// 同じIDがあるかどうか探す
	auto it = std::find(m_hitIds.begin(), m_hitIds.end(), other.GetID());
	// 同じIDがなかった(初めて当たった相手)の場合
	if (it == m_hitIds.end())
	{
		// 当たった相手のIDを登録
		m_hitIds.push_back(other.GetID());
		// 当たった相手が敵だったら
		if (other.GetType() == Type::Enemy)
		{
			other.OnHitAttack();
		}
	}
}
