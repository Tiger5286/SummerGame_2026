#include "SphereCollider.h"
#include "Dxlib.h"

SphereCollider::SphereCollider(float radius):
	ColliderBase(ColliderType::Sphere),
	m_radius(radius)
{
}

SphereCollider::~SphereCollider()
{
}

void SphereCollider::Draw()
{
	unsigned int color = 0xffff00;
	if (!m_isEnable) color = 0x222200;

	DrawSphere3D(m_pos.ToDxLib(), m_radius, 8, color, 0xffffff, false);
}

MV1_COLL_RESULT_POLY_DIM SphereCollider::CheckCollModel(int mapHandle)
{
	auto result = MV1CollCheck_Sphere(mapHandle, -1, m_pos.ToDxLib(), m_radius);

	return result;
}
