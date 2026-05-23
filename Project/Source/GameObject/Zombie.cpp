#include "Zombie.h"
#include <string>
#include "../Utility/Matrix4x4.h"
#include <cassert>
#include "Player.h"
#include "../Utility/MyLib.h"

namespace
{
	// アニメーション名
	const std::wstring kIdleAnimName = L"Zombie|Idle";
	const std::wstring kWalkAnimName = L"Zombie|Walk";
	const std::wstring kAttackAnimName = L"Zombie|Attack";
	const std::wstring kHitAnimName = L"Zombie|Hit";

	// ゾンビの移動速度
	constexpr float kZombieMoveSpeed = 3.0f;
	// プレイヤーを見つける距離
	constexpr float kPlayerFindDist = 500.0f;
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

	// アニメーションの初期化
	m_anim.Init(m_modelHandle, kIdleAnimName);
}

void Zombie::End()
{}

void Zombie::Update()
{
	// 自分の向きのベクトル
	Vector3 forwardVec = kDefaultDir * Matrix4x4::GetRotY(m_angle);
	// 敵からプレイヤーの位置までのベクトル
	Vector3 toPlayerVec = m_pPlayer->GetPos() - m_pos;
	toPlayerVec.y = 0.0f;	// y軸は無視する
	// 二つのベクトルの角度を計算
	float cos = forwardVec.Dot(toPlayerVec) / (1.0f * toPlayerVec.Length());
	if (cos > kPlayerFindCos && toPlayerVec.SquaredLength() < kPlayerFindDist * kPlayerFindDist)
	{
		// プレイヤーを見つけた時の処理
	}

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
#endif

	// モデルの描画
	MV1DrawModel(m_modelHandle);
}
