#include "GeneralStateHeavySlash.h"
#include "General.h"
#include "../../Player/Player.h"
#include "Utility/MyLib.h"
#include "Utility/Matrix4x4.h"
#include "../../Attack.h"
#include "Singleton/EffectManager.h"

#include "GeneralStateWalk.h"

namespace
{
	const std::wstring kAnimName = L"General|HeavySlash";

	constexpr int kAttackFrame = 33 * 2;
	constexpr int kStartSlashFrame = 54 * 2;
	constexpr int kEndSlashFrame = 85 * 2;

	const Vector3 kAttackOffset = Vector3(0, 150, -150);
	const MyLib::AttackData kAttackData = {
		.colliderRadius		= 150.0f,
		.damage				= 50,
		.hitCharacterType	= MyLib::CharacterType::Player,
		.isKnockDown		= false,
		.isIgnoreInvincible	= true
	};
}

void GeneralStateHeavySlash::OnEnter()
{
	m_pGeneral = std::dynamic_pointer_cast<General>(m_pOwner.lock());
	auto general = m_pGeneral.lock();
	general->m_anim.ChangeAnim(kAnimName, 0.5f, false);
	// プレイヤーの方向を向く
	Vector3 toPlayer = general->m_pPlayer->GetPos() - general->m_pos;
	general->m_angle = MyLib::GetAngleVec(toPlayer.z, toPlayer.x);
}

void GeneralStateHeavySlash::Update()
{
	m_frame++;
	auto general = m_pGeneral.lock();

	// 攻撃の位置
	Vector3 colPos = general->m_pos + kAttackOffset * Matrix4x4::GetRotY(general->m_angle);
	// 初撃
	if (m_frame == kAttackFrame)
	{
		//Vector3 colPos = general->m_pos + kAttackOffset * Matrix4x4::GetRotY(general->m_angle);
		m_pAttack = std::make_shared<Attack>();
		m_pAttack->SetData(kAttackData,shared_from_this());
		m_pAttack->Init();
		m_pAttack->SetPos(colPos);
		EffectManager::GetInstance().PlayEffect(L"SwordRush", colPos);
	}

	// 連続斬撃
	if (m_frame > kStartSlashFrame && m_frame < kEndSlashFrame)
	{
		if (m_frame % 10 == 0)
		{
			//Vector3 colPos = general->m_pos + kAttackOffset * Matrix4x4::GetRotY(general->m_angle);
			m_pAttack = std::make_shared<Attack>();
			m_pAttack->SetData(kAttackData,shared_from_this());
			m_pAttack->Init();
			m_pAttack->SetPos(colPos);
		}
	}

	// 攻撃の更新
	if (m_pAttack != nullptr)
	{
		m_pAttack->Update();
	}

	// アニメーションが終わったらステートを終了
	if (general->m_anim.IsEnd())
	{
		ChangeState(std::make_shared<GeneralStateWalk>());
		return;
	}
}

void GeneralStateHeavySlash::Exit()
{

}

void GeneralStateHeavySlash::Draw()
{
#ifdef _DEBUG
	if (m_pAttack != nullptr)
	{
		m_pAttack->Draw();
	}
#endif
}
