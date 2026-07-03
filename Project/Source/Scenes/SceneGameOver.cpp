#include "SceneGameOver.h"
#include "DxLib.h"
#include "Singleton/Input.h"
#include "SceneManager.h"
#include "SceneTitle.h"

SceneGameOver::SceneGameOver(SceneManager& sceneManager) :
	SceneBase(sceneManager)
{
}

void SceneGameOver::Init()
{
}

void SceneGameOver::End()
{
}

void SceneGameOver::Update()
{
	auto& input = Input::GetInstance();
	if (input.IsTriggerd(XINPUT_BUTTON_A))
	{
		m_sceneManager.ChangeScene(std::make_shared<SceneTitle>(m_sceneManager),false);
		return;
	}
}

void SceneGameOver::Draw()
{
#ifdef _DEBUG
	DrawString(0, 0, L"SceneGameOver", 0xffffff);
	DrawString(0, 16, L"Aボタンでタイトル", 0xffffff);
#endif
}
