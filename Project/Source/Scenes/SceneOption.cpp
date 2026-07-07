#include "SceneOption.h"
#include "DxLib.h"
#include "Game.h"
#include "Singleton/Input.h"
#include "SceneManager.h"

namespace
{
	constexpr int kWindowWidth = Game::kScreenWidth / 10 * 7;
	constexpr int kWindowHeight = Game::kScreenHeight / 10 * 7;
}

SceneOption::SceneOption(SceneManager& sceneManager) :
	SceneBase(sceneManager)
{
	m_menuActions[static_cast<int>(OptionMenu::SeVolume)] = {
		.name = L"SeVolume",
		.action = [this]() {SeVolume(); }
	};
	m_menuActions[static_cast<int>(OptionMenu::BgmVolume)] = {
		.name = L"BgmVolume",
		.action = [this]() {BgmVolume(); }
	};
	m_menuActions[static_cast<int>(OptionMenu::WindowMode)] = {
		.name = L"WindowMode",
		.action = [this]() {WindowMode(); }
	};

	for (int i = 0; i < 2; i++)
	{
		m_volume[i] = 0.5f;
	}
}

void SceneOption::Init()
{
	m_windowMode = GetWindowModeFlag();
}

void SceneOption::End()
{
}

void SceneOption::Update()
{
	auto& input = Input::GetInstance();

	// 上下キーでメニューの選択を変更する
	if (input.IsTriggerd(XINPUT_BUTTON_DPAD_DOWN))
	{
		m_selectIndex++;
		if (m_selectIndex >= static_cast<int>(OptionMenu::Num))
		{
			m_selectIndex = 0;
		}
	}
	if (input.IsTriggerd(XINPUT_BUTTON_DPAD_UP))
	{
		m_selectIndex--;
		if (m_selectIndex < 0)
		{
			m_selectIndex = static_cast<int>(OptionMenu::Num) - 1;
		}
	}
	m_menuActions[m_selectIndex].action();
	// Bボタンでオプションを閉じる
	if (input.IsTriggerd(XINPUT_BUTTON_B))
	{
		m_sceneManager.PopScene();
		return;
	}
}

void SceneOption::Draw()
{
	// 背景の描画
	int x1, y1, x2, y2;
	x1 = Game::kScreenWidth / 2 - kWindowWidth / 2;
	y1 = Game::kScreenHeight / 2 - kWindowHeight / 2;
	x2 = Game::kScreenWidth / 2 + kWindowWidth / 2;
	y2 = Game::kScreenHeight / 2 + kWindowHeight / 2;
	DrawBox(x1, y1, x2, y2, 0x000000, true);

	for (int i = 0; i < static_cast<int>(OptionMenu::Num); i++)
	{
		unsigned int color = 0xffffff;
		if (i == m_selectIndex) color = 0xff0000;
		DrawString(x1 + 50, y1 + 50 + i * 20, m_menuActions[i].name.c_str(), color);
		
		if (i == static_cast<int>(OptionMenu::SeVolume) || i == static_cast<int>(OptionMenu::BgmVolume))
		{
			int stringWidth = GetDrawStringWidth(m_menuActions[i].name.c_str(), m_menuActions[i].name.size());
			int x = x1 + 50 + stringWidth + 15;
			int y = y1 + 50 + i * 20 + 8;
			int barLen = 100;
			DrawLine(x, y, x + barLen, y, 0xffffff, 2);
			DrawCircle(x + m_volume[i] * barLen, y, 8, 0xff0000, true);
		}
		else if (i == static_cast<int>(OptionMenu::WindowMode))
		{
			int stringWidth = GetDrawStringWidth(m_menuActions[i].name.c_str(), m_menuActions[i].name.size());
			int x = x1 + 50 + stringWidth + 15;
			int y = y1 + 50 + i * 20 + 8;
			unsigned int color = 0xffffff;
			if (m_windowMode) color = 0xff0000;
			DrawCircle(x, y, 8, color, true);
		}
	}

	// 枠の描画
	x1 = Game::kScreenWidth / 2 - kWindowWidth / 2;
	y1 = Game::kScreenHeight / 2 - kWindowHeight / 2;
	x2 = Game::kScreenWidth / 2 + kWindowWidth / 2;
	y2 = Game::kScreenHeight / 2 + kWindowHeight / 2;
	DrawBox(x1, y1, x2, y2, 0xffffff, false, 10);

#ifdef _DEBUG
	DrawString(x1 + 10, y1 + 10, L"SceneOption", 0xffffff);
#endif
}

void SceneOption::SeVolume()
{
	auto& input = Input::GetInstance();
	if (input.IsPressed(XINPUT_BUTTON_DPAD_RIGHT))
	{
		m_volume[0] += 0.02f;
	}
	if (input.IsPressed(XINPUT_BUTTON_DPAD_LEFT))
	{
		m_volume[0] -= 0.02f;
	}

	if (m_volume[0] > 1.0f)
	{
		m_volume[0] = 1.0f;
	}
	if (m_volume[0] < 0.0f)
	{
		m_volume[0] = 0.0f;
	}
}

void SceneOption::BgmVolume()
{
	auto& input = Input::GetInstance();
	if (input.IsPressed(XINPUT_BUTTON_DPAD_RIGHT))
	{
		m_volume[1] += 0.02f;
	}
	if (input.IsPressed(XINPUT_BUTTON_DPAD_LEFT))
	{
		m_volume[1] -= 0.02f;
	}

	if (m_volume[1] > 1.0f)
	{
		m_volume[1] = 1.0f;
	}
	if (m_volume[1] < 0.0f)
	{
		m_volume[1] = 0.0f;
	}
}

void SceneOption::WindowMode()
{
	auto& input = Input::GetInstance();
	if (input.IsTriggerd(XINPUT_BUTTON_A))
	{
		m_windowMode = !m_windowMode;
		ChangeWindowMode(m_windowMode);
	}
}
