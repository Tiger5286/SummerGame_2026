#include "BurningWing.h"
#include "Singleton/ModelManager.h"
#include "Utility/Matrix4x4.h"

namespace
{
	const std::wstring kAnimName = L"AngelWIngs_Armature|Burning";
	constexpr int kScaleMaxFrame = 50 * 2;
	constexpr int kBurningEndFrame = 170 * 2;
	constexpr int kAnimMaxFrame = 191 * 2;
}

BurningWing::~BurningWing()
{
	MV1DeleteModel(m_modelHandle);
}

void BurningWing::Init(const Vector3& pos, float angle)
{
	m_pos = pos;
	m_angle = angle;

	m_modelHandle = ModelManager::GetInstance().DuplicateModel(L"Wing");
	auto scaleMtx = Matrix4x4::GetScale(Vector3(m_scale, m_scale, m_scale));
	auto rotMtx = Matrix4x4::GetRotY(m_angle);
	auto transMtx = Matrix4x4::GetTranslate(m_pos);
	auto mtx = scaleMtx * rotMtx * transMtx;
	MV1SetMatrix(m_modelHandle, mtx.ToDxLib());

	m_anim.Init(m_modelHandle, kAnimName);
	m_anim.ChangeAnim(kAnimName, 0.5f, false);
}

void BurningWing::Update()
{
	m_frame++;
	// スケールを徐々に大きくする
	if (m_frame < kScaleMaxFrame)
	{
		float scale = 1.0f / kScaleMaxFrame;
		m_scale += scale;
		if (m_scale > 1.0f)
		{
			m_scale = 1.0f;
		}
	}
	// バーニングが終わったらスケールを徐々に小さくする
	if (m_frame > kBurningEndFrame)
	{
		float scale = 1.0f / (kAnimMaxFrame - kBurningEndFrame);
		m_scale -= scale;
		if (m_scale < 0.0f)
		{
			m_scale = 0.0f;
		}
	}

	auto scaleMtx = Matrix4x4::GetScale(Vector3(m_scale, m_scale, m_scale));
	auto rotMtx = Matrix4x4::GetRotY(m_angle);
	auto transMtx = Matrix4x4::GetTranslate(m_pos);
	auto mtx = scaleMtx * rotMtx * transMtx;
	MV1SetMatrix(m_modelHandle, mtx.ToDxLib());

	m_anim.Update();
}

void BurningWing::Draw()
{
	MV1DrawModel(m_modelHandle);
}
