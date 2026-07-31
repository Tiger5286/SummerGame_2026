#include "ZombieBoss.h"
#include "Utility/Matrix4x4.h"

namespace
{
	constexpr const wchar_t* kInitAnimName = L"ZombieBoss|Idle";

	constexpr float kScale = 2.0f;
}

void ZombieBoss::Init()
{
	m_anim.Init(m_modelHandle, kInitAnimName);
}

void ZombieBoss::End()
{
}

void ZombieBoss::OnUpdate()
{
	m_pos += m_vel;

	// モデルの回転角度を更新
	float diff = MyLib::GetAngleDif(m_angle, m_drawAngle);
	m_drawAngle += diff * 0.1f;

	Gravity();
	Resistance();

	m_anim.Update();

	auto scale = Matrix4x4::GetScale(Vector3(kScale, kScale, kScale));
	auto rot = Matrix4x4::GetRotY(m_drawAngle);
	auto trans = Matrix4x4::GetTranslate(m_pos);
	auto mat = scale * rot * trans;
	MV1SetMatrix(m_modelHandle, mat.ToDxLib());
}

void ZombieBoss::Draw()
{
	MV1DrawModel(m_modelHandle);

	//m_pState->Draw();
}

int ZombieBoss::GetMaxHP() const
{
	return 1000;
}