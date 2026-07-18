#include "General.h"
#include "Singleton/ModelManager.h"
#include "../../../Collider/CapsuleCollider.h"
#include "Singleton/CollisionManager.h"
#include "Singleton/UIManager.h"

#include "Game/UI/BossHpBar.h"

#include "GeneralStateBase.h"
#include "GeneralStateWalk.h"
#include "GeneralStateHeavySlash.h"
#include "GeneralStateThrust.h"
#include "GeneralStateProjectile.h"
#include "GeneralStateDeath.h"
#include "GeneralStateAppear.h"

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
	m_isBoss = true;

	m_swordModelHandle = ModelManager::GetInstance().DuplicateModel(L"GeneralSword");

	m_anim.Init(m_modelHandle, L"General|Idle");

	// 当たり判定の初期化
	m_pCollider = std::make_shared<CapsuleCollider>(70, 250);
	// 当たり判定の登録
	CollisionManager::GetInstance().Register(shared_from_this());

	// ステートの初期化
	m_pState = std::make_shared<GeneralStateAppear>();
	m_pState->ChangeState(m_pState);
	auto general = std::dynamic_pointer_cast<General>(shared_from_this());
	m_pState->Enter(general);
	CheckChangeState();

	RotateToPlayer();

	m_type = MyLib::CharacterType::Enemy;

	m_hp = kMaxHp;

	m_pBossBar = std::make_shared<BossHpBar>();
	m_pBossBar->SetInfo(general);
	UIManager::GetInstance().AddUI(m_pBossBar);

	m_targetUIOffset = Vector3(0, 280, 0);
}

void General::End()
{

}

void General::OnUpdate()
{
	m_pos += m_vel;

	Gravity();
	Resistance();

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

#ifdef _DEBUG
	if (CheckHitKey(KEY_INPUT_3))
	{
		m_hp = 1;
	}
#endif
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
	auto frameIndex = MV1SearchFrame(m_modelHandle, L"mixamorig:RightHand");
	auto m = MV1GetFrameLocalWorldMatrix(m_modelHandle, frameIndex);
	resultMat = MMult(MMult(MMult(scale, rot), trans), m);
	MV1SetMatrix(m_swordModelHandle, resultMat);
	MV1DrawModel(m_swordModelHandle);

	// ステートに描画したいものがあったら描画
	m_pState->Draw();

#ifdef _DEBUG
	m_pCollider->Draw();
#endif
}

void General::OnHitAttack(const MyLib::AttackData& atkData)
{
	std::shared_ptr<GeneralStateDeath> state = nullptr;
	state = std::dynamic_pointer_cast<GeneralStateDeath>(m_pState);
	if (state != nullptr) return;	// 現在のステートがDeathならreturn
	state = nullptr;

	m_hp -= atkData.damage;

	if (m_hp <= 0)
	{
		m_pState->ChangeState(std::make_shared<GeneralStateDeath>());
	}
}

void General::AttackRandom()
{
	// クールタイムをリセット
	m_attackCooltime = kAttackCooltime;
	// 攻撃の種類をランダムで決定する
	int rand = GetRand(2);
	switch (rand)
	{
	case 0:
		m_pState->ChangeState(std::make_shared<GeneralStateHeavySlash>());
		return;
	case 1:
		m_pState->ChangeState(std::make_shared<GeneralStateThrust>());
		return;
	case 2:
		m_pState->ChangeState(std::make_shared<GeneralStateProjectile>());
		return;
	}
}
