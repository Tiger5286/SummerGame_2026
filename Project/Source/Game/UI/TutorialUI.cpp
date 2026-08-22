#include "TutorialUI.h"
#include "Game.h"
#include <string_view>

namespace
{
	constexpr int kLayer = 2;

	constexpr int kWindowMargin = 20;
	constexpr int kWindowWidth = 500;
	constexpr int kWindowHeight = 100;
	constexpr int kPosX = kWindowWidth / 2 + kWindowMargin;
	constexpr int kPosY = Game::kScreenHeight - kWindowHeight / 2 - kWindowMargin;

	constexpr int kTutorialStepNum = static_cast<int>(TutorialManager::TutorialStep::Num);
	constexpr std::wstring_view kTutorialTexts[kTutorialStepNum] = {
	L"　　で移動",
	L"　　で攻撃",
	L"　　でフレイムシフト　敵に接近できる",
	L"　　でジャンプ",
	L"　　でウィングスピン",
	L"　　で回避",
	L"溜まったら　　　で必殺技",
	L"チュートリアル終了"
	};

	constexpr int kFontSize = 30;

	constexpr float kIconScaleSpeed = 0.1f;
	constexpr float kMaxIconScaleAngle = DX_PI_F * 2 * 2;
	constexpr float kIconScaleAmplitude = 0.5f;

	enum class Buttons
	{
		LStick,
		X,
		B,
		A,
		Y,
		RB,
		LB,

		Num
	};

	constexpr const wchar_t* kButtonFilePaths[static_cast<int>(Buttons::Num)] = {
		L"data/Graphs/Buttons/LStick.png",
		L"data/Graphs/Buttons/X.png",
		L"data/Graphs/Buttons/B.png",
		L"data/Graphs/Buttons/A.png",
		L"data/Graphs/Buttons/Y.png",
		L"data/Graphs/Buttons/RB.png",
		L"data/Graphs/Buttons/LB.png"
	};

	static_assert(std::size(kButtonFilePaths) == static_cast<int>(Buttons::Num));

	constexpr int kButtonPosOffsetX[static_cast<int>(Buttons::Num)] = {
		-50,-50,-220,-70,-110,-50,+5,
	};
	constexpr float kButtonScale = 0.5f;
}

TutorialUI::TutorialUI():
	UIBase(kLayer)
{
}

TutorialUI::~TutorialUI()
{
	DeleteGraph(m_iconHandle);
	DeleteFontToHandle(m_fontHandle);
	for (auto& handle : m_buttonHandles)
	{
		DeleteGraph(handle);
	}
}

void TutorialUI::Init()
{
	m_iconHandle = LoadGraph(L"data/Graphs/TutorialIcon.png");
	m_fontHandle = CreateFontToHandle(Game::kMainFontName, kFontSize, -1);
	m_buttonHandles.resize(static_cast<int>(Buttons::Num));
	for (int i = 0; i < static_cast<int>(Buttons::Num); i++)
	{
		m_buttonHandles[i] = LoadGraph(kButtonFilePaths[i]);
	}
}

void TutorialUI::Update()
{
	// ボタンアイコンを拡縮させる
	if (m_iconScaleAngle < kMaxIconScaleAngle)
	{
		m_iconScaleAngle += kIconScaleSpeed;
	}
	else
	{
		m_iconScaleAngle = kMaxIconScaleAngle;
	}
}

void TutorialUI::Draw()
{
	// チュートリアルが終わっていたら描画しない
	if (m_currentStep == TutorialManager::TutorialStep::End) return;

	// 背景を描画
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 128);
	int x1, y1, x2, y2;
	x1 = kPosX - kWindowWidth / 2;
	y1 = kPosY - kWindowHeight / 2;
	x2 = kPosX + kWindowWidth / 2;
	y2 = kPosY + kWindowHeight / 2;
	DrawBox(x1, y1, x2, y2, 0x000000, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	// 枠を描画
	DrawBox(x1, y1, x2, y2, 0xffffff, false, 3);

	// アイコンを描画
	DrawRotaGraph(x2, y1, 1.0 + abs(sinf(m_iconScaleAngle)) * kIconScaleAmplitude, 0.0, m_iconHandle, true);

	// テキストを描画
	auto text = kTutorialTexts[static_cast<int>(m_currentStep)];
	int strW = GetDrawStringWidthToHandle(text.data(), text.size(),m_fontHandle);
	DrawStringToHandle(kPosX - strW / 2, kPosY - kFontSize / 2, text.data(), 0xffffff, m_fontHandle);

	// ボタンアイコンを描画
	DrawRotaGraph(kPosX + kButtonPosOffsetX[static_cast<int>(m_currentStep)], kPosY, kButtonScale, 0.0, m_buttonHandles[static_cast<int>(m_currentStep)], true);
}
