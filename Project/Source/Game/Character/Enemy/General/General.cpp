#include "General.h"
#include "Singleton/ModelManager.h"
#include "../../../Collider/CapsuleCollider.h"
#include "Singleton/CollisionManager.h"

#include "GeneralStateBase.h"
#include "GeneralStateWalk.h"
#include "GeneralStateHeavySlash.h"

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

	// 当たり判定の初期化
	m_pCollider = std::make_shared<CapsuleCollider>(70, 250);
	// 当たり判定の登録
	CollisionManager::GetInstance().Register(shared_from_this());

	// ステートの初期化
	m_pState = std::make_shared<GeneralStateHeavySlash>();
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

	// 当たり判定の位置更新
	auto capsule = std::dynamic_pointer_cast<CapsuleCollider>(m_pCollider);
	Vector3 colliderPos = m_pos + Vector3(0, capsule->GetRadius(), 0);
	m_pCollider->SetPos(colliderPos);
	// マップとの当たり判定
	auto collResult = m_pCollider->CheckCollModel(m_mapHandle);
	CheckHitMapCapsule(collResult);
	// 当たり判定に使用したメモリを解放
	MV1CollResultPolyDimTerminate(collResult);

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

	// ステートに描画したいものがあったら描画
	m_pState->Draw();

#ifdef _DEBUG
	m_pCollider->Draw();
#endif
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
