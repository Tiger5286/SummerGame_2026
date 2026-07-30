#include "SceneOption.h"
#include "DxLib.h"
#include "Game.h"
#include "Singleton/Input.h"
#include "SceneManager.h"
#include "Singleton/SoundManager.h"

namespace
{
	constexpr int kWindowWidth = Game::kScreenWidth / 10 * 7;
	constexpr int kWindowHeight = Game::kScreenHeight / 10 * 7;

	constexpr float kVolumeControlSpeed = 0.005f;

	enum class MenuList
	{
		SeVolume,
		BgmVolume,
		WindowMode,

		Num
	};
}

SceneOption::SceneOption(SceneManager& sceneManager) :
	SceneBase(sceneManager)
{
	for (int i = 0; i < 2; i++)
	{
		m_volume[i] = 0.0f;
	}
}

void SceneOption::Init()
{
	m_isFullScreen = !GetWindowModeFlag();
	auto& soundManager = SoundManager::GetInstance();
	m_volume[0] = soundManager.GetSEVolume() / 255.0f;
	m_volume[1] = soundManager.GetBGMVolume() / 255.0f;

	std::vector<Menu::Funcs> menuActions;
	menuActions.resize(static_cast<int>(MenuList::Num));
	menuActions[static_cast<int>(MenuList::SeVolume)] = {
		.name = L"効果音音量",
		.action = [this]() {SeVolume(); },
		.type = Menu::Type::Bar,
		.pBarRate = &m_volume[0]
	};
	menuActions[static_cast<int>(MenuList::BgmVolume)] = {
		.name = L"BGM音量",
		.action = [this]() {BgmVolume(); },
		.type = Menu::Type::Bar,
		.pBarRate = &m_volume[1]
	};
	menuActions[static_cast<int>(MenuList::WindowMode)] = {
		.name = L"フルスクリーン",
		.action = [this]() {WindowMode(); },
		.type = Menu::Type::Switch,
		.pBarRate = nullptr,
		.pSwitch = &m_isFullScreen
	};
	m_menu.Init(menuActions, 1280 / 2, 720 / 2, 1.0f);
}

void SceneOption::End()
{
}

void SceneOption::Update()
{
	auto& input = Input::GetInstance();

	m_menu.Update();

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

	//for (int i = 0; i < static_cast<int>(Menu::Num); i++)
	//{
	//	unsigned int color = 0xffffff;
	//	if (i == m_selectIndex) color = 0xff0000;
	//	DrawString(x1 + 50, y1 + 50 + i * 20, m_menuActions[i].name.c_str(), color);
	//	
	//	if (i == static_cast<int>(Menu::SeVolume) || i == static_cast<int>(Menu::BgmVolume))
	//	{
	//		int stringWidth = GetDrawStringWidth(m_menuActions[i].name.c_str(), m_menuActions[i].name.size());
	//		int x = x1 + 50 + stringWidth + 15;
	//		int y = y1 + 50 + i * 20 + 8;
	//		int barLen = 100;
	//		DrawLine(x, y, x + barLen, y, 0xffffff, 2);
	//		float rate = m_volume[i] / 255.0f;
	//		DrawCircle(x + rate * barLen, y, 8, 0xff0000, true);
	//	}
	//	else if (i == static_cast<int>(Menu::WindowMode))
	//	{
	//		int stringWidth = GetDrawStringWidth(m_menuActions[i].name.c_str(), m_menuActions[i].name.size());
	//		int x = x1 + 50 + stringWidth + 15;
	//		int y = y1 + 50 + i * 20 + 8;
	//		unsigned int color = 0xffffff;
	//		if (m_windowMode) color = 0xff0000;
	//		DrawCircle(x, y, 8, color, true);
	//	}
	//}

	// 枠の描画
	x1 = Game::kScreenWidth / 2 - kWindowWidth / 2;
	y1 = Game::kScreenHeight / 2 - kWindowHeight / 2;
	x2 = Game::kScreenWidth / 2 + kWindowWidth / 2;
	y2 = Game::kScreenHeight / 2 + kWindowHeight / 2;
	DrawBox(x1, y1, x2, y2, 0xffffff, false, 10);

	m_menu.Draw();

#ifdef _DEBUG
	DrawString(x1 + 10, y1 + 10, L"SceneOption", 0xffffff);
#endif
}

void SceneOption::SeVolume()
{
	auto& input = Input::GetInstance();
	if (input.IsPressed(XINPUT_BUTTON_DPAD_RIGHT))
	{
		m_volume[0]+= kVolumeControlSpeed;
	}
	if (input.IsPressed(XINPUT_BUTTON_DPAD_LEFT))
	{
		m_volume[0]-= kVolumeControlSpeed;
	}

	if (m_volume[0] > 1.0f)
	{
		m_volume[0] = 1.0f;
	}
	if (m_volume[0] < 0)
	{
		m_volume[0] = 0;
	}

	SoundManager::GetInstance().ChangeVolume(SoundType::SE, m_volume[0] * 255);
}

void SceneOption::BgmVolume()
{
	auto& input = Input::GetInstance();
	if (input.IsPressed(XINPUT_BUTTON_DPAD_RIGHT))
	{
		m_volume[1]+= kVolumeControlSpeed;
	}
	if (input.IsPressed(XINPUT_BUTTON_DPAD_LEFT))
	{
		m_volume[1]-= kVolumeControlSpeed;
	}

	if (m_volume[1] > 1.0f)
	{
		m_volume[1] = 1.0f;
	}
	if (m_volume[1] < 0)
	{
		m_volume[1] = 0;
	}

	SoundManager::GetInstance().ChangeVolume(SoundType::BGM, m_volume[1] * 255);
}

void SceneOption::WindowMode()
{
	auto& input = Input::GetInstance();
	if (input.IsTriggerd(XINPUT_BUTTON_A))
	{
		m_isFullScreen = !m_isFullScreen;
		ChangeWindowMode(!m_isFullScreen);
	}
}
