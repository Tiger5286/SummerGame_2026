#include "Zombie.h"
#include <string>
#include "Utility/Matrix4x4.h"
#include <cassert>
#include "Game/Character/Player/Player.h"
#include "Utility/MyLib.h"
#include "Game/Collider/CapsuleCollider.h"

#include "ZombieStateBase.h"
#include "ZombieStateIdle.h"

namespace
{
	// アニメーション名
	const std::wstring kIdleAnimName = L"Zombie|Idle";
	const std::wstring kAttackAnimName = L"Zombie|Attack";
	const std::wstring kHitAnimName = L"Zombie|Hit";
	
	// 当たり判定
	constexpr float kColliderRadius = 25.0f;
	constexpr float kColliderHeight = 120.0f;

	// プレイヤーを見つける距離
	constexpr float kPlayerFindDist = 700.0f;
	constexpr float kUnconditionalFindDist = 100.0f;	// 条件なしで見つける距離(プレイヤーが近すぎるときは角度に関係なく見つける)
	constexpr float kUnconditionalFindDistFighting = 400.0f;	// 戦闘中は条件なしで見つける範囲を広げる

	// プレイヤーを見つける角度			// 45度
	constexpr float kPlayerFindRad = DX_PI_F / 4;
	const float kPlayerFindCos = cosf(kPlayerFindRad);

	// モデルのデフォルトの向き(angleが0の時の向き)
	const Vector3 kDefaultDir = Vector3(0, 0, -1);
}

Zombie::Zombie()
{}

Zombie::~Zombie()
{}

void Zombie::Init()
{
	// プレイヤーが設定されていないならエラー
	assert(m_pPlayer != nullptr && "プレイヤーが設定されていません Zombie::Init()");

	// 当たり判定の初期化
	m_pCollider = std::make_shared<CapsuleCollider>(kColliderRadius, kColliderHeight);

	// アニメーションの初期化
	m_anim.Init(m_modelHandle, kIdleAnimName);

	// ステートの初期化
	m_pState = std::make_shared<ZombieStateIdle>();
	m_pState->ChangeState(m_pState);
	m_pState->Enter(weak_from_this());
	CheckChangeState();
}

void Zombie::End()
{}

void Zombie::Update()
{
	// 次のステートがあったらステートを変更する
	CheckChangeState();

	// ステートの更新
	m_pState->Update();

	// 位置に速度を足す
	m_pos += m_vel;

	// 重力をかける
	Gravity();

	// 当たり判定の更新
	auto capsule = std::dynamic_pointer_cast<CapsuleCollider>(m_pCollider);
	capsule->SetPos(m_pos + Vector3(0.0f, capsule->GetRadius(), 0.0f));

	// マップとの当たり判定
	auto collResult = m_pCollider->CheckCollModel(m_mapHandle);
	CheckHitMap(collResult);
	// 当たり判定に使用したメモリを解放
	MV1CollResultPolyDimTerminate(collResult);

	// 行列を生成してモデルに適用
	auto mtx = Matrix4x4::GetRotY(m_angle) * Matrix4x4::GetTranslate(m_pos);
	MV1SetMatrix(m_modelHandle, mtx.ToDxLib());

	// アニメーションの更新
	m_anim.Update();
}

void Zombie::Draw()
{
#ifdef _DEBUG
	MyLib::DrawFan3D(m_pos, m_angle, kPlayerFindRad, kPlayerFindDist, 8);
	MyLib::DrawCircle3D(m_pos, kUnconditionalFindDist, 32);
#endif

	// モデルの描画
	MV1DrawModel(m_modelHandle);

#ifdef _DEBUG
	m_pCollider->Draw();
#endif
}

void Zombie::CheckChangeState()
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

bool Zombie::IsFindPlayer(bool isFighting)
{
	// 自分の向きのベクトル
	Vector3 forwardVec = kDefaultDir * Matrix4x4::GetRotY(m_angle);
	// 敵からプレイヤーの位置までのベクトル
	Vector3 toPlayerVec = m_pPlayer->GetPos() - m_pos;
	toPlayerVec.y = 0.0f;	// y軸は無視する
	// 二つのベクトルの角度を計算
	float cos = forwardVec.Dot(toPlayerVec) / (1.0f * toPlayerVec.Length());

	bool isFindAngle = cos > kPlayerFindCos;	// プレイヤーが見つける角度の範囲にいるかどうか
	bool isFindDist = toPlayerVec.SquaredLength() < kPlayerFindDist * kPlayerFindDist;	// プレイヤーが見つける距離の範囲にいるかどうか
	bool isUnconditionalFindDist;
	if (!isFighting)	// 戦闘中でないなら見つける範囲が狭い
	{
		isUnconditionalFindDist = toPlayerVec.SquaredLength() < kUnconditionalFindDist * kUnconditionalFindDist;	// 条件なしで見つける距離の範囲にいるかどうか
	}
	else	// 戦闘中なら見つける範囲が広い
	{
		isUnconditionalFindDist = toPlayerVec.SquaredLength() < kUnconditionalFindDistFighting * kUnconditionalFindDistFighting;	// 条件なしで見つける距離の範囲にいるかどうか
	}

	return ((isFindAngle && isFindDist) || isUnconditionalFindDist);
}
