#include "GeneralStateAppear.h"
#include "General.h"
#include "Singleton/FadeManager.h"
#include "Singleton/EffectManager.h"

#include "GeneralStateWalk.h"

namespace
{
	constexpr const wchar_t* kIdleAnimName = L"General|Idle";
	constexpr const wchar_t* kRoarAnimName = L"General|Power";

	constexpr int kStartAnimFrame = 120;
	constexpr int kStartRoarFrame = kStartAnimFrame + 18 * 2;
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
		general->m_anim.ChangeAnim(kRoarAnimName, 0.3f, false);
	}
	if (m_frame == kStartRoarFrame)
	{
		EffectManager::GetInstance().PlayEffect(L"BossRoar", general->m_pos + Vector3(0, 250, 0));
	}

	if (general->m_anim == kRoarAnimName && general->m_anim.IsEnd())
	{
		ChangeState(std::make_shared<GeneralStateWalk>());
		return;
	}
}

void GeneralStateAppear::Exit()
{
}
