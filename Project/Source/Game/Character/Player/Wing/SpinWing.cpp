#include "SpinWing.h"
#include "Singleton/ModelManager.h"
#include "DxLib.h"
#include "Utility/Matrix4x4.h"

namespace
{
	// アニメーション名
	const std::wstring kAnimName = L"AngelWIngs_Armature|Spin";
	// 攻撃終了フレーム
	constexpr int kEndAttackFrame = 90;
	// 角度の最高速度、初期速度、加速度
	constexpr float kMaxAngleVel = 0.3f;
	constexpr float kDefaultAngleVel = -0.3f;
	constexpr float kAngleVelAccel = 0.01f;
	// 拡大率の加算値
	constexpr float kAddScaleValue = 0.06f;
}

SpinWing::~SpinWing()
{
	// モデルの削除
	MV1DeleteModel(m_handle);
}

void SpinWing::Init(const Vector3& pos)
{
	// 位置の初期化
	m_pos = pos;
	// モデルの複製とアニメーションの初期化
	m_handle = ModelManager::GetInstance().DuplicateModel(L"Wing");
	//MV1SetOpacityRate(m_handle, 0.6f);
	m_anim.Init(m_handle, kAnimName);
	// 角度の初期化
	m_angleVel = kDefaultAngleVel;
	// モデルの行列の初期化
	auto scaleMtx = Matrix4x4::GetScale(Vector3(m_scale, m_scale, m_scale));
	auto rotMtx = Matrix4x4::GetRotY(m_angle);
	auto transMtx = Matrix4x4::GetTranslate(m_pos);
	auto mtx = scaleMtx * rotMtx * transMtx;
	MV1SetMatrix(m_handle, mtx.ToDxLib());
}

void SpinWing::Update()
{
	m_frame++;
	// 良い感じに回転させる
	m_angleVel += kAngleVelAccel;
	if (m_angleVel > kMaxAngleVel)
	{
		m_angleVel = kMaxAngleVel;
	}
	m_angle += m_angleVel;
	// 拡大率を変化させる
	if (m_frame < kEndAttackFrame)
	{	// 最初は拡大させる
		m_scale += kAddScaleValue;
		if (m_scale > 1.0f)
		{
			m_scale = 1.0f;
		}
	}
	else	// 攻撃が終わったら縮小させる
	{
		m_scale -= kAddScaleValue;
		if (m_scale < 0.0f)
		{
			m_scale = 0.0f;
		}
	}
	// モデルの行列の更新
	auto scaleMtx = Matrix4x4::GetScale(Vector3(m_scale, m_scale, m_scale));
	auto rotMtx = Matrix4x4::GetRotY(m_angle);
	auto transMtx = Matrix4x4::GetTranslate(m_pos);
	auto mtx = scaleMtx * rotMtx * transMtx;
	MV1SetMatrix(m_handle, mtx.ToDxLib());
}

void SpinWing::Draw()
{
	// モデルの描画
	MV1DrawModel(m_handle);
}
