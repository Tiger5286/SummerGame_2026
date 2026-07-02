#include "GeneralStateProjectile.h"
#include "General.h"
#include "../../Player/Player.h"
#include "../../Attack.h"

#include "GeneralStateWalk.h"

namespace
{
	const std::wstring kAnimName = L"General|SlashShot";

	constexpr int kShotFrame = 60;

	constexpr float kShotSpeed = 20.0f;

	const Vector3 kAttackOffset = Vector3(0, 150, -150);
	const MyLib::AttackData kAttackData = {
		.colliderRadius = 150.0f,
		.damage = 20,
		.hitCharacterType = MyLib::CharacterType::Player,
		.isKnockDown = false,
		.isIgnoreInvincible = false
	};
}

void GeneralStateProjectile::Enter(std::weak_ptr<Character> pOwner)
{
	m_pGeneral = std::dynamic_pointer_cast<General>(pOwner.lock());
	auto general = m_pGeneral.lock();
	general->m_anim.ChangeAnim(kAnimName, 0.5f, false);
	// プレイヤーの方向を向く
	Vector3 toPlayer = general->m_pPlayer->GetPos() - general->m_pos;
	general->m_angle = MyLib::GetAngleVec(toPlayer.z, toPlayer.x);
}

void GeneralStateProjectile::Update()
{
	m_frame++;
	auto general = m_pGeneral.lock();

	// 斬撃を飛ばす
	if (m_frame == kShotFrame)
	{
		// 当たり判定の生成
		m_pAtk = std::make_shared<Attack>();
		m_pAtk->SetData(kAttackData,shared_from_this());
		m_pAtk->Init();
		Vector3 colPos = general->m_pos + kAttackOffset * Matrix4x4::GetRotY(general->m_angle);
		m_pAtk->SetPos(colPos);
		Vector3 colVel = Vector3::Back() * kShotSpeed * Matrix4x4::GetRotY(general->m_angle);
		m_pAtk->SetVel(colVel);
	}
	// 当たり判定の更新
	if (m_pAtk != nullptr)
	{
		m_pAtk->Update();
	}

	// アニメーションが終わったらステートを終了
	if (general->m_anim.IsEnd())
	{
		ChangeState(std::make_shared<GeneralStateWalk>());
		return;
	}
}

void GeneralStateProjectile::Exit()
{

}

void GeneralStateProjectile::Draw()
{
#ifdef _DEBUG
	if (m_pAtk != nullptr)
	{
		m_pAtk->Draw();
	}
#endif
}
