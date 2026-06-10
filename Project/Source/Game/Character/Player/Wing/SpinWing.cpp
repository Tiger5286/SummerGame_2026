#include "SpinWing.h"
#include "Singleton/ModelManager.h"
#include "DxLib.h"
#include "Utility/Matrix4x4.h"

namespace
{
	const std::wstring kAnimName = L"AngelWIngs_Armature|Spin";

	constexpr int kEndAttackFrame = 45 * 2;
}

SpinWing::~SpinWing()
{
	MV1DeleteModel(m_handle);
}

void SpinWing::Init(const Vector3& pos)
{
	m_pos = pos;

	m_handle = ModelManager::GetInstance().DuplicateModel(L"Wing");
	m_anim.Init(m_handle, kAnimName);

	m_angleVel = -0.3f;

	auto scaleMtx = Matrix4x4::GetScale(Vector3(m_scale, m_scale, m_scale));
	auto rotMtx = Matrix4x4::GetRotY(m_angle);
	auto transMtx = Matrix4x4::GetTranslate(m_pos);
	auto mtx = scaleMtx * rotMtx * transMtx;
	MV1SetMatrix(m_handle, mtx.ToDxLib());
}

void SpinWing::Update()
{
	m_frame++;

	m_angleVel += 0.01f;
	if (m_angleVel > 0.3f)
	{
		m_angleVel = 0.3f;
	}
	m_angle += m_angleVel;

	if (m_frame < kEndAttackFrame)
	{
		m_scale += 0.06f;
		if (m_scale > 1.0f)
		{
			m_scale = 1.0f;
		}
	}
	else
	{
		m_scale -= 0.06f;
		if (m_scale < 0.0f)
		{
			m_scale = 0.0f;
		}
	}

	auto scaleMtx = Matrix4x4::GetScale(Vector3(m_scale, m_scale, m_scale));
	auto rotMtx = Matrix4x4::GetRotY(m_angle);
	auto transMtx = Matrix4x4::GetTranslate(m_pos);
	auto mtx = scaleMtx * rotMtx * transMtx;
	MV1SetMatrix(m_handle, mtx.ToDxLib());
}

void SpinWing::Draw()
{
	MV1DrawModel(m_handle);
}
