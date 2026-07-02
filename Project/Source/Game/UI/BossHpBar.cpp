#include "BossHpBar.h"
#include "../Character/Enemy/EnemyBase.h"
#include "Singleton/CameraSetter.h"
#include "Game.h"

namespace
{
	constexpr int kWidth = 600;
	constexpr int kHeight = 40;

	constexpr int kThickness = 5;

	constexpr int kOffsetX = 30;
	constexpr int kOffsetY = 30;
}

BossHpBar::BossHpBar() :
	UIBase(2)
{
}

BossHpBar::~BossHpBar()
{
}

void BossHpBar::Init()
{
	m_RTHandle = MakeScreen(kWidth, kHeight);
}

void BossHpBar::Update()
{
	// 持ち主が死んでいたら自分も死ぬ
	if (m_pOwner.lock()->GetHP() <= 0)
	{
		m_isAlive = false;
	}
}

void BossHpBar::Draw()
{
	SetDrawScreen(m_RTHandle);
	// 背景
	DrawBox(0, 0, kWidth, kHeight, 0x333333, true);

	// HPバー本体
	int hp = m_pOwner.lock()->GetHP();
	int maxHp = m_pOwner.lock()->GetMaxHP();
	float rate = static_cast<float>(hp) / static_cast<float>(maxHp);
	int x1, y1, x2, y2;
	x1 = kThickness / 2;
	y1 = kThickness / 2;
	x2 = (kWidth - kThickness / 2) * rate;
	y2 = kHeight - kThickness / 2;
	DrawBox(x1, y1, x2, y2, 0xff0000, true, kThickness);

	// 枠
	DrawBox(0, 0, kWidth, kHeight, 0x000000, false, kThickness);

	SetDrawScreen(DX_SCREEN_BACK);

	CameraSetter::GetInstance().SetCameraSetting();

	DrawGraph(Game::kScreenWidth - kWidth - kOffsetX, kOffsetY, m_RTHandle, false);
}
