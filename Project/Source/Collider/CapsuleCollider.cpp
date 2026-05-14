#include "CapsuleCollider.h"
#include "DxLib.h"

CapsuleCollider::CapsuleCollider(float radius, float height) :
	ColliderBase(ColliderType::Capsule),
	m_radius(radius),
	m_height(height)
{
}

CapsuleCollider::~CapsuleCollider()
{
}

void CapsuleCollider::Draw()
{
	DrawCapsule3D(m_pos.ToDxLib(),
				 (m_pos + Vector3::Up() * m_height).ToDxLib(),
				 m_radius, 8, 0xffff00, 0xffffff, false);
}

MV1_COLL_RESULT_POLY_DIM CapsuleCollider::CheckCollModel(int mapHandle)
{
	auto result = MV1CollCheck_Capsule(mapHandle, -1,
		m_pos.ToDxLib(),
		(m_pos + Vector3::Up() * m_height).ToDxLib(),
		m_radius);

	return result;
}
