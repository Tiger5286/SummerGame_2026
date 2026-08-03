#include "SceneDebug.h"
#include <string_view>
#include <array>
#include "Singleton/Input.h"

#include "SceneManager.h"
#include "SceneTitle.h"
#include "SceneStageSelect.h"
#include "SceneMain.h"
#include "ScenePause.h"
#include "SceneOption.h"
#include "SceneClear.h"
#include "SceneGameOver.h"

namespace
{
	constexpr std::array<std::wstring_view, static_cast<int>(SceneDebug::Scene::Num)> kSceneNames =
	{
		L"Title",
		L"StageSelect",
		L"Main",
		L"Pause",
		L"Option",
		L"Clear",
		L"GameOver"
	};

	constexpr int kDefaultFontSize = 16;
	constexpr int kStringMargin = 8;

	SceneMain::UniqueFiles kUniqueFiles =
	{
		.collisionFilePath = L"data/Stages/First/Collision.mv1",
		.stageFilePath = L"data/Stages/First/Stage.mv1",
		.spawnerDataFilePath = L"data/Stages/First/SpawnerData.dat"
	};
}

SceneDebug::SceneDebug(SceneManager& sceneManager) :
	SceneBase(sceneManager)
{
	m_sceneChangeFuncs[static_cast<int>(Scene::Title)]		 = [this]() { m_sceneManager.ChangeScene(std::make_shared<SceneTitle>	   (m_sceneManager), false); };
	m_sceneChangeFuncs[static_cast<int>(Scene::StageSelect)] = [this]() { m_sceneManager.ChangeScene(std::make_shared<SceneStageSelect>(m_sceneManager), false); };
	m_sceneChangeFuncs[static_cast<int>(Scene::Main)]		 = [this]() { ChangeSceneSceneMain(); };
	m_sceneChangeFuncs[static_cast<int>(Scene::Pause)]		 = [this]() { m_sceneManager.PushScene(  std::make_shared<ScenePause>      (m_sceneManager)		  ); };
	m_sceneChangeFuncs[static_cast<int>(Scene::Option)]		 = [this]() { m_sceneManager.PushScene(  std::make_shared<SceneOption>     (m_sceneManager)		  ); };
	m_sceneChangeFuncs[static_cast<int>(Scene::Clear)]		 = [this]() { m_sceneManager.ChangeScene(std::make_shared<SceneClear>      (m_sceneManager), false); };
	m_sceneChangeFuncs[static_cast<int>(Scene::GameOver)]	 = [this]() { m_sceneManager.ChangeScene(std::make_shared<SceneGameOver>   (m_sceneManager), false); };
}

SceneDebug::~SceneDebug()
{
}

void SceneDebug::Init()
{
}

void SceneDebug::End()
{
}

void SceneDebug::Update()
{
	auto& input = Input::GetInstance();
	// 上下で選択
	if (input.IsTriggerd(XINPUT_BUTTON_DPAD_DOWN, true))
	{
		int next = static_cast<int>(m_selectScene) + 1;
		if (next >= static_cast<int>(Scene::Num))
		{
			next = 0;
		}
		m_selectScene = static_cast<Scene>(next);
	}
	if (input.IsTriggerd(XINPUT_BUTTON_DPAD_UP, true))
	{
		int next = static_cast<int>(m_selectScene) - 1;
		if (next < 0)
		{
			next = static_cast<int>(Scene::Num) - 1;
		}
		m_selectScene = static_cast<Scene>(next);
	}
	// Aで決定
	if (input.IsTriggerd(XINPUT_BUTTON_A))
	{
		m_sceneChangeFuncs[static_cast<int>(m_selectScene)]();
		return;
	}
}

void SceneDebug::Draw()
{
	DrawString(0, 0, L"SceneDebug", 0xffffff);

	unsigned int color = 0xffffff;
	for (int i = 0; i < static_cast<int>(Scene::Num); i++)
	{
		if (static_cast<int>(m_selectScene) == i)
		{
			color = 0xff0000;
		}
		DrawString(50, 50 + (kDefaultFontSize + kStringMargin) * i, kSceneNames[i].data(), color);
		color = 0xffffff;
	}
}

void SceneDebug::ChangeSceneSceneMain()
{
	auto sceneMain = std::make_shared<SceneMain>(m_sceneManager);
	sceneMain->SetData(kUniqueFiles);
	m_sceneManager.ChangeScene(sceneMain, true);
}
