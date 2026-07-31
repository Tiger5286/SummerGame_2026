#include "ZombieBoss.h"
#include "Utility/Matrix4x4.h"
#include "Game/Collider/CapsuleCollider.h"
#include "Singleton/CollisionManager.h"

namespace
{
	constexpr const wchar_t* kInitAnimName = L"ZombieBoss|Idle";

	constexpr float kScale = 2.0f;

	constexpr int kMaxHP = 2000;

	constexpr float kColliderRadius = 70;
	constexpr float kColliderHeight = 250;

	const Vector3 kTargetUIOffset = Vector3(0, 280, 0);
}

void ZombieBoss::Init()
{
	m_anim.Init(m_modelHandle, kInitAnimName);

	m_pCollider = std::make_shared<CapsuleCollider>(kColliderRadius, kColliderHeight);
	CollisionManager::GetInstance().Register(shared_from_this());

	RotateToPlayer();

	m_hp = kMaxHP;

	m_targetUIOffset = kTargetUIOffset;
}

void ZombieBoss::End()
{
}

void ZombieBoss::OnUpdate()
{
	m_pos += m_vel;

	Gravity();
	Resistance();

	// 当たり判定の位置更新
	auto capsule = std::dynamic_pointer_cast<CapsuleCollider>(m_pCollider);
	Vector3 colliderPos = m_pos + Vector3(0, capsule->GetRadius(), 0);
	m_pCollider->SetPos(colliderPos);
	// マップとの当たり判定
	auto collResult = m_pCollider->CheckCollModel(m_mapHandle);
	CheckHitMapCapsule(collResult);
	// 当たり判定に使用したメモリを解放
	MV1CollResultPolyDimTerminate(collResult);

	m_anim.Update();

	// モデルの回転角度を更新
	float diff = MyLib::GetAngleDif(m_angle, m_drawAngle);
	m_drawAngle += diff * 0.1f;

	auto scale = Matrix4x4::GetScale(Vector3(kScale, kScale, kScale));
	auto rot = Matrix4x4::GetRotY(m_drawAngle);
	auto trans = Matrix4x4::GetTranslate(m_pos);
	auto mat = scale * rot * trans;
	MV1SetMatrix(m_modelHandle, mat.ToDxLib());
}

void ZombieBoss::Draw()
{
	MV1DrawModel(m_modelHandle);

	//m_pState->Draw();
#ifdef _DEBUG
	m_pCollider->Draw();
#endif
}

int ZombieBoss::GetMaxHP() const
{
	return kMaxHP;
}