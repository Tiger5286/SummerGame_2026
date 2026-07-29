#include "SceneGameOver.h"
#include "DxLib.h"
#include "Singleton/Input.h"
#include "SceneManager.h"
#include "Game.h"

#include "SceneTitle.h"
#include "SceneMain.h"

namespace
{
	constexpr float kBackScale = 1.3f;

	constexpr int kLogoOffsetY = -100;

	constexpr int kFontSize = 30;
	constexpr int kStringMargin = 15;
}

SceneGameOver::SceneGameOver(SceneManager& sceneManager) :
	SceneBase(sceneManager)
{
	m_menuActions[static_cast<int>(Menu::Retry)] = {
		.name = L"もういちど挑戦",
		.action = [this]() {Retry(); }
	};
	m_menuActions[static_cast<int>(Menu::Title)] = {
		.name = L"タイトルに戻る",
		.action = [this]() {Title(); }
	};
}

void SceneGameOver::Init()
{
	m_backHandle = LoadGraph(L"data/Graphs/StoneBrick.png");
	m_logoHandle = LoadGraph(L"data/Graphs/GameOverLogo.png");
	m_fontHandle = CreateFontToHandle(Game::kMainFontName, kFontSize, -1);
}

void SceneGameOver::End()
{
	DeleteGraph(m_backHandle);
	DeleteGraph(m_logoHandle);
	DeleteFontToHandle(m_fontHandle);
}

void SceneGameOver::Update()
{
	auto& input = Input::GetInstance();
	// 上下キーでメニューの選択を変更する
	if (input.IsTriggerd(XINPUT_BUTTON_DPAD_DOWN, true))
	{
		m_selectIndex++;
		if (m_selectIndex >= static_cast<int>(Menu::Num))
		{
			m_selectIndex = 0;
		}
	}
	if (input.IsTriggerd(XINPUT_BUTTON_DPAD_UP, true))
	{
		m_selectIndex--;
		if (m_selectIndex < 0)
		{
			m_selectIndex = static_cast<int>(Menu::Num) - 1;
		}
	}
	// 決定ボタンで選択中のメニューのアクションを実行する
	if (input.IsTriggerd(XINPUT_BUTTON_A))
	{
		m_menuActions[m_selectIndex].action();
		return;
	}
}

void SceneGameOver::Draw()
{
	// 背景
	DrawRotaGraph(Game::kScreenWidth / 2, Game::kScreenHeight / 2, kBackScale, 0.0, m_backHandle, false);
	// ロゴ
	DrawRotaGraph(Game::kScreenWidth / 2, Game::kScreenHeight / 2 + kLogoOffsetY, 1.0, 0.0, m_logoHandle, true);

	int x = Game::kScreenWidth / 2;
	int y = Game::kScreenHeight / 2 + 250;
	for (int i = 0; i < static_cast<int>(Menu::Num); i++)
	{
		unsigned int color = 0xffffff;
		if (i == m_selectIndex) color = 0xff0000;
		int strWidth = GetDrawStringWidthToHandle(m_menuActions[i].name.c_str(), m_menuActions[i].name.size(), m_fontHandle);
		DrawStringToHandle(x - strWidth / 2, y + i * (kFontSize + kStringMargin), m_menuActions[i].name.c_str(), color, m_fontHandle);
	}

#ifdef _DEBUG
	DrawString(0, 0, L"SceneGameOver", 0xffffff);
#endif
}

void SceneGameOver::Retry()
{
	m_sceneManager.ChangeSceneWithFade(std::make_shared<SceneMain>(m_sceneManager), true);
}

void SceneGameOver::Title()
{
	m_sceneManager.ChangeSceneWithFade(std::make_shared<SceneTitle>(m_sceneManager), false);
}
