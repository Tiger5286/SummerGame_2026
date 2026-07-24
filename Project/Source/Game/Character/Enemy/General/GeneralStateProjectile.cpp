#include "GeneralStateProjectile.h"
#include "General.h"
#include "../../Player/Player.h"
#include "../../Attack.h"
#include "Singleton/EffectManager.h"

#include "GeneralStateWalk.h"

namespace
{
	const std::wstring kAnimName = L"General|SlashShot";

	constexpr int kShotFrame = 60;

	constexpr float kShotSpeed = 20.0f;

	const Vector3 kAttackOffset = Vector3(0, 150, -150);
	const MyLib::AttackData kAttackData = {
		.colliderRadius = 150.0f,
		.damage = 100,
		.hitCharacterType = MyLib::CharacterType::Player,
		.isKnockDown = false,
		.isIgnoreInvincible = false
	};

	constexpr int kPlayEffectFrame = 50;
	const Vector3 kEffectPosOffset = Vector3(0, 0, -200);
	constexpr float kEffectAngleOffset = DX_PI_F / 2;
}

void GeneralStateProjectile::OnEnter()
{
	m_pGeneral = std::dynamic_pointer_cast<General>(m_pOwner.lock());
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
	auto& effectManager = EffectManager::GetInstance();

	// エフェクトを再生
	if (m_frame == kPlayEffectFrame)
	{
		// 斬撃エフェクトを生成
		Vector3 effectPosOffset = kEffectPosOffset * Matrix4x4::GetRotY(general->m_angle);
		auto handle = effectManager.PlayEffect(L"BossProjectileSlash", general->m_pos + effectPosOffset);
		SetRotationPlayingEffekseer3DEffect(handle, 0.0f, general->m_angle + kEffectAngleOffset, 0.0f);
	}

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

		// 飛ぶ斬撃エフェクトを生成
		m_projectileEffectHandle = effectManager.PlayEffect(L"BossProjectile", colPos);
		SetRotationPlayingEffekseer3DEffect(m_projectileEffectHandle, 0.0f, general->m_angle + kEffectAngleOffset, 0.0f);
	}
	// 当たり判定の更新
	if (m_pAtk != nullptr)
	{
		m_pAtk->Update();
		// 飛ぶ斬撃エフェクトの位置を設定
		Vector3 colPos = m_pAtk->GetPos();
		SetPosPlayingEffekseer3DEffect(m_projectileEffectHandle, colPos.x, colPos.y, colPos.z);
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
	// 飛ぶ斬撃エフェクトを止める
	EffectManager::GetInstance().StopEffect(m_projectileEffectHandle);
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
