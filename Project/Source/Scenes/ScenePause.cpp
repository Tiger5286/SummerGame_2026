#include "ScenePause.h"
#include "DxLib.h"
#include "Game.h"
#include "Singleton/Input.h"
#include "SceneManager.h"
#include <map>
#include <string_view>
#include "Singleton/SoundManager.h"

#include "SceneTitle.h"
#include "SceneOption.h"

namespace
{
	constexpr std::wstring_view kTitleString = L"ポーズ";
	constexpr int kTitleFontSize = 70;

	constexpr int kMenuStartY = 450;
	enum class MenuList
	{
		Resume,
		Option,
		BackToTitle,

		Num
	};
}

ScenePause::ScenePause(SceneManager& sceneManager) :
	SceneBase(sceneManager)
{
}

void ScenePause::Init()
{
	m_titleFontHandle = CreateFontToHandle(Game::kMainFontName, kTitleFontSize, 0);
	m_titleStringWidth = GetDrawStringWidthToHandle(kTitleString.data(), kTitleString.size(), m_titleFontHandle);

	// メニューのアクションを設定する
	std::vector<Menu::Funcs> funcs;
	funcs.resize(static_cast<int>(MenuList::Num));
	funcs[static_cast<int>(MenuList::Resume)] = {
		.name = L"再開",
		.action = [this]() {Resume(); }
	};
	funcs[static_cast<int>(MenuList::Option)] = {
		.name = L"オプション",
		.action = [this]() {Option(); }
	};
	funcs[static_cast<int>(MenuList::BackToTitle)] = {
		.name = L"タイトルに戻る",
		.action = [this]() {BackToTitle(); }
	};
	m_menu.Init(funcs, Game::kScreenWidth / 2, kMenuStartY);

	SoundManager::GetInstance().PlaySoundGame(L"Decision");
}

void ScenePause::End()
{
	DeleteFontToHandle(m_titleFontHandle);
}

void ScenePause::Update()
{
	auto& input = Input::GetInstance();

	// スタートボタン、Bボタンでポーズを解除する
	if (input.IsTriggerd(XINPUT_BUTTON_START) || input.IsTriggerd(XINPUT_BUTTON_B))
	{
		Resume();
		return;
	}

	m_menu.Update();
}

void ScenePause::Draw()
{
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 128);
	DrawBox(0, 0, Game::kScreenWidth, Game::kScreenHeight, 0x000000, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	DrawStringToHandle(Game::kScreenWidth / 2 - m_titleStringWidth / 2,
					   Game::kScreenHeight / 2 - kTitleFontSize / 2 - 100,
					   kTitleString.data(), 0xffffff, m_titleFontHandle);

	m_menu.Draw();

#ifdef _DEBUG
	DrawString(0, 0, L"ScenePause", 0xffffff);
#endif
}

void ScenePause::Resume()
{
	m_sceneManager.PopScene();
	SoundManager::GetInstance().PlaySoundGame(L"Cancel");
}

void ScenePause::Option()
{
	m_sceneManager.PushScene(std::make_shared<SceneOption>(m_sceneManager));
}

void ScenePause::BackToTitle()
{
	m_sceneManager.ResetSceneWithFade(std::make_shared<SceneTitle>(m_sceneManager));
	SoundManager::GetInstance().PlaySoundGame(L"Cancel");
}
