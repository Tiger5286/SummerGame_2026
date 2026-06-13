#include "Vulture.h"
#include <cassert>
#include "../../../Collider/SphereCollider.h"
#include "Singleton/CollisionManager.h"
#include "Utility/Matrix4x4.h"

#include "VultureStateIdle.h"
#include "VultureStateHit.h"

namespace
{
	constexpr float kScale = 0.5f;
	const Matrix4x4 kScaleMatrix = Matrix4x4::GetScale(Vector3(kScale, kScale, kScale));

	constexpr float kColliderRadius = 50.0f;

	const std::wstring kIdleAnimName = L"VultureCinereous_Skelmesh|VultureCinereous_Flying";

	constexpr int kMaxHP = 200;
}

void Vulture::Init()
{
	// プレイヤーが設定されていないならエラー
	assert(m_pPlayer != nullptr && "プレイヤーが設定されていません Zombie::Init()");

	// 当たり判定の初期化
	m_pCollider = std::make_shared<SphereCollider>(kColliderRadius);
	// 当たり判定の登録
	CollisionManager::GetInstance().Register(shared_from_this());

	// アニメーションの初期化
	m_anim.Init(m_modelHandle, kIdleAnimName);

	// Hpの初期化
	m_hp = kMaxHP;

	// ステートの初期化
	m_pState = std::make_shared<VultureStateIdle>();
	m_pState->ChangeState(m_pState);
	m_pState->Enter(weak_from_this());
	CheckChangeState();

	// キャラクタータイプをEnemyにする
	m_type = Type::Enemy;
}

void Vulture::End()
{}

void Vulture::Update()
{
	CheckChangeState();

	m_pState->Update();

	auto rotMtx = Matrix4x4::GetRotY(m_drawAngle);
	auto transMtx = Matrix4x4::GetTranslate(m_pos);
	auto mtx = kScaleMatrix * rotMtx * transMtx;
	MV1SetMatrix(m_modelHandle, mtx.ToDxLib());

	m_pCollider->SetPos(m_pos + Vector3::Up() * kColliderRadius);

	m_anim.Update();
}

void Vulture::Draw()
{
	MV1DrawModel(m_modelHandle);

	m_pState->Draw();

#ifdef _DEBUG
	m_pCollider->Draw();
#endif
}

void Vulture::OnHitAttack(int damage)
{
	m_pState->ChangeState(std::make_shared<VultureStateHit>());
	return;
}

void Vulture::CheckChangeState()
{
	auto nextState = m_pState->GetNextState();
	// 次のステートがある場合は切り替え
	if (m_pState != nextState)
	{
		m_pState->Exit();

		m_pState = nextState;

		m_pState->Enter(weak_from_this());

		m_pState->ChangeState(m_pState);
	}
}
