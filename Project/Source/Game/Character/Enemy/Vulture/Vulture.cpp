#include "Vulture.h"
#include <cassert>
#include "../../../Collider/SphereCollider.h"
#include "Singleton/CollisionManager.h"
#include "Utility/Matrix4x4.h"
#include "Utility/MyLib.h"

#include "VultureStateIdle.h"
#include "VultureStateHit.h"
#include "VultureStateDown.h"
#include "VultureStateDeath.h"

namespace
{
	constexpr float kScale = 0.5f;
	const Matrix4x4 kScaleMatrix = Matrix4x4::GetScale(Vector3(kScale, kScale, kScale));

	constexpr float kColliderRadius = 50.0f;

	const std::wstring kIdleAnimName = L"VultureCinereous_Skelmesh|VultureCinereous_Flying";

	constexpr int kMaxHP = 300;

	constexpr float kRiseSpeed = 2.0f;
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
	// 飛んでいるので接地フラグを切る
	m_isGround = false;

	// ステートの初期化
	m_pState = std::make_shared<VultureStateIdle>();
	m_pState->ChangeState(m_pState);
	m_pState->Enter(weak_from_this());
	CheckChangeState();

	// キャラクタータイプをEnemyにする
	m_type = MyLib::CharacterType::Enemy;
}

void Vulture::End()
{}

void Vulture::Update()
{
	CheckChangeState();

	m_pState->Update();

	// 飛んでいなかったら落下する
	if (!m_isFlying)
	{
		Gravity();
	}
	else	// 飛んでいたら高度を維持する
	{
		KeepHeight();
	}
	Resistance();

	m_pos += m_vel;

	// 当たり判定の更新
	m_pCollider->SetPos(m_pos + Vector3::Up() * kColliderRadius);

	// マップとの当たり判定
	auto collResult = m_pCollider->CheckCollModel(m_mapHandle);
	CheckHitMapSphere(collResult);
	// 当たり判定に使用したメモリを解放
	MV1CollResultPolyDimTerminate(collResult);

	// モデルの回転角度を更新
	float diff = MyLib::GetAngleDif(m_angle, m_drawAngle);
	m_drawAngle += diff * 0.1f;

	// 行列を生成してモデルに適用
	auto rotMtx = Matrix4x4::GetRotY(m_drawAngle);
	auto transMtx = Matrix4x4::GetTranslate(m_pos);
	auto mtx = kScaleMatrix * rotMtx * transMtx;
	MV1SetMatrix(m_modelHandle, mtx.ToDxLib());

	// アニメーションの更新
	m_anim.Update();
}

void Vulture::Draw()
{
	MV1DrawModel(m_modelHandle);

	m_pState->Draw();

#ifdef _DEBUG
	m_pCollider->Draw();

	Vector3 end = m_pos - Vector3::Up() * m_flyHeight;
	DrawLine3D(m_pos.ToDxLib(), end.ToDxLib(), 0xffff00);
#endif
}

void Vulture::OnHitAttack(const MyLib::AttackData& atkData)
{
	// 死亡ステートなら被弾しない
	std::shared_ptr<VultureStateBase> state = nullptr;
	state = std::dynamic_pointer_cast<VultureStateDeath>(m_pState);
	if (state != nullptr)
	{
		return;
	}
	state = nullptr;

	m_hp -= atkData.damage;
	// hpがなくなったら死亡
	if (m_hp <= 0)
	{
		m_pState->ChangeState(std::make_shared<VultureStateDeath>());
	}
	else	// hpがあるなら被弾
	{
		if (atkData.isKnockDown)
		{	// ノックダウン属性の攻撃ならダウンする
			m_pState->ChangeState(std::make_shared<VultureStateDown>());
		}
		else	// そうでなければ普通に被弾する
		{
			m_pState->ChangeState(std::make_shared<VultureStateHit>());
		}
	}
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

void Vulture::KeepHeight()
{
	Vector3 end = m_pos - Vector3::Up() * m_flyHeight;
	auto result = MV1CollCheck_Line(m_mapHandle, -1, m_pos.ToDxLib(), end.ToDxLib());
	if (result.HitFlag)
	{
		m_vel.y = kRiseSpeed;
	}
	else
	{
		m_vel.y = 0.0f;
	}
}
