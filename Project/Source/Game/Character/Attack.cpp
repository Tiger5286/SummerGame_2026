#include "Attack.h"
#include "Game/Collider/SphereCollider.h"
#include "Singleton/CollisionManager.h"

void Attack::Init()
{
	m_pCollider = std::make_shared<SphereCollider>(m_data.colliderRadius);
	CollisionManager::GetInstance().Register(shared_from_this());
}

void Attack::End()
{}

void Attack::Update()
{
	m_pos += m_vel;

	m_pCollider->SetPos(m_pos);
}

void Attack::Draw()
{
#ifdef _DEBUG
	m_pCollider->Draw();
#endif
}

void Attack::OnCollision(Character & other)
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
			other.OnHitAttack(m_data);
		}
	}
}
