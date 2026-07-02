#include "PlayerHPUI.h"
#include "../Character/Player/Player.h"
#include "Singleton/CameraSetter.h"

namespace
{
	constexpr int kWidth = 400;
	constexpr int kHeight = 40;

	constexpr int kEdgeThickness = 5;

	constexpr int kMaxHP = 1000;
}

PlayerHPUI::~PlayerHPUI()
{

}

void PlayerHPUI::Init()
{
	m_RTHandle = MakeScreen(kWidth, kHeight);
}

void PlayerHPUI::Update()
{

}

void PlayerHPUI::Draw()
{
	SetDrawScreen(m_RTHandle);
	// 背景
	DrawBox(0, 0, kWidth, kHeight, 0x333333, true);

	// HPバー本体
	int hp = m_pPlayer.lock()->GetHP();
	float rate = static_cast<float>(hp) / static_cast<float>(kMaxHP);
	int x1, y1, x2, y2;
	x1 = kEdgeThickness / 2;
	y1 = kEdgeThickness / 2;
	x2 = kWidth * rate - kEdgeThickness / 2;
	y2 = kHeight - kEdgeThickness / 2;
	DrawBox(x1, y1, x2, y2, 0x00ff00, true);

	// 枠
	DrawBox(0, 0, kWidth, kHeight, 0x000000, false, kEdgeThickness);
	
	SetDrawScreen(DX_SCREEN_BACK);

	CameraSetter::GetInstance().SetCameraSetting();

	DrawGraph(30, 30, m_RTHandle, false);
}