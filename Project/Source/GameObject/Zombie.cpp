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
	// ゾンビの回転速度
	constexpr float kRotateSpeed = 0.01f;
	//constexpr float kRotateSpeedNear = 0.1f;	// 近いときは早く回転する

	// プレイヤーを見つける距離
	constexpr float kPlayerFindDist = 700.0f;
	constexpr float kUnconditionalFindDist = 100.0f;	// 条件なしで見つける距離(プレイヤーが近すぎるときは角度に関係なく見つける)
	// プレイヤーを追いかけなくなる距離
	constexpr float kStopChaseDist = 100.0f;

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

	bool isFindAngle = cos > kPlayerFindCos;	// プレイヤーが見つける角度の範囲にいるかどうか
	bool isFindDist = toPlayerVec.SquaredLength() < kPlayerFindDist * kPlayerFindDist;	// プレイヤーが見つける距離の範囲にいるかどうか
	bool isUnconditionalFindDist = toPlayerVec.SquaredLength() < kUnconditionalFindDist * kUnconditionalFindDist;	// 条件なしで見つける距離の範囲にいるかどうか
	bool isStopChase = toPlayerVec.SquaredLength() < kStopChaseDist * kStopChaseDist;	// プレイヤーが追いかけなくなる距離の範囲にいるかどうか

	if (((isFindAngle && isFindDist) || isUnconditionalFindDist))
	{
		// プレイヤーを見つけた時の処理
		// 外積の正負でどっちに回転すべきか判定
		auto cross = forwardVec.Cross(toPlayerVec);
		// 回転速度を設定
		float rotSpeed = kRotateSpeed;
		//// 近いなら早く回転
		//if (isUnconditionalFindDist)
		//{
		//	rotSpeed = kRotateSpeedNear;
		//}

		// 回転
		if (cross.y < 0)
		{
			m_angle -= rotSpeed;
		}
		else if (cross.y > 0)
		{
			m_angle += rotSpeed;
		}
		
		// プレイヤーが近すぎるときは追いかけない
		if (!isStopChase)
		{
			Vector3 moveVec = forwardVec * kZombieMoveSpeed;
			m_pos += moveVec;
		}
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
	MyLib::DrawCircle3D(m_pos, kUnconditionalFindDist, 32);
#endif

	// モデルの描画
	MV1DrawModel(m_modelHandle);
}
