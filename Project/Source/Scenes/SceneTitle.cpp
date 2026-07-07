#include "SceneTitle.h"
#include "DxLib.h"
#include "Singleton/Input.h"
#include "SceneManager.h"
#include "Game.h"

#include "SceneMain.h"
#include "SceneOption.h"
#include "Application.h"

SceneTitle::SceneTitle(SceneManager& sceneManager) :
	SceneBase(sceneManager)
{
	m_menuActions[static_cast<int>(Menu::Start)] = {
		.name = L"Start",
		.action = [this]() {Start(); }
	};
	m_menuActions[static_cast<int>(Menu::Option)] = {
		.name = L"Option",
		.action = [this]() {Option(); }
	};
	m_menuActions[static_cast<int>(Menu::Exit)] = {
		.name = L"Exit",
		.action = [this]() {Exit(); }
	};
}

void SceneTitle::Init()
{
	m_titleGraphHandle = LoadGraph(L"data/Graphs/title.png");
}

void SceneTitle::End()
{
	DeleteGraph(m_titleGraphHandle);
}

void SceneTitle::Update()
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

void SceneTitle::Draw()
{
	DrawRotaGraph(Game::kScreenWidth / 2, Game::kScreenHeight / 2 - 100, 0.8, 0.0, m_titleGraphHandle, true);

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
	DrawString(0, 0, L"SceneTitle", 0xffffff);
#endif
}

void SceneTitle::Start()
{
	m_sceneManager.ChangeSceneWithFade(std::make_shared<SceneMain>(m_sceneManager), true);
}

void SceneTitle::Option()
{
	m_sceneManager.PushScene(std::make_shared<SceneOption>(m_sceneManager));
}

void SceneTitle::Exit()
{
	Application::GetInstance().RequestExit();
}