#include "ScenePause.h"
#include "DxLib.h"
#include "Game.h"
#include "Singleton/Input.h"
#include "SceneManager.h"
#include <map>
#include <string_view>

#include "SceneTitle.h"
#include "SceneOption.h"

namespace
{
	constexpr std::wstring_view kTitleString = L"ポーズ";

	constexpr int kTitleFontSize = 70;

	constexpr int kMenuStartY = 450;
	constexpr int kMenuFontSize = 30;
	constexpr int kMenuMargin = 50;
	constexpr unsigned int kMenuTextColor = 0xffffff;

	constexpr float kMaxMenuScale = 1.1f;
	constexpr float kLerpT = 0.2f;
}

ScenePause::ScenePause(SceneManager& sceneManager) :
	SceneBase(sceneManager)
{
	// メニューのアクションを設定する
	m_menuActions[static_cast<int>(Menu::Resume)] = {
		.name = L"再開",
		.action = [this]() { Resume(); }
	};
	m_menuActions[static_cast<int>(Menu::Option)] = {
		.name = L"オプション",
		.action = [this]() { Option(); }
	};
	m_menuActions[static_cast<int>(Menu::BackToTitle)] = {
		.name = L"タイトルに戻る",
		.action = [this]() { BackToTitle(); }
	};
}

void ScenePause::Init()
{
	m_menuUIHandle = LoadGraph(L"data/Graphs/Game/MenuUI.png");
	m_selectMenuUIHandle = LoadGraph(L"data/Graphs/Game/MenuUI_Select.png");

	m_titleFontHandle = CreateFontToHandle(Game::kMainFontName, kTitleFontSize, 0);
	m_titleStringWidth = GetDrawStringWidthToHandle(kTitleString.data(), kTitleString.size(), m_titleFontHandle);

	m_menuFontHandle = CreateFontToHandle(Game::kMainFontName, kMenuFontSize, 0);
}

void ScenePause::End()
{
	DeleteGraph(m_menuUIHandle);
	DeleteGraph(m_selectMenuUIHandle);

	DeleteFontToHandle(m_titleFontHandle);
	DeleteFontToHandle(m_menuFontHandle);
}

void ScenePause::Update()
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
	// スタートボタン、Bボタンでポーズを解除する
	if (input.IsTriggerd(XINPUT_BUTTON_START) || input.IsTriggerd(XINPUT_BUTTON_B))
	{
		Resume();
		return;
	}

	// 選択中のUIの大きさをlerpで動かす
	for (int i = 0; i < static_cast<int>(Menu::Num); i++)
	{
		if (m_selectIndex == i)
		{
			m_menuScales[i] = std::lerp(m_menuScales[i], kMaxMenuScale, kLerpT);
		}
		else
		{
			m_menuScales[i] = std::lerp(m_menuScales[i], 1.0f, kLerpT);
		}

		if (m_selectIndex == i)
		{
			m_menuColorRate[i] = std::lerp(m_menuColorRate[i], 1.0f, kLerpT);
		}
		else
		{
			m_menuColorRate[i] = std::lerp(m_menuColorRate[i], 0.0f, kLerpT);
		}
	}
}

void ScenePause::Draw()
{
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 128);
	DrawBox(0, 0, Game::kScreenWidth, Game::kScreenHeight, 0x000000, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	DrawStringToHandle(Game::kScreenWidth / 2 - m_titleStringWidth / 2,
					   Game::kScreenHeight / 2 - kTitleFontSize / 2 - 100,
					   kTitleString.data(), 0xffffff, m_titleFontHandle);

	for (int i = 0; i < static_cast<int>(Menu::Num); i++)
	{
		int handle = m_menuUIHandle;
		if (i == m_selectIndex) handle = m_selectMenuUIHandle;
		auto actionName = m_menuActions[i].name;

		int origBlendMode, origBlendParam;
		GetDrawBlendMode(&origBlendMode, &origBlendParam);

		int black = (1.0f - m_menuColorRate[i]) * 255;
		int red = m_menuColorRate[i] * 255;
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, black);
		DrawRotaGraph(Game::kScreenWidth / 2, kMenuStartY + kMenuFontSize / 2 + (kMenuFontSize + kMenuMargin) * i, m_menuScales[i], 0.0, m_menuUIHandle, true);
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, red);
		DrawRotaGraph(Game::kScreenWidth / 2, kMenuStartY + kMenuFontSize / 2 + (kMenuFontSize + kMenuMargin) * i, m_menuScales[i], 0.0, m_selectMenuUIHandle, true);
		SetDrawBlendMode(origBlendMode, origBlendParam);

		// 文字の背景の描画
		//DrawRotaGraph(Game::kScreenWidth / 2, kMenuStartY + kMenuFontSize / 2 + (kMenuFontSize + kMenuMargin) * i, m_menuScales[i], 0.0, handle, true);
		// 文字の描画
		int width = GetDrawExtendStringWidthToHandle(m_menuScales[i], actionName.c_str(), actionName.size(), m_menuFontHandle);
		DrawExtendStringToHandle(Game::kScreenWidth / 2 - width / 2,
						   kMenuStartY + (kMenuFontSize + kMenuMargin) * i,
						   m_menuScales[i], m_menuScales[i], actionName.c_str(), kMenuTextColor, m_menuFontHandle);
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
	m_sceneManager.PushScene(std::make_shared<SceneOption>(m_sceneManager));
}

void ScenePause::BackToTitle()
{
	m_sceneManager.ResetSceneWithFade(std::make_shared<SceneTitle>(m_sceneManager));
}
