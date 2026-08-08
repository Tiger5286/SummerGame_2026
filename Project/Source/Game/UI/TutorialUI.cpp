#include "TutorialUI.h"
#include "Game.h"
#include <string_view>

namespace
{
	constexpr int kLayer = 2;

	constexpr int kWindowMargin = 20;
	constexpr int kWindowWidth = 500;
	constexpr int kWindowHeight = 100;
	constexpr int kPosX = Game::kScreenWidth - kWindowWidth / 2 - kWindowMargin;
	constexpr int kPosY = kWindowHeight / 2 + kWindowMargin;

	constexpr int kTutorialStepNum = static_cast<int>(TutorialManager::TutorialStep::Num);
	constexpr std::wstring_view kTutorialTexts[kTutorialStepNum] = {
	L"Lスティックで移動",
	L"Xボタンで攻撃",
	L"Rスティック押し込みでロックオン　Bでフレイムシフト",
	L"Aボタンでジャンプ",
	L"Yボタンでウィングスピン",
	L"RBボタンで回避",
	L"溜まったらLBボタンで必殺技",
	L"チュートリアル終了"
	};
}

TutorialUI::TutorialUI():
	UIBase(kLayer)
{
}

TutorialUI::~TutorialUI()
{
}

void TutorialUI::Init()
{
}

void TutorialUI::Update()
{
}

void TutorialUI::Draw()
{
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 128);
	int x1, y1, x2, y2;
	x1 = kPosX - kWindowWidth / 2;
	y1 = kPosY - kWindowHeight / 2;
	x2 = kPosX + kWindowWidth / 2;
	y2 = kPosY + kWindowHeight / 2;
	DrawBox(x1, y1, x2, y2, 0x000000, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	auto text = kTutorialTexts[static_cast<int>(m_currentStep)];
	int strW = GetDrawStringWidth(text.data(), text.size());
	DrawString(kPosX - strW / 2, kPosY - 8, text.data(), 0xffffff);
}
