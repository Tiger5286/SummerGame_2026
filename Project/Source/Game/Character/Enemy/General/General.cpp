#include "General.h"
#include "Singleton/ModelManager.h"

namespace
{
	constexpr float kGeneralScale = 2.0f;

	constexpr float kSwordScale = 2.5f;
	constexpr float kSwordRotZ = -DX_PI_F / 2;
	const Vector3 kSwordOffset = Vector3(0, 10, 0);
}

General::~General()
{
	MV1DeleteModel(m_swordModelHandle);
}

void General::Init()
{
	m_swordModelHandle = ModelManager::GetInstance().DuplicateModel(L"GeneralSword");

	m_anim.Init(m_modelHandle, L"Armature|Idle");
}

void General::End()
{

}

void General::Update()
{
	m_anim.Update();
}

void General::Draw()
{
	// 敵本体の描画
	auto scale = MGetScale(VGet(kGeneralScale, kGeneralScale, kGeneralScale));
	auto rot = MGetRotY(m_drawAngle);
	auto trans = MGetTranslate(m_pos.ToDxLib());
	auto resultMat = MMult(MMult(scale, rot), trans);
	MV1SetMatrix(m_modelHandle, resultMat);
	MV1DrawModel(m_modelHandle);

	// 剣の描画
	scale = MGetScale(VGet(kSwordScale, kSwordScale, kSwordScale));
	rot = MGetRotZ(kSwordRotZ);
	trans = MGetTranslate(kSwordOffset.ToDxLib());
	auto m = MV1GetFrameLocalWorldMatrix(m_modelHandle, 37);
	resultMat = MMult(MMult(MMult(scale, rot), trans), m);
	MV1SetMatrix(m_swordModelHandle, resultMat);
	MV1DrawModel(m_swordModelHandle);
}