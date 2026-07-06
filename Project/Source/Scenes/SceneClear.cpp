#include "SceneClear.h"
#include "DxLib.h"
#include "Singleton/Input.h"
#include "SceneManager.h"
#include "SceneTitle.h"

SceneClear::SceneClear(SceneManager& sceneManager) :
	SceneBase(sceneManager)
{
}

void SceneClear::Init()
{
}

void SceneClear::End()
{
}

void SceneClear::Update()
{
	auto& input = Input::GetInstance();
	if (input.IsTriggerd(XINPUT_BUTTON_A))
	{
		m_sceneManager.ChangeSceneWithFade(std::make_shared<SceneTitle>(m_sceneManager), false);
		return;
	}
}

void SceneClear::Draw()
{
	DrawString(0, 0, L"SceneClear\n", 0xffffff);
	DrawString(0, 16, L"Aボタンでタイトルに戻る\n", 0xffffff);
}
