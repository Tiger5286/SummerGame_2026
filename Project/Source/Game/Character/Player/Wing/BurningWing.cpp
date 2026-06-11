#include "BurningWing.h"
#include "Singleton/ModelManager.h"
#include "Utility/Matrix4x4.h"

namespace
{
	// アニメーション名
	const std::wstring kAnimName = L"AngelWIngs_Armature|Burning";
	// スケールの最大値に達するフレーム数
	constexpr int kScaleMaxFrame = 50 * 2;
	// バーニングが終わるフレーム数
	constexpr int kBurningEndFrame = 170 * 2;
	// アニメーションの最後のフレーム数
	constexpr int kAnimMaxFrame = 191 * 2;
}

BurningWing::~BurningWing()
{	// モデルのハンドルを削除
	MV1DeleteModel(m_modelHandle);
}

void BurningWing::Init(const Vector3& pos, float angle)
{
	// 初期化
	m_pos = pos;
	m_angle = angle;
	// モデルを複製して取得
	m_modelHandle = ModelManager::GetInstance().DuplicateModel(L"Wing");
	// モデルの行列を初期化
	auto scaleMtx = Matrix4x4::GetScale(Vector3(m_scale, m_scale, m_scale));
	auto rotMtx = Matrix4x4::GetRotY(m_angle);
	auto transMtx = Matrix4x4::GetTranslate(m_pos);
	auto mtx = scaleMtx * rotMtx * transMtx;
	MV1SetMatrix(m_modelHandle, mtx.ToDxLib());
	// アニメーションを初期化
	m_anim.Init(m_modelHandle, kAnimName);
	m_anim.ChangeAnim(kAnimName, 0.5f, false);

	//MV1SetOpacityRate(m_modelHandle, 0.99f);
}

void BurningWing::Update()
{
	m_frame++;
	// スケールを徐々に大きくする
	if (m_frame < kScaleMaxFrame)
	{
		float scale = 1.0f / kScaleMaxFrame;
		m_scale += scale * 1.2f;
		if (m_scale > 1.0f)
		{
			m_scale = 1.0f;
		}
	}
	// バーニングが終わったらスケールを徐々に小さくする
	if (m_frame > kBurningEndFrame)
	{
		float scale = 1.0f / (kAnimMaxFrame - kBurningEndFrame);
		m_scale -= scale * 1.4f;
		if (m_scale < 0.0f)
		{
			m_scale = 0.0f;
		}
	}
	// モデルの行列を更新
	auto scaleMtx = Matrix4x4::GetScale(Vector3(m_scale, m_scale, m_scale));
	auto rotMtx = Matrix4x4::GetRotY(m_angle);
	auto transMtx = Matrix4x4::GetTranslate(m_pos);
	auto mtx = scaleMtx * rotMtx * transMtx;
	MV1SetMatrix(m_modelHandle, mtx.ToDxLib());
	// アニメーションを更新
	m_anim.Update();
}

void BurningWing::Draw()
{	// モデルを描画
	MV1DrawModel(m_modelHandle);
}
