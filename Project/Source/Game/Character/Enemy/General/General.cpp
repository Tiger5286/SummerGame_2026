#include "General.h"
#include "Singleton/ModelManager.h"

#include "GeneralStateBase.h"
#include "GeneralStateWalk.h"

namespace
{
	constexpr float kGeneralScale = 2.0f;

	constexpr float kSwordScale = 2.5f;
	constexpr float kSwordRotZ = -DX_PI_F / 2;
	const Vector3 kSwordOffset = Vector3(0, 35, 0);
}

General::~General()
{
	MV1DeleteModel(m_swordModelHandle);
}

void General::Init()
{
	m_swordModelHandle = ModelManager::GetInstance().DuplicateModel(L"GeneralSword");

	m_anim.Init(m_modelHandle, L"General|Idle");

	// ステートの初期化
	m_pState = std::make_shared<GeneralStateWalk>();
	m_pState->ChangeState(m_pState);
	m_pState->Enter(weak_from_this());
	CheckChangeState();
}

void General::End()
{

}

void General::Update()
{
	CheckChangeState();
	m_pState->Update();

	m_pos += m_vel;

	

	m_anim.Update();
}

void General::Draw()
{
	// モデルの回転角度を更新
	float diff = MyLib::GetAngleDif(m_angle, m_drawAngle);
	m_drawAngle += diff * 0.1f;

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

void General::CheckChangeState()
{
	auto nextState = m_pState->GetNextState();
	// 次のステートがある場合は切り替え
	if (m_pState != nextState)
	{
		m_pState->Exit();

		m_pState = nextState;

		m_pState->Enter(weak_from_this());

		m_pState->ChangeState(m_pState);
	}
}
