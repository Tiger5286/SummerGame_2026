#include "VultureStateHit.h"
#include "Vulture.h"

#include "VultureStateIdle.h"

namespace
{
	const std::wstring kAnimName = L"VultureCinereous_Skelmesh|VultureCinereous_TakeOff";
}

void VultureStateHit::Enter(std::weak_ptr<Character> pOwner)
{
	m_pVulture = std::dynamic_pointer_cast<Vulture>(pOwner.lock());
	auto vulture = m_pVulture.lock();
	vulture->m_anim.ChangeAnim(kAnimName, 1.0f, false);
}

void VultureStateHit::Update()
{
	auto vulture = m_pVulture.lock();
	// 被弾アニメーションが終わったらIdleへ
	if (vulture->m_anim.IsEnd())
	{
		ChangeState(std::make_shared<VultureStateIdle>());
		return;
	}
}

void VultureStateHit::Exit()
{}
