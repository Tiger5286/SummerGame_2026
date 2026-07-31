#include "SceneOption.h"
#include "DxLib.h"
#include "Game.h"
#include "Singleton/Input.h"
#include "SceneManager.h"
#include "Singleton/SoundManager.h"
#include <string_view>

namespace
{
	constexpr int kWindowWidth = Game::kScreenWidth / 10 * 7;
	constexpr int kWindowHeight = Game::kScreenHeight / 10 * 7;

	constexpr float kVolumeControlSpeed = 0.01f;

	enum class MenuList
	{
		SeVolume,
		BgmVolume,
		WindowMode,
		Back,

		Num
	};

	constexpr int kMenuStartY = 270;

	constexpr int kFontSize = 40;
	constexpr std::wstring_view kTitle = L"オプション";

	constexpr int kBackAlpha = 200;
}

SceneOption::SceneOption(SceneManager& sceneManager) :
	SceneBase(sceneManager)
{
	for (int i = 0; i < static_cast<int>(Volumes::Num); i++)
	{
		m_volume[i] = 0.0f;
	}
}

void SceneOption::Init()
{
	m_fontHandle = CreateFontToHandle(Game::kMainFontName, kFontSize, -1);

	m_isFullScreen = !GetWindowModeFlag();
	auto& soundManager = SoundManager::GetInstance();
	m_volume[static_cast<int>(Volumes::SE)] = soundManager.GetSEVolume() / 255.0f;
	m_volume[static_cast<int>(Volumes::BGM)] = soundManager.GetBGMVolume() / 255.0f;

	std::vector<Menu::Funcs> menuActions;
	menuActions.resize(static_cast<int>(MenuList::Num));
	menuActions[static_cast<int>(MenuList::SeVolume)] = {
		.name = L"効果音音量",
		.action = [this]() {SeVolume(); },
		.type = Menu::Type::Bar,
		.pBarRate = &m_volume[static_cast<int>(Volumes::SE)]
	};
	menuActions[static_cast<int>(MenuList::BgmVolume)] = {
		.name = L"BGM音量",
		.action = [this]() {BgmVolume(); },
		.type = Menu::Type::Bar,
		.pBarRate = &m_volume[static_cast<int>(Volumes::BGM)]
	};
	menuActions[static_cast<int>(MenuList::WindowMode)] = {
		.name = L"フルスクリーン",
		.action = [this]() {WindowMode(); },
		.type = Menu::Type::Switch,
		.pBarRate = nullptr,
		.pSwitch = &m_isFullScreen
	};
	menuActions[static_cast<int>(MenuList::Back)] = {
		.name = L"戻る",
		.action = [this]() { m_sceneManager.PopScene(); },
		.type = Menu::Type::Normal,
		.pBarRate = nullptr,
		.pSwitch = nullptr
	};
	m_menu.Init(menuActions, Game::kScreenWidth / 2, kMenuStartY);
}

void SceneOption::End()
{
	DeleteFontToHandle(m_fontHandle);
}

void SceneOption::Update()
{
	auto& input = Input::GetInstance();

	// Bボタンでオプションを閉じる
	if (input.IsTriggerd(XINPUT_BUTTON_B))
	{
		m_sceneManager.PopScene();
		return;
	}

	// メニューの更新
	m_menu.Update();
}

void SceneOption::Draw()
{
	// 背景の描画
	int x1, y1, x2, y2;
	x1 = Game::kScreenWidth / 2 - kWindowWidth / 2;
	y1 = Game::kScreenHeight / 2 - kWindowHeight / 2;
	x2 = Game::kScreenWidth / 2 + kWindowWidth / 2;
	y2 = Game::kScreenHeight / 2 + kWindowHeight / 2;
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, kBackAlpha);
	DrawBox(x1, y1, x2, y2, 0x000000, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	// 枠の描画
	x1 = Game::kScreenWidth / 2 - kWindowWidth / 2;
	y1 = Game::kScreenHeight / 2 - kWindowHeight / 2;
	x2 = Game::kScreenWidth / 2 + kWindowWidth / 2;
	y2 = Game::kScreenHeight / 2 + kWindowHeight / 2;
	DrawBox(x1, y1, x2, y2, 0xffffff, false, 10);

	// タイトルの描画
	int width = GetDrawStringWidthToHandle(kTitle.data(), kTitle.size(), m_fontHandle);
	DrawStringToHandle(Game::kScreenWidth / 2 - width / 2, 150, kTitle.data(), 0xffffff, m_fontHandle);

	// メニューの描画
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
		m_volume[static_cast<int>(Volumes::SE)] += kVolumeControlSpeed;
	}
	if (input.IsPressed(XINPUT_BUTTON_DPAD_LEFT))
	{
		m_volume[static_cast<int>(Volumes::SE)]-= kVolumeControlSpeed;
	}

	if (m_volume[static_cast<int>(Volumes::SE)] > 1.0f)
	{
		m_volume[static_cast<int>(Volumes::SE)] = 1.0f;
	}
	if (m_volume[static_cast<int>(Volumes::SE)] < 0)
	{
		m_volume[static_cast<int>(Volumes::SE)] = 0;
	}

	SoundManager::GetInstance().ChangeVolume(SoundType::SE, m_volume[static_cast<int>(Volumes::SE)] * 255);
}

void SceneOption::BgmVolume()
{
	auto& input = Input::GetInstance();
	if (input.IsPressed(XINPUT_BUTTON_DPAD_RIGHT))
	{
		m_volume[static_cast<int>(Volumes::BGM)]+= kVolumeControlSpeed;
	}
	if (input.IsPressed(XINPUT_BUTTON_DPAD_LEFT))
	{
		m_volume[static_cast<int>(Volumes::BGM)]-= kVolumeControlSpeed;
	}

	if (m_volume[static_cast<int>(Volumes::BGM)] > 1.0f)
	{
		m_volume[static_cast<int>(Volumes::BGM)] = 1.0f;
	}
	if (m_volume[static_cast<int>(Volumes::BGM)] < 0)
	{
		m_volume[static_cast<int>(Volumes::BGM)] = 0;
	}

	SoundManager::GetInstance().ChangeVolume(SoundType::BGM, m_volume[static_cast<int>(Volumes::BGM)] * 255);
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
