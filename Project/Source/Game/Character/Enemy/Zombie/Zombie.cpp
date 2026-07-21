#include "Zombie.h"
#include <string>
#include "Utility/Matrix4x4.h"
#include <cassert>
#include "Game/Character/Player/Player.h"
#include "Utility/MyLib.h"
#include "Game/Collider/CapsuleCollider.h"
#include "Singleton/CollisionManager.h"
#include "../../../UI/EnemyHpBar.h"
#include "../../CharacterStateBase.h"

#include "ZombieStateIdle.h"
#include "ZombieStateHit.h"
#include "ZombieStateDeath.h"

namespace
{
	// アニメーション名
	const std::wstring kIdleAnimName = L"Zombie|Idle";
	
	// 当たり判定
	constexpr float kColliderRadius = 25.0f;
	constexpr float kColliderHeight = 120.0f;

	// プレイヤーを見つける距離
	constexpr float kFindDist = 700.0f;
	constexpr float kUnconditionalFindDist = 250.0f;	// 条件なしで見つける距離(プレイヤーが近すぎるときは角度に関係なく見つける)
	constexpr float kUnconditionalFindDistFighting = 500.0f;	// 戦闘中は条件なしで見つける範囲を広げる
	// プレイヤーを見つける角度			// 50度
	constexpr float kFindAngleRad = DX_PI_F / 3.6f;
	const float kFindAngleCos = cosf(kFindAngleRad);

	// プレイヤーを攻撃する距離
	constexpr float kAttackDist = 140.0f;

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
	// 当たり判定の登録
	CollisionManager::GetInstance().Register(shared_from_this());

	// アニメーションの初期化
	m_anim.Init(m_modelHandle, kIdleAnimName);

	// Hpの初期化
	m_hp = kMaxHp;

	// ステートの初期化
	m_pState = std::make_shared<ZombieStateIdle>();
	m_pState->ChangeState(m_pState);
	auto zombie = std::dynamic_pointer_cast<Zombie>(shared_from_this());
	m_pState->Enter(zombie);
	CheckChangeState();

	// キャラクタータイプをEnemyにする
	m_type = MyLib::CharacterType::Enemy;

	// プレイヤーの方を向く
	RotateToPlayer();

	BaseInit(kMaxHp);

	m_targetUIOffset = Vector3(0, 120, 0);
}

void Zombie::End()
{
}

void Zombie::OnUpdate()
{
	// 位置に速度を足す
	m_pos += m_vel;

	// 重力をかける
	Gravity();
	// 速度に抵抗をかける
	Resistance();

	// 当たり判定の更新
	auto capsule = std::dynamic_pointer_cast<CapsuleCollider>(m_pCollider);
	capsule->SetPos(m_pos + Vector3(0.0f, capsule->GetRadius(), 0.0f));

	// マップとの当たり判定
	auto collResult = m_pCollider->CheckCollModel(m_mapHandle);
	CheckHitMapCapsule(collResult);
	// 当たり判定に使用したメモリを解放
	MV1CollResultPolyDimTerminate(collResult);

	// モデルの回転角度を更新
	float diff = MyLib::GetAngleDif(m_angle, m_drawAngle);
	m_drawAngle += diff * 0.1f;

	// 行列を生成してモデルに適用
	auto mtx = Matrix4x4::GetRotY(m_drawAngle) * Matrix4x4::GetTranslate(m_pos);
	MV1SetMatrix(m_modelHandle, mtx.ToDxLib());

	// アニメーションの更新
	m_anim.Update();
}

void Zombie::Draw()
{
#ifdef _DEBUG
	MyLib::DrawFan3D(m_pos, m_angle, kFindAngleRad, kFindDist, 8);
	if (!m_isFighting)
	{
		MyLib::DrawCircle3D(m_pos, kUnconditionalFindDist, 32);
	}
	else
	{
		MyLib::DrawCircle3D(m_pos, kUnconditionalFindDistFighting, 32);
	}
#endif

	// モデルの描画
	MV1DrawModel(m_modelHandle);

	m_pState->Draw();

#ifdef _DEBUG
	m_pCollider->Draw();
#endif
}

void Zombie::OnCollision(Character& other)
{
}

void Zombie::OnHitAttack(const MyLib::AttackData& atkData)
{
	// 基底クラスの関数を呼ぶ
	Character::OnHitAttack(atkData);

	// 死亡ステートなら被弾しない
	std::shared_ptr<ZombieStateBase> state = nullptr;
	state = std::dynamic_pointer_cast<ZombieStateDeath>(m_pState);
	if (state != nullptr)
	{
		return;
	}
	state = nullptr;

	m_hp -= atkData.damage;
	// hpがなくなったら死亡
	if (m_hp <= 0)
	{
		m_pState->ChangeState(std::make_shared<ZombieStateDeath>());
	}
	else	// hpがあるなら被弾
	{
		m_pState->ChangeState(std::make_shared<ZombieStateHit>());
	}

	//printfDx(L"ゾンビが攻撃を食らった！HP:%d\n",m_hp);
}

bool Zombie::IsPlayerInFan()
{
	// 自分の向きのベクトル
	Vector3 forwardVec = kDefaultDir * Matrix4x4::GetRotY(m_angle);
	// 敵からプレイヤーの位置までのベクトル
	Vector3 toPlayerVec = m_pPlayer->GetPos() - m_pos;
	toPlayerVec.y = 0.0f;	// y軸は無視する
	// 二つのベクトルの角度を計算
	float cos = forwardVec.Dot(toPlayerVec) / (1.0f * toPlayerVec.Length());
	bool isFindAngle = cos > kFindAngleCos;
	// 敵からプレイヤーまでのベクトルの長さを比較
	bool isFindDist = toPlayerVec.SquaredLength() < kFindDist * kFindDist;

	return isFindDist && isFindAngle;
}

bool Zombie::IsPlayerInCircle()
{
	// 敵からプレイヤーの位置までのベクトル
	Vector3 toPlayerVec = m_pPlayer->GetPos() - m_pos;

	if (m_isFighting)
	{
		return toPlayerVec.SquaredLength() < kUnconditionalFindDistFighting * kUnconditionalFindDistFighting;
	}
	else
	{
		return toPlayerVec.SquaredLength() < kUnconditionalFindDist * kUnconditionalFindDist;
	}
}

bool Zombie::IsPlayerInAttackDist()
{
	// 敵からプレイヤーの位置までのベクトル
	Vector3 toPlayerVec = m_pPlayer->GetPos() - m_pos;
	return toPlayerVec.SquaredLength() < kAttackDist * kAttackDist;;
}
