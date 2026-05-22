#include "Zombie.h"
#include <string>
#include "../Utility/Matrix4x4.h"
#include <cassert>
#include "Player.h"

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
	constexpr float kPlayerFindTheta = DX_PI_F / 4;

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
	// 二つのベクトルの角度を計算
	float theta = forwardVec.Dot(toPlayerVec) / (forwardVec.Length() * toPlayerVec.Length());
	//printfDx(L"theta:%.2f\n", theta);

	// 行列を生成してモデルに適用
	auto mtx = Matrix4x4::GetRotY(m_angle) * Matrix4x4::GetTranslate(m_pos);
	MV1SetMatrix(m_modelHandle, mtx.ToDxLib());

	// アニメーションの更新
	m_anim.Update();
}

void Zombie::Draw()
{
#ifdef _DEBUG
	Vector3 lineVec1 = kDefaultDir * Matrix4x4::GetRotY(kPlayerFindTheta) * kPlayerFindDist;
	Vector3 lineVec2 = kDefaultDir * Matrix4x4::GetRotY(-kPlayerFindTheta) * kPlayerFindDist;
	Vector3 lineVec3 = kDefaultDir * kPlayerFindDist;
	DrawLine3D(m_pos.ToDxLib(), (m_pos + lineVec1).ToDxLib(), 0xff0000);
	DrawLine3D(m_pos.ToDxLib(), (m_pos + lineVec2).ToDxLib(), 0xff0000);
	DrawLine3D(m_pos.ToDxLib(), (m_pos + lineVec3).ToDxLib(), 0xff0000);

	constexpr int temp = 16;
	const auto theta = kPlayerFindTheta / temp;
	for (int i = 0; i < temp; i++)
	{
		Vector3 tempV = lineVec2 * Matrix4x4::GetRotY(theta * i);
		Vector3 tempV2 = lineVec2 * Matrix4x4::GetRotY(theta * (i + 1));
		DrawLine3D((m_pos + tempV).ToDxLib(), (m_pos + tempV2).ToDxLib(), 0xff0000);

		tempV = lineVec1 * Matrix4x4::GetRotY(-theta * i);
		tempV2 = lineVec1 * Matrix4x4::GetRotY(-theta * (i + 1));
		DrawLine3D((m_pos + tempV).ToDxLib(), (m_pos + tempV2).ToDxLib(), 0xff0000);
	}
#endif

	// モデルの描画
	MV1DrawModel(m_modelHandle);
}
