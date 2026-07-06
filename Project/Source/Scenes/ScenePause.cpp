#include "ScenePause.h"
#include "DxLib.h"
#include "Game.h"
#include "Singleton/Input.h"
#include "SceneManager.h"
#include <map>

#include "SceneTitle.h"

ScenePause::ScenePause(SceneManager& sceneManager) :
	SceneBase(sceneManager)
{
	// メニューのアクションを設定する
	m_menuActions[static_cast<int>(PauseMenu::Resume)] = {
		.name = L"Resume",
		.action = [this]() { Resume(); }
	};
	m_menuActions[static_cast<int>(PauseMenu::Option)] = {
		.name = L"Option",
		.action = [this]() { Option(); }
	};
	m_menuActions[static_cast<int>(PauseMenu::BackToTitle)] = {
		.name = L"Back to Title",
		.action = [this]() { BackToTitle(); }
	};
}

void ScenePause::Init()
{

}

void ScenePause::End()
{

}

void ScenePause::Update()
{
	auto& input = Input::GetInstance();

	// 上下キーでメニューの選択を変更する
	if (input.IsTriggerd(XINPUT_BUTTON_DPAD_DOWN))
	{
		m_selectIndex++;
		if (m_selectIndex >= static_cast<int>(PauseMenu::Num))
		{
			m_selectIndex = 0;
		}
	}
	if (input.IsTriggerd(XINPUT_BUTTON_DPAD_UP))
	{
		m_selectIndex--;
		if (m_selectIndex < 0)
		{
			m_selectIndex = static_cast<int>(PauseMenu::Num) - 1;
		}
	}
	// 決定ボタンで選択中のメニューのアクションを実行する
	if (input.IsTriggerd(XINPUT_BUTTON_A))
	{
		m_menuActions[m_selectIndex].action();
		return;
	}
	// スタートボタンでポーズを解除する
	if (input.IsTriggerd(XINPUT_BUTTON_START))
	{
		Resume();
		return;
	}
}

void ScenePause::Draw()
{
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 128);
	DrawBox(0, 0, Game::kScreenWidth, Game::kScreenHeight, 0x000000, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	for (int i = 0; i < static_cast<int>(PauseMenu::Num); i++)
	{
		unsigned int color = 0xffffff;
		if (i == m_selectIndex) color = 0xff0000;
		DrawString(100, 100 + i * 20, m_menuActions[i].name.c_str(), color);
	}

#ifdef _DEBUG
	DrawString(0, 0, L"ScenePause", 0xffffff);
#endif
}

void ScenePause::Resume()
{
	m_sceneManager.PopScene();
}

void ScenePause::Option()
{
	
}

void ScenePause::BackToTitle()
{
	m_sceneManager.ResetSceneWithFade(std::make_shared<SceneTitle>(m_sceneManager));
}
