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

	constexpr int kMenuStartY = 550;

	enum class MenuList
	{
		Retry,
		Title,

		Num
	};
}

SceneGameOver::SceneGameOver(SceneManager& sceneManager) :
	SceneBase(sceneManager)
{
}

void SceneGameOver::Init()
{
	m_backHandle = LoadGraph(L"data/Graphs/StoneBrick.png");
	m_logoHandle = LoadGraph(L"data/Graphs/GameOverLogo.png");

	std::vector<Menu::Funcs> funcs;
	funcs.resize(static_cast<int>(MenuList::Num));
	funcs[static_cast<int>(MenuList::Retry)] = {
		.name = L"もういちど挑戦",
		.action = [this]() {Retry(); }
	};
	funcs[static_cast<int>(MenuList::Title)] = {
		.name = L"タイトルに戻る",
		.action = [this]() {Title(); }
	};
	m_menu.Init(funcs, Game::kScreenWidth, kMenuStartY);
}

void SceneGameOver::End()
{
	DeleteGraph(m_backHandle);
	DeleteGraph(m_logoHandle);
}

void SceneGameOver::Update()
{
	m_menu.Update();
}

void SceneGameOver::Draw()
{
	// 背景
	DrawRotaGraph(Game::kScreenWidth / 2, Game::kScreenHeight / 2, kBackScale, 0.0, m_backHandle, false);
	// ロゴ
	DrawRotaGraph(Game::kScreenWidth / 2, Game::kScreenHeight / 2 + kLogoOffsetY, 1.0, 0.0, m_logoHandle, true);

	m_menu.Draw();

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
