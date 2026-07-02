#include "PlayerSpecialUI.h"
#include "../Character/Player/Player.h"
#include "Singleton/CameraSetter.h"

namespace
{
	constexpr int kWidth = 200;
	constexpr int kHeight = 20;

	constexpr int kThickness = 2;

	constexpr int kMaxSpecial = 1000;
}

PlayerSpecialUI::PlayerSpecialUI()
{
}

PlayerSpecialUI::~PlayerSpecialUI()
{
}

void PlayerSpecialUI::Init()
{
	m_RTHandle = MakeScreen(kWidth, kHeight);
}

void PlayerSpecialUI::Update()
{
}

void PlayerSpecialUI::Draw()
{
	SetDrawScreen(m_RTHandle);
	// 背景
	DrawBox(0, 0, kWidth, kHeight, 0x333333, true);

	// HPバー本体
	int special = m_pPlayer.lock()->GetSpecialCharge();
	float rate = static_cast<float>(special) / static_cast<float>(kMaxSpecial);
	int x1, y1, x2, y2;
	x1 = kThickness / 2;
	y1 = kThickness / 2;
	x2 = kWidth * rate - kThickness / 2;
	y2 = kHeight - kThickness / 2;
	DrawBox(x1, y1, x2, y2, 0xffff00, true);

	// 枠
	DrawBox(0, 0, kWidth, kHeight, 0x000000, false, kThickness);

	SetDrawScreen(DX_SCREEN_BACK);

	CameraSetter::GetInstance().SetCameraSetting();

	DrawGraph(30, 70, m_RTHandle, false);
}
