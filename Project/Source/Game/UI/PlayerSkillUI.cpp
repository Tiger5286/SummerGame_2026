#include "PlayerSkillUI.h"
#include "../Character/Player/Player.h"
#include "Game.h"

namespace
{
	constexpr int kSkillCTFrame = 10 * 60;

	constexpr int kOffset = 95;
}

PlayerSkillUI::~PlayerSkillUI()
{
	DeleteGraph(m_handle);
}

void PlayerSkillUI::Init()
{
	m_handle = LoadGraph(L"data/Graphs/SkillUI.png");
}

void PlayerSkillUI::Update()
{
}

void PlayerSkillUI::Draw()
{
	auto player = m_pPlayer.lock();
	float rate = static_cast<float>(player->GetSkillCooltime()) / static_cast<float>(kSkillCTFrame);
	DrawCircleGauge(Game::kScreenWidth - kOffset, Game::kScreenHeight - kOffset, rate * 100, m_handle);
}
