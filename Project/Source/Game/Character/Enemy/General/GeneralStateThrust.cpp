#include "GeneralStateThrust.h"
#include "General.h"
#include "../../Player/Player.h"
#include "Utility/MyLib.h"
#include "../../Attack.h"

#include "GeneralStateWalk.h"

namespace
{
	const std::wstring kAnimName = L"General|Thrust";

	constexpr int kStartThrustFrame = 60;
	constexpr int kEndThrustFrame = 90;
	constexpr int kEndStateFrame = 240;

	constexpr float kThrustSpeed = 25.0f;

	const Vector3 kAttackOffset = Vector3(0, 150, -150);
	const MyLib::AttackData kAttackData = {
		.colliderRadius = 150.0f,
		.damage = 20,
		.hitCharacterType = MyLib::CharacterType::Player,
		.isKnockDown = false,
		.isIgnoreInvincible = false
	};
}

void GeneralStateThrust::Enter(std::weak_ptr<Character> pOwner)
{
	m_pGeneral = std::dynamic_pointer_cast<General>(pOwner.lock());
	auto general = m_pGeneral.lock();
	general->m_anim.ChangeAnim(kAnimName, 0.5f, false);
	// プレイヤーの方向を向く
	Vector3 toPlayer = general->m_pPlayer->GetPos() - general->m_pos;
	general->m_angle = MyLib::GetAngleVec(toPlayer.z, toPlayer.x);
}

void GeneralStateThrust::Update()
{
	m_frame++;
	auto general = m_pGeneral.lock();

	// 突進開始
	if (m_frame == kStartThrustFrame)
	{
		// 当たり判定の生成
		m_pAtk = std::make_shared<Attack>();
		m_pAtk->SetData(kAttackData);
		m_pAtk->Init();
		Vector3 colPos = general->m_pos + kAttackOffset * Matrix4x4::GetRotY(general->m_angle);
		m_pAtk->SetPos(colPos);
	}
	// 突進
	if (m_frame > kStartThrustFrame && m_frame < kEndThrustFrame)
	{
		Vector3 moveVec = MyLib::GetVecAngle(general->m_angle);
		moveVec *= kThrustSpeed;
		general->m_vel = moveVec;
		// 当たり判定の位置更新
		Vector3 colPos = general->m_pos + kAttackOffset * Matrix4x4::GetRotY(general->m_angle);
		m_pAtk->SetPos(colPos);
	}
	// 突進終了
	if (m_frame > kEndThrustFrame)
	{
		m_pAtk = nullptr;
	}

	// 当たり判定の更新
	if (m_pAtk != nullptr)
	{
		m_pAtk->Update();
	}

	// 一定時間経過したらステートを終了
	if (m_frame > kEndStateFrame)
	{
		ChangeState(std::make_shared<GeneralStateWalk>());
		return;
	}
}

void GeneralStateThrust::Exit()
{

}

void GeneralStateThrust::Draw()
{
#ifdef _DEBUG
	if (m_pAtk != nullptr)
	{
		m_pAtk->Draw();
	}
#endif
}