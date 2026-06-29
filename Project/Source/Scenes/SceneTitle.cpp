#include "SceneTitle.h"
#include "DxLib.h"
#include "Singleton/Input.h"
#include "SceneManager.h"
#include "SceneMain.h"

SceneTitle::SceneTitle(SceneManager& sceneManager) :
	SceneBase(sceneManager)
{

}

void SceneTitle::Init()
{

}

void SceneTitle::End()
{

}

void SceneTitle::Update()
{
	auto& input = Input::GetInstance();
	if (input.IsTriggerd(XINPUT_BUTTON_A))
	{
		m_sceneManager.ChangeScene(std::make_shared<SceneMain>(m_sceneManager),true);
		return;
	}
}

void SceneTitle::Draw()
{
	DrawString(0, 0, L"SceneTitle Aボタンでスタート", 0xffffff);
}
