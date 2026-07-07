#include "SceneGameOver.h"
#include "DxLib.h"
#include "Singleton/Input.h"
#include "SceneManager.h"
#include "Game.h"

#include "SceneTitle.h"
#include "SceneMain.h"

SceneGameOver::SceneGameOver(SceneManager& sceneManager) :
	SceneBase(sceneManager)
{
	m_menuActions[static_cast<int>(Menu::Retry)] = {
		.name = L"Retry",
		.action = [this]() {Retry(); }
	};
	m_menuActions[static_cast<int>(Menu::Title)] = {
		.name = L"Title",
		.action = [this]() {Title(); }
	};
}

void SceneGameOver::Init()
{
}

void SceneGameOver::End()
{
}

void SceneGameOver::Update()
{
	auto& input = Input::GetInstance();
	// 上下キーでメニューの選択を変更する
	if (input.IsTriggerd(XINPUT_BUTTON_DPAD_DOWN))
	{
		m_selectIndex++;
		if (m_selectIndex >= static_cast<int>(Menu::Num))
		{
			m_selectIndex = 0;
		}
	}
	if (input.IsTriggerd(XINPUT_BUTTON_DPAD_UP))
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
	int x = Game::kScreenWidth / 2;
	int y = Game::kScreenHeight / 2 + 250;
	for (int i = 0; i < static_cast<int>(Menu::Num); i++)
	{
		unsigned int color = 0xffffff;
		if (i == m_selectIndex) color = 0xff0000;
		int strWidth = GetDrawStringWidth(m_menuActions[i].name.c_str(), m_menuActions[i].name.size());
		DrawString(x - strWidth / 2, y + i * 20, m_menuActions[i].name.c_str(), color);
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
