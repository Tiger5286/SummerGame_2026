#include "GeneralStateAppear.h"
#include "General.h"
#include "Singleton/FadeManager.h"
#include "Singleton/EffectManager.h"
#include "Singleton/SoundManager.h"

#include "GeneralStateWalk.h"

namespace
{
	// アニメーション名
	constexpr const wchar_t* kIdleAnimName = L"General|Idle";
	constexpr const wchar_t* kRoarAnimName = L"General|Power";

	// 叫びアニメーションを開始するフレーム数
	constexpr int kStartAnimFrame = 120;
	// 叫びアニメーションを始めてから実際に叫んでる部分までのフレーム数
	constexpr int kStartRoarFrame = kStartAnimFrame + 18 * 2;

	const Vector3 kEffectOffset = Vector3(0, 250, 0);

	constexpr float kAngleOffset = DX_PI_F / 5;

	constexpr float kAnimSpeed = 0.3f;
}

void GeneralStateAppear::OnEnter()
{
	m_pGeneral = std::dynamic_pointer_cast<General>(m_pOwner.lock());
	auto general = m_pGeneral.lock();
	general->m_anim.ChangeAnim(kIdleAnimName);
}

void GeneralStateAppear::Update()
{
	auto general = m_pGeneral.lock();

	m_frame++;

	if (m_frame == kStartAnimFrame)
	{
		general->m_anim.ChangeAnim(kRoarAnimName, kAnimSpeed, false);
	}
	if (m_frame == kStartRoarFrame)
	{
		EffectManager::GetInstance().PlayEffect(L"BossRoar", general->m_pos + kEffectOffset);
		SoundManager::GetInstance().PlaySoundGame(L"GeneralAppear");
	}

	if (general->m_anim == kRoarAnimName)
	{
		// 叫びアニメーションで横を向いてしまうため無理やり補正
		general->m_drawAngle = general->m_angle - kAngleOffset;

		// アニメーションが終わったらボス戦の行動に移行
		if (general->m_anim.IsEnd())
		{
			ChangeState(std::make_shared<GeneralStateWalk>());
			return;
		}
	}
}

void GeneralStateAppear::Exit()
{
}
