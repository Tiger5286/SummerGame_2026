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
